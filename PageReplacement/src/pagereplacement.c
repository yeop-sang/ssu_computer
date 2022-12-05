//
// Created by Sangyeop Kim on 2022/11/21.
//

#include "fifo.h"

char *const print_original_string = "Print Original Frames";
char *const split_line = "===========================";
char *const page_fault_string = "Occurring Page Fault!";
char *const print_changed_string = "Print Changed Frames";

// Is there page number?
int find_page_number(unsigned char page, PageStream *frame, int frame_num) {
    for (int i = 0; i < frame_num; ++i) {
        if (frame[i].number == page)
            return i;
    }
    // Occurring page fault, the func returns -1
    return -1;
}

// find replacement index following algorithm
int find_replace_index(int mode, PageStream *frame, int frame_num) {
    int res = 0;
    PageStream *res_frame = frame + res, *cur;

    // When an empty frame or more exist, return empty frame index
    if((res = find_page_number(0,frame, frame_num))> 0)
        return res;

    if (is_mode(mode, FIFO))
        for (int i = 1; i < frame_num; ++i) {
            cur = frame + i;
            // find the frame have the smallest history number
            if (cur->history < res_frame->history) {
                res = i;
                res_frame = cur;
            }
        }
    else
        for (int i = 1; i < frame_num; ++i) {
            cur = frame + i;
            // find the frame have the biggest history number
            if (cur->history > res_frame->history) {
                res = i;
                res_frame = cur;
            }
        }
    return res;
}

// FIFO or LIFO algorithm
int fifo_lifo(int mode, PageStream *ps, int page_num, int frame_num) {
    // Info. var.
    int pagefault = 0;

    // Initialize frame
    PageStream frame[frame_num];
    for (int i = 0; i < frame_num; ++i) {
        frame[i].number = 0;
        frame[i].history = 0;
    }

    // PageReplacement algorithm
    PageStream *ps_cur, *frame_cur;
    int replace_index;
    for (int i = 0; i < page_num; ++i) {
        ps_cur = ps + i;
        // print original frame
        printf("%s\n", split_line);
        printf("%dth %s\n", i, print_original_string);
        printPageStream(frame, mode, frame_num);
        printf("%s\n", split_line);
        // Occurring page fault
        if (find_page_number(ps_cur->number, frame, frame_num) < 0) {
            printf("%s\n", page_fault_string);
            // Add page Fault Count
            pagefault++;
            
            // find First-In frame
            replace_index = find_replace_index(mode, frame, frame_num);
            frame_cur = frame + replace_index;
            // Replace new frame
            copyFromPageStreamToFrame(ps_cur, frame_cur);
            // update history
            frame_cur->history = i;
        }
        // print changed frame
        printf("%s\n", print_changed_string);
        printPageStream(frame, mode, frame_num);
        printf("%s\n", split_line);
    }

    // delete frame
    return pagefault;
}

int fifoSimulation(PageStream *ps, int page_num, int frame_num) {
    return fifo_lifo(1 << FIFO, ps, page_num, frame_num);
}

int lifoSimulation(PageStream *ps, int page_num, int frame_num) {
    return fifo_lifo(1 << LIFO, ps, page_num, frame_num);
}
