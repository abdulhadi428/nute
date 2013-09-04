#pragma once

#include "ThreeKShell.h"
//extern ImagesRepository_ gImagesRepository;

#define NO_IMAGE -1
#define battery_0 SYS_IMAGES_COUNT+0
#define battery_1 SYS_IMAGES_COUNT+1
#define battery_2 SYS_IMAGES_COUNT+2
#define battery_3 SYS_IMAGES_COUNT+3
#define battery_4 SYS_IMAGES_COUNT+4
#define big_arrow_left SYS_IMAGES_COUNT+5
#define big_arrow_right SYS_IMAGES_COUNT+6
#define lock_closed SYS_IMAGES_COUNT+7
#define lock_open SYS_IMAGES_COUNT+8
#define music_button SYS_IMAGES_COUNT+9
#define scroll_down SYS_IMAGES_COUNT+10
#define scroll_up SYS_IMAGES_COUNT+11
#define signal_0 SYS_IMAGES_COUNT+12
#define signal_1 SYS_IMAGES_COUNT+13
#define signal_2 SYS_IMAGES_COUNT+14
#define signal_3 SYS_IMAGES_COUNT+15
#define signal_4 SYS_IMAGES_COUNT+16
#define small_arrow_bottom SYS_IMAGES_COUNT+17
#define small_arrow_left SYS_IMAGES_COUNT+18
#define small_arrow_right SYS_IMAGES_COUNT+19
#define small_arrow_right_red SYS_IMAGES_COUNT+20
#define stress_background SYS_IMAGES_COUNT+21

#define OS_IMAGES_COUNT stress_background + 1 - SYS_IMAGES_COUNT

#define APP_IMAGES_COUNT 27

#define BlueArrowDown SYS_IMAGES_COUNT + OS_IMAGES_COUNT+0
#define BlueArrowLeft SYS_IMAGES_COUNT + OS_IMAGES_COUNT+1
#define BlueArrowRight SYS_IMAGES_COUNT + OS_IMAGES_COUNT+2
#define BlueArrowUp SYS_IMAGES_COUNT + OS_IMAGES_COUNT+3
#define BlueBack SYS_IMAGES_COUNT + OS_IMAGES_COUNT+4
#define BlueDiscuss SYS_IMAGES_COUNT + OS_IMAGES_COUNT+5
#define BlueExclamation SYS_IMAGES_COUNT + OS_IMAGES_COUNT+6
#define BlueHealth SYS_IMAGES_COUNT + OS_IMAGES_COUNT+7
#define BlueHeart SYS_IMAGES_COUNT + OS_IMAGES_COUNT+8
#define BlueInfo SYS_IMAGES_COUNT + OS_IMAGES_COUNT+9
#define BlueMail SYS_IMAGES_COUNT + OS_IMAGES_COUNT+10
#define BlueQuestion SYS_IMAGES_COUNT + OS_IMAGES_COUNT+11
#define BlueRss SYS_IMAGES_COUNT + OS_IMAGES_COUNT+12
#define BlueWarning SYS_IMAGES_COUNT + OS_IMAGES_COUNT+13
#define GreenOk SYS_IMAGES_COUNT + OS_IMAGES_COUNT+14
#define OrangeArrowDown SYS_IMAGES_COUNT + OS_IMAGES_COUNT+15
#define OrangeArrowLeft SYS_IMAGES_COUNT + OS_IMAGES_COUNT+16
#define OrangeArrowRight SYS_IMAGES_COUNT + OS_IMAGES_COUNT+17
#define OrangeArrowUp SYS_IMAGES_COUNT + OS_IMAGES_COUNT+18
#define OrangeBack SYS_IMAGES_COUNT + OS_IMAGES_COUNT+19
#define OrangeDocument SYS_IMAGES_COUNT + OS_IMAGES_COUNT+20
#define OrangeExclamation SYS_IMAGES_COUNT + OS_IMAGES_COUNT+21
#define OrangeHealth SYS_IMAGES_COUNT + OS_IMAGES_COUNT+22
#define OrangeHeart SYS_IMAGES_COUNT + OS_IMAGES_COUNT+23
#define OrangeMail SYS_IMAGES_COUNT + OS_IMAGES_COUNT+24
#define OrangeTarget SYS_IMAGES_COUNT + OS_IMAGES_COUNT+25
#define RedCancel SYS_IMAGES_COUNT + OS_IMAGES_COUNT+26

void FillAppSpecificImages(ImagesRepository* imagesrep);