#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <setjmp.h>

static jmp_buf global_buf;

void ssu_nested_func(int loc_var, int loc_volatile, int loc_register) {
    printf(
            "before longjmp, "
            "loc_var = %d, "
            "loc_volatile = %d,"
            "loc_register = %d\n",
            loc_var,
            loc_volatile,
            loc_register
    );
    longjmp(global_buf, 1);
}

int main(void) {
    register int loc_register;
    volatile int loc_volatile;
    int loc_var;

    loc_var = 10;
    loc_volatile = 11;
    loc_register = 12;

    if (setjmp(global_buf) != 0) {
        printf("after longjmp, loc_var = %d, loc_volatile = %d, loc_register = %d\n",
               loc_var, loc_volatile, loc_register);
        exit(0);
    }

    loc_var = 80;
    loc_volatile = 81;
    loc_register = 83;
    ssu_nested_func(loc_var, loc_volatile, loc_register);
    exit(0);
}
