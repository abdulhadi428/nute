#include "sensors.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>

#include "delay_util.h"

// ============================= Variables =====================================
extern qt_touch_lib_config_data_t qt_config_data;
extern qt_touch_lib_measure_data_t qt_measure_data; // touch output - measurement data

/* current time, set by timer ISR */
static volatile uint16_t current_time_ms_touch = 0u;


#ifdef QTOUCH_STUDIO_MASKS
extern TOUCH_DATA_T SNS_array[2][2];
extern TOUCH_DATA_T SNSK_array[2][2];
#endif

struct Keys_t EKeys;

// =========================== Implementation ==================================
void Keys_t::Init(void) {
    MCUCR |= (1<<PUD);  // Disable all pull-ups
    #ifdef QTOUCH_STUDIO_MASKS
    SNS_array[0][0]= 0x5;
    SNS_array[0][1]= 0x0;
    SNS_array[1][0]= 0x41;
    SNS_array[1][1]= 0x0;

    SNSK_array[0][0]= 0xA;
    SNSK_array[0][1]= 0x0;
    SNSK_array[1][0]= 0x82;
    SNSK_array[1][1]= 0x0;
    #endif

    // Configure the Sensors as keys
    qt_enable_key( CHANNEL_0, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_1, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_2, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_3, AKS_GROUP_1, 5u, HYST_6_25 );

    //  Set the parameters like recalibration threshold, Max_On_Duration etc in this function by the user
    /*  This will be modified by the user to different values   */
    qt_config_data.qt_di              = DEF_QT_DI;
    qt_config_data.qt_neg_drift_rate  = DEF_QT_NEG_DRIFT_RATE;
    qt_config_data.qt_pos_drift_rate  = DEF_QT_POS_DRIFT_RATE;
    qt_config_data.qt_max_on_duration = DEF_QT_MAX_ON_DURATION;
    qt_config_data.qt_drift_hold_time = DEF_QT_DRIFT_HOLD_TIME;
    qt_config_data.qt_recal_threshold = DEF_QT_RECAL_THRESHOLD;
    qt_config_data.qt_pos_recal_delay = DEF_QT_POS_RECAL_DELAY;

    /*  Address to pass address of user functions   */
    /*  This function is called after the library has made capacitive measurements,
    *   but before it has processed them. The user can use this hook to apply filter
    *   functions to the measured signal values.(Possibly to fix sensor layout faults)    */
    qt_filter_callback = 0;

    qt_init_sensing();  // initialise touch sensing

    // Init variables
    Key[0].EventPress = &EVENT_KeyDown;
    Key[0].MayRepeat  = true;
    Key[1].EventPress = &EVENT_KeyUp;
    Key[1].MayRepeat  = true;
    Key[2].EventPress = &EVENT_KeyOnOff;
    Key[3].EventPress = &EVENT_KeyLit;
}

void Keys_t::Task(void) {
    if(!Delay.Elapsed(&Timer, QT_MEASUREMENT_PERIOD)) return;
    // Update the current time
    current_time_ms_touch += QT_MEASUREMENT_PERIOD;
    // Measure
    // Status flags to indicate the re-burst for library
    uint16_t status_flag = 0u;
    bool burst_flag = false;
    do {
        status_flag = qt_measure_sensors(current_time_ms_touch);
        burst_flag  = status_flag & QTLIB_BURST_AGAIN;
        wdt_reset();
    } while (burst_flag);

    // Check if key event triggered
    for (uint8_t i=0; i<4; i++) {
        if (IsTouched(i) && !Key[i].IsDown) {
            Key[i].IsDown = true;
            if (Key[i].MayRepeat) ATOMIC_BLOCK(ATOMIC_FORCEON) {
                Key[i].Delay = KEY_REPEAT_TIMEOUT;
                Delay.Reset(&Key[i].Timer);
            }
            if (Key[i].EventPress != 0) Key[i].EventPress();
        }
        else if (!IsTouched(i) && Key[i].IsDown) Key[i].IsDown = false;

        // Check if continuous keypress
        if (Key[i].MayRepeat) {
            if (Key[i].IsDown) if (Delay.Elapsed(&Key[i].Timer, Key[i].Delay)) {
                ATOMIC_BLOCK(ATOMIC_FORCEON) {
                    Key[i].Delay = KEY_REPEAT_DELAY;
                }
                if (Key[i].EventPress != 0) Key[i].EventPress();
            } // if is down && delay elapsed
        } // if may repeat
    } // for
}


