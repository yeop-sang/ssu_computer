#define PRINT_FOR_DEBUG

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <time.h>
#include "blkmap.h"

AddrMapTable addrmaptbl;
extern FILE *devicefp;

int reserved_empty_blk = DATABLKS_PER_DEVICE;

/****************  prototypes ****************/
void ftl_open();

void ftl_write(int lsn, char *sectorbuf);

void ftl_read(int lsn, char *sectorbuf);

void print_block(int pbn);

void print_addrmaptbl();

/********* my prototypes ***********/
void print_addrmaptbl_info(int lsn);

int cal_sector_offset(int lsn) {
    return lsn % SECTORS_PER_BLOCK;
}

int cal_lbn(int lsn) {
    return lsn / SECTORS_PER_BLOCK;
}

int cal_ppn(int pbn, int sector_offset) {
    return pbn * PAGES_PER_BLOCK + sector_offset;
}

void parse_page(char *pagebuf, char *sectorbuf, SpareData *sparebuf) {
    if (sectorbuf != NULL)
        memcpy(sectorbuf, pagebuf, SECTOR_SIZE);
    if (sparebuf != NULL)
        memcpy(sparebuf, pagebuf + SECTOR_SIZE, SPARE_SIZE);
}

char *create_page(char *pagebuf, char *sectorbuf, SpareData *sparebuf) {
    if (sectorbuf != NULL)
        memcpy(pagebuf, sectorbuf, SECTOR_SIZE);
    else
        // 들어온 값이 없으면 기본값으로 초기화
        memset(pagebuf, -1, SECTOR_SIZE);
    if (sparebuf != NULL)
        memcpy(pagebuf + SECTOR_SIZE, sparebuf, SPARE_SIZE);
    else
        memset(pagebuf + SECTOR_SIZE, -1, SPARE_SIZE);
    return pagebuf;
}

char *get_page(int ppn, char *pagebuf) {
    if (dd_read(ppn, pagebuf) < 0) {
        return NULL;
    }
    return pagebuf;
}

int is_used(int ppn, SpareData *sparebuf) {
    // page 정보를 읽어올 변수
    char pagebuf[PAGE_SIZE];

    // 읽어오는 데 실패하면 -1
    if (get_page(ppn, pagebuf) == NULL) {
        fprintf(stderr, "read fail\n");
        // 사용할 수 없는 블록은 사용 중이라 판단해도 무방
        return -1;
    }

    // page데이터를 분할한다.
    parse_page(pagebuf, NULL, sparebuf);

    // lsn이 양수이면 현재 사용중
    if (sparebuf->lsn >= 0)
        return 1;
        // spare data의 lsn이 음수이면 사용중이지 않음
        // 1. 모든 바이트가 FF로 초기화되어 변경되지 않았으면 -1
    else if (sparebuf->lsn == -1)
        return 0;
        // 2. -1이 아니면 현재 삭제된 page
    else
        return -2;
}

void move_block(int new_pbn, int old_pbn, int except_sector_offset) {
    char pagebuf[PAGE_SIZE];
    SpareData spare;
    char saved_page[SECTORS_PER_PAGE][PAGE_SIZE];
    int saved_element = 0;

    // 원래 데이터 복사
    for (int temp_sector_offset = 0; temp_sector_offset < SECTORS_PER_BLOCK; ++temp_sector_offset) {
        // 건너뛰어야하는 데이터의 경우 건너뜀!
        if (except_sector_offset == temp_sector_offset)
            continue;

        get_page(
                cal_ppn(old_pbn, temp_sector_offset),
                pagebuf
        );

        parse_page(pagebuf, NULL, &spare);

        // 사용중인 block 일때는 복사
        if (spare.lsn > 0) {
            memcpy(saved_page[temp_sector_offset], pagebuf, PAGE_SIZE);
            // 및 비트 마스킹
            saved_element |= 1 << temp_sector_offset;
        }
    }

    // 새로운 pbn에 저장
    for (int temp_sector_offset = 0; temp_sector_offset < SECTORS_PER_BLOCK; ++temp_sector_offset) {
        // 저장되었으면 해당 블록에 쓴다.
        if (saved_element & (1 << temp_sector_offset))
            dd_write(
                    cal_ppn(new_pbn, temp_sector_offset),
                    saved_page[temp_sector_offset]
            );
    }

    // 옮겨진 애들에 대해서 table 수정
    for (int lbn = 0; lbn < DATABLKS_PER_DEVICE; ++lbn) {
        if (addrmaptbl.pbn[lbn] == old_pbn) {
            addrmaptbl.pbn[lbn] = new_pbn;
            break;
        }
    }

    // 원래 블록 삭제
    dd_erase(reserved_empty_blk);
}

