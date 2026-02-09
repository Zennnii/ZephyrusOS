#include "pcid.h"

void init_pcid(void) {
    uint64_t ram = total_ram;
    LOG_INFO_NONL("Total amount of usable system RAM: ");
    serial_write_int(total_ram);
}