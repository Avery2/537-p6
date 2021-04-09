#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "ptentry.h"

#define PGSIZE 4096


static int 
err(char *msg, ...) {
    printf(1, "XV6_TEST_OUTPUT %s\n", msg);
    exit();
}

int 
main(void){
    const uint PAGES_NUM = 5;
    
    // Allocate one pages of space
    char *ptr = sbrk(PGSIZE * sizeof(char));
    while ((uint)ptr != 0x6000)
        ptr = sbrk(PGSIZE * sizeof(char));

    ptr = sbrk(PAGES_NUM * PGSIZE);
    struct pt_entry pt_entries[PAGES_NUM];

    // Call the mencrypt with len = PAGES_NUM - 1
    if (mencrypt(ptr, PAGES_NUM) != 0)
        err("mencrypt return non-zero value when mencrypt is called on valid range\n");
    
    int pid = fork();

    if (pid == 0) {
        // Child process goes here
        if (getpgtable(pt_entries, PAGES_NUM) >= 0) {
            for (int i = 0; i < PAGES_NUM; i++) {
                printf(1, "XV6_TEST_OUTPUT Index %d: pdx: 0x%x, ptx: 0x%x, present: %d, writable: %d, encrypted: %d\n", 
                    i,
                    pt_entries[i].pdx,
                    pt_entries[i].ptx,
                    pt_entries[i].present,
                    pt_entries[i].writable,
                    pt_entries[i].encrypted
                );
            }
        }
    }

    wait();
    exit();
}