//
// flash memory를 처음 사용할 때 필요한 초기화 작업, 예를 들면 address mapping table에 대한
// 초기화 등의 작업을 수행한다
//
void ftl_open() {
    int i;

    // initialize the address mapping table
    for (i = 0; i < DATABLKS_PER_DEVICE; i++) {
        addrmaptbl.pbn[i] = -1;
    }

    //
    // 추가적으로 필요한 작업이 있으면 수행하면 되고 없으면 안해도 무방함
    //

    return;
}

//
// file system을 위한 FTL이 제공하는 write interface
// 'sectorbuf'가 가리키는 메모리의 크기는 'SECTOR_SIZE'이며, 호출하는 쪽에서 미리 메모리를 할당받아야 함
//
void ftl_write(int lsn, char *sectorbuf) {
#ifdef PRINT_FOR_DEBUG            // 필요 시 현재의 block mapping table을 출력해 볼 수 있음

    print_addrmaptbl_info(lsn);

#endif

//
// block mapping 기법에서 overwrite가 발생하면 이를 해결하기 위해 반드시 하나의 empty block이
// 필요하며, 초기값은 flash memory에서 맨 마지막 block number를 사용함
// overwrite를 해결하고 난 후 당연히 reserved_empty_blk는 overwrite를 유발시킨 (invalid) block이 되어야 함
// 따라서 reserved_empty_blk는 고정되어 있는 것이 아니라 상황에 따라 계속 바뀔 수 있음
//

    // lbn이 범위를 넘어 섰는지 판단
    if (lsn >= DATABLKS_PER_DEVICE * SECTORS_PER_BLOCK) {
        fprintf(stderr, "lsn is out of range.\n");
        return;
    }

    // 0. 삽입시 필요한 정보
    char pagebuf[PAGE_SIZE];
    SpareData spare;

    // 1. mapping 시 필요한 값 게산
    int sector_offset = cal_sector_offset(lsn);
    int lbn = cal_lbn(lsn);

    // 2. 해당 lbn이 사용중인지 확인
    if (addrmaptbl.pbn[lbn] != -1) {
        // ppn 계산
        int ppn = cal_ppn(
                addrmaptbl.pbn[lbn],
                sector_offset
        );

        // 만약 해당 block이 사용 중이 아니라면
        if (!is_used(ppn, pagebuf)) {
            // spare에 lsn 값 저장
            spare.lsn = lsn;

            // 필요한 정보 저장
            dd_write(
                    cal_ppn(addrmaptbl.pbn[lbn], sector_offset),
                    create_page(pagebuf, sectorbuf, &spare)
            );

        }

            // 만약 사용중이 아니라면
        else {
            // 해당 블록을 다른 블록으로 copy
            int old_pbn = addrmaptbl.pbn[lbn];

            move_block(reserved_empty_blk, addrmaptbl.pbn[lbn], sector_offset);
            // 새로 작성한 블록에 넣어야 하는 페이지 삽입

            // spare에 lsn 값 저장
            spare.lsn = lsn;

            dd_write(
                    cal_ppn(
                            reserved_empty_blk,
                            sector_offset
                    ), create_page(pagebuf, sectorbuf, &spare)
            );

            // 이후 reserved_empty_blk 수정
            reserved_empty_blk = old_pbn;
        }

        // 이후 종료
        return;
    }

    // 새로운 lbn 할당 작업
    // 3. 삽입할 위치 찾기
    int pbn = 0, unused_pbn = -1, state = 0;
    for (pbn = 0; pbn < BLOCKS_PER_DEVICE; ++pbn) {
        // reserved_empty_blk는 비워 둬야함!
        if (pbn == reserved_empty_blk)
            continue;

        // 비어있으면 해당 위치 사용
        state = is_used(
                cal_ppn(pbn, sector_offset),
                &spare
        );
        if (!state)
            break;
            // 삭제된 block 미리 저장
        else if (state < -1)
            unused_pbn = pbn;
    }

    // 4. 빈 공간에 삽입

    // 4-1 빈 공간을 하나 찾았을때!
    if (pbn != BLOCKS_PER_DEVICE) {
        // spare에 lsn 값 저장
        spare.lsn = lsn;
        // mapping table에 저장
        addrmaptbl.pbn[lbn] = pbn;

        // 필요한 정보 저장
        dd_write(
                cal_ppn(pbn, sector_offset),
                create_page(pagebuf, sectorbuf, &spare)
        );
    }

        // 4-2 빈 공간이 하나도 없을때!
    else {
        // 꽉 채워졌지만 사용하지 않은 page를 찾았을때!
        if (unused_pbn != -1) {
            // 임시로 저장할 원래 데이터
            char saved_page[SECTORS_PER_PAGE][PAGE_SIZE];
            int saved_element = 0;

            // 원래 데이터 저장
            for (int temp_sector_offset = 0; temp_sector_offset < SECTORS_PER_BLOCK; ++temp_sector_offset) {
                get_page(
                        cal_ppn(unused_pbn, temp_sector_offset),
                        pagebuf
                );

                parse_page(pagebuf, NULL, &spare);

                // 사용중인 block 일때는 복사
                if (spare.lsn > 0) {
                    memcpy(saved_page[temp_sector_offset], pagebuf, PAGE_SIZE);
                    // 및 비트 마스킹
                    saved_element |= 1 << temp_sector_offset;
                }
            }

            // saved_page에 삽입해야하는 정보 삽입
            memcpy(
                    pagebuf,
                    create_page(
                            pagebuf,
                            sectorbuf,
                            &spare
                    ),
                    PAGE_SIZE
            );

            // reserved_empty_blk에 해당하는 곳에 복사
            for (int temp_sector_offset = 0; temp_sector_offset < SECTORS_PER_BLOCK; ++temp_sector_offset) {
                // 저장되었으면 해당 블록에 쓴다.
                if (saved_element & (1 << temp_sector_offset))
                    dd_write(
                            cal_ppn(reserved_empty_blk, temp_sector_offset),
                            saved_page[temp_sector_offset]
                    );
            }

            // 옮겨진 애들에 대해서 table 수정
            for (int lbn = 0; lbn < DATABLKS_PER_DEVICE; ++lbn) {
                if (addrmaptbl.pbn[lbn] == unused_pbn) {
                    addrmaptbl.pbn[lbn] = reserved_empty_blk;
                    break;
                }
            }

            // 다 작성된 이후에는 reserved_empty_blk 변경 및 해당 페이지 삭제
            reserved_empty_blk = unused_pbn;
            dd_erase(reserved_empty_blk);

        } else {
            // 사용하지 않는 공간도 없을때
            fprintf(stderr,
                    "Memory is fulled!!\n"
                    "requested:\n"
                    "\tlsn : %d \n"
                    "\tdata: %s",
                    lsn, sectorbuf
            );
        }
    }

    return;
}

