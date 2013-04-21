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

// ``user space'' entry point
msg_t armlet_main_thd(void*)
{
    chThdSleepSeconds(1);
    Lcd.Cls((Color_t)0x0FFF);

//    Font2 f2("DejaVuSansMono14.dat");
//    f2.drawString(5, 0, 0x0000, "Hello world!");
//    f2.drawString(5, 16, 0x0000, "Прекрасный день");
//    f2.drawString(5, 32, 0x0000, "чтобы умереть!");

    memfont_drawString(largeFont, 5, 0, "Hello world!", 0x0000);

    uint16_t c = 0;
    while (1)
    {
//        Lcd.Cls((Color_t)c);
        chThdSleepSeconds(1);

        c += 1;
        if (c >= 0x1000)
            c = 0;
    }

    return 0;
}
