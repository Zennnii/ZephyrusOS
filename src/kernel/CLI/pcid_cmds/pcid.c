#include "pcid.h"

void meminfof() {
    if (mbi->flags & 1) {
        uint32_t mem_low = mbi->mem_lower / 1024;
        uint32_t mem_high = mbi->mem_upper / 1024;
        uint32_t mem_total = mem_low + mem_high;

        print("Total system RAM: "); print_dec(mem_total); print("MB"); newLine();
    }
}