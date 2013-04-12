#include <string.h>
#include <stdlib.h>

#include "lcd2630.h"
#include "chthreads.h"
#include "Font.h"
#include "lcd_addon.h"
#include "chheap.h"

void mmm()
{
    Font f("font.dat");
    f.drawString(5, 50, 0x0F00, "Hello world!");
}

// ``user space'' entry point
void armlet_main()
{
    mmm();
    lcd_putImage(160-32, 20, "1.bmp", 0x0F00, 0x0000);
    Uart.Printf("sizeof FIL = %d\n", sizeof(FIL));

//    lcd_putImage(160-32, 40, "3.bmp", 0x0F00, 0x0000);
//    lcd_putImage(160-32, 60, "4.bmp", 0x0F00, 0x0000);
//    lcd_putImage(160-32, 80, "5.bmp", 0x0F00, 0x0000);

    while (1)
    {
        chThdSleepSeconds(1);
    }
}
