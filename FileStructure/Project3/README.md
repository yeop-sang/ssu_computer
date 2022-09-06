# 과제 3: Flash Memory에서의 Block Mapping FTL 구현

## 명세서

### 개요

<u>**Block mapping 기법**</u>(강의자료 “Flash Memory Overview”)을 따르는 FTL layer를 구현하시오.

네 개의 소스 파일이 주어지며 이것을 이용하여 상기의 FTL을 구현하시오.

1. blkmap.h : flash memory를 구성하는 block, page, sector, spare area 등의 상수 정의와
   address mapping table에 대한 구조체가 정의 되어 있음
2. devicedriver.c : flash memory에 page 단위로 데이터를 읽고 쓰기 위한 read()와 write() 함수와,
   또한 block을 소거하는 erase() 함수가 정의되어 있음
3. ftl.c: 상기의 FTL 기법을 따르는 ftl_open(), ftl_write(), ftl_read()를 <u>**구현해야 됨**</u>
4. main.c: file system의 역할을 수행하며 ftl_write()와 ftl_read() 테스트를 위해 제공하는 것으로 필요하면 활용하기 바람.

### 준수 사항

1. flash memory를 위한 파일 명은 반드시 '**<u>flashmemory</u>**' 이어야 하며(main.c 참조),
   또한 실행 디렉토리에 생성되어야 함
2. 프로그램 작성 시 반드시 **blkmap.h에 정의되어 있는 상수**를 사용해야 하며,
   상수 값은 "수정가능"으로 표시된 것만 원하는 갓으로 바꿀 수 있음
3. flash memory는 초기에 모든 byte 값이 '0xFF'로 설정되며(main.c 참조)
   다른 값으로 초기화하면 채점 시 제대로 동작하지 않음
4. block mapping 기법에서는 굳이 spare are에 lsn과 같은 필요한 정보를 넣을 필요가 없으나,
   공부하는 차원에서 spare area에 lsn을 저장할 것
5. <u>FTL layer에서 flashmemory에 직접 데이터를 읽고 쓸 수 없으며,
   반드시 제공되는 device driver에서 제공하는 interface를 호출해야함.</u>
   그렇지 않은 경우 채점 프로그램이 제대로 동작하지 않음

### 개발 환경

- OS: Linux Ubuntu 18.04
- compiler: gcc 7.5

### 제출물

- 완성된 ftl.c와 blkmap.h 두 개의 파일을 하위폴더 없이 zip 파일로 압축하여 과제 게시판에 제출함
- 압축한 파일은 반드시 20192380_3.zip 과 같이 작성

---

## 과제 분석

### FTL 정의

- NAND flash memory가 hard disk 같은 일반적인 block 장치처럼 보이게 변환
- wear-leveling(셀을 골고루 돌아가며 읽기/쓰기를 진행)
- flash memory의 물리적 제약(한정된 공간, In-place update 불가능 등)을 software적으로 해결
- file system의 logical address를 flash memory의 physical address로 mapping
- out-of-place update를 가능하도록 제공

### Block mapping 기법

- block 단위로 logical-physical address mapping table이 작동
- 삽입하는 방법
    1. mapping 할 값 계산
        1. physical sector offset 계산 : logical sector number % SECTORS_PER_BLOCK
        2. logical block number 계산   : logical sector number // SECTORS_PER_BLOCK
    2. 해당 lsn이 사용되는 중인지 확인
       - 만일 사용중이라면 해당 위치의 spare를 제거
    3. 비어 있는 공간 탐색
        - 테이블을 순회하며 해당 block의 sector offset에 해당하는 공간이 비어 있으면 그 공간에 삽입
    4. 빈 공간이 있나?
       - 있으면 삽입한 위치에 대한 값을 mapping table에 저장
       - 없으면
         1. 사용하지 않는 페이지 삭제 

### 주어진 file 내용 분석

1. lkmap.h
    1. 상수
        - pages per block (수정 가능)
        - blocks per device (수정 가능)
        - 등등
    2. 구조체
        - SpareData : flash memory의 spare area를 다루기 위한 구조체
            1. lsn(logical sector number)
            2. dummy: 데이터
        - AddrMapTal : address mapping table을 위한 구조체
            1. pbn(physical block number)

2. **devicedriver.c**
    - ftl의 동작을 c언어 함수로 구현
    - file을 flash memory라 가정하고 작동한다.

    1. nt dd_read(int ppn, char* pagebuf) : ppn에서 pagebuf data 읽어옴
        1. 매개변수
            - ppn(physical page number)
            - pagebuf : pagebuf에 읽어온 값 저장
        2. return value
            - 성공시 1 (한 페이지만 읽어온 경우를 성공이라 함!)
            - 실패시 -1
    2. int dd_write(int ppn, char* pagebuf) : ppn에 pagebuf data 저장
        1. 매개변수
            - ppn(physical page number)
            - pagebuf : flash memory에 적어야 하는 값에 대한 포인터
        2. return value
            - 성공시 1 (한 페이지만 읽어온 경우를 성공이라 함!)
            - 실패시 -1
    3. int dd_erase(int pbn) : pbn에 **0xFF**값(초기화 값)을 덮어쓴다 -> 페이지 삭제
        1. 매개변수
            - pbn(physical block number)
        2. return value
            - 성공시 1 (한 페이지만 읽어온 경우를 성공이라 함!)
            - 실패시 -1

3. **ftl.c** : 과제를 수행해야하는 파일
    1. ftl_open: flash memory 처음 사용시 필요한 초기화 작업, address mapping table 초기화 등등
    2. ftl_write(int lsn, char* sectorbuf)
        - file system을 위한 FTL이 제공하는 write interface
        - block mapping 기법에서 overwrite이

        1. 매개변수
            - lsn(logical sector number)
            - sectorbuf의 size는 반드시 SECTOR_SIZE이다.
    3. 