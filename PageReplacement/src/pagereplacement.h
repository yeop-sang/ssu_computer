//
// Created by Sangyeop Kim on 2022/11/21.
//

#ifndef PAGEREPLACEMENT_FIFO_H
#define PAGEREPLACEMENT_FIFO_H

#include "pagereferencestring.h"

int fifoSimulation(PageStream *ps, int page_num, int frame_num);
int lifoSimulation(PageStream *ps, int page_num, int frame_num);

#endif //PAGEREPLACEMENT_FIFO_H
