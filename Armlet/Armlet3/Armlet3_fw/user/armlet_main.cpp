#include <string.h>
#include <stdlib.h>

#include "lcd2630.h"
#include "chthreads.h"
#include "Font2.h"
#include "lcd_addon.h"
#include "chheap.h"

#include "MemFont.h"

static WORKING_AREA(waUserThread, 4096);
msg_t armlet_main_thd(void*);

void armlet_main()
{
    chThdCreateStatic(waUserThread, sizeof(waUserThread), NORMALPRIO, armlet_main_thd, NULL);

    while (1)
    {
        chThdSleepSeconds(1);
    }
}

#include "DejaVuSansMono14.txt"

const uint8_t image1[] = {
#include "icons/2a_help_01.txt"
};

const uint8_t image2[] = {
#include "icons/2k_heart.txt"
};

// ``user space'' entry point
msg_t armlet_main_thd(void*)
{
    chThdSleepSeconds(1);

    lcd_putMemoryImage(10, 10, image1, 0x00F);

    lcd_putMemoryImage(50, 10, image2);

    while (1)
    {
        chThdSleepSeconds(1);
    }

    return 0;
}