//
// file system을 위한 FTL이 제공하는 read interface
// 'sectorbuf'가 가리키는 메모리의 크기는 'SECTOR_SIZE'이며, 호출하는 쪽에서 미리 메모리를 할당받아야 함
// 
void ftl_read(int lsn, char *sectorbuf) {
#ifdef PRINT_FOR_DEBUG            // 필요 시 현재의 block mapping table을 출력해 볼 수 있음
    print_addrmaptbl_info(lsn);
#endif

    if (lsn >= DATABLKS_PER_DEVICE * SECTORS_PER_BLOCK) {
        fprintf(stderr, "lsn is out of range.\n");
        return;
    }

    int lbn = cal_lbn(lsn);
    int sector_offset = cal_sector_offset(lsn);
    char pagebuf[PAGE_SIZE];

    get_page(
            cal_ppn(
                    addrmaptbl.pbn[lbn],
                    sector_offset
            ),
            pagebuf
    );

    parse_page(pagebuf, sectorbuf, NULL);
}

//
// for debugging
//
void print_block(int pbn) {
    char *pagebuf;
    SpareData *sdata;
    int i;

    pagebuf = (char *) malloc(PAGE_SIZE);
    sdata = (SpareData *) malloc(SPARE_SIZE);

    printf("Physical Block Number: %d\n", pbn);

    for (i = pbn * PAGES_PER_BLOCK; i < (pbn + 1) * PAGES_PER_BLOCK; i++) {
        dd_read(i, pagebuf);
        memcpy(sdata, pagebuf + SECTOR_SIZE, SPARE_SIZE);
        printf("\t   %5d-[%7d]\n", i, sdata->lsn);
    }

    free(pagebuf);
    free(sdata);

    return;
}

//
// for debugging
//
void print_addrmaptbl() {
    int i;

    printf("Address Mapping Table: \n");
    for (i = 0; i < DATABLKS_PER_DEVICE; i++) {
        if (addrmaptbl.pbn[i] >= 0) {
            printf("[%d %d]\n", i, addrmaptbl.pbn[i]);
        }
    }
}

//
// for debug
//
void print_addrmaptbl_info(int lsn) {
    char pagebuf[PAGE_SIZE];
    printf("%d : %s\n",
           lsn,
           get_page(
                   cal_ppn(
                           addrmaptbl.pbn[
                                   cal_lbn(lsn)
                           ],
                           cal_sector_offset(lsn)
                   ),
                   pagebuf
           )
    );

}