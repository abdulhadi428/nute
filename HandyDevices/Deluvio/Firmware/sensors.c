#include "sensors.h"
#include "common.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include "touch_qt_config.h"
#include "touch_api.h"
#include "beep.h"

#include "delay_util.h"

// ============================= Variables =====================================
extern qt_touch_lib_config_data_t qt_config_data;
// touch output - measurement data
extern qt_touch_lib_measure_data_t qt_measure_data;
// current time, needed for sensors measuring
uint16_t current_time_ms_touch = 0u;

#ifdef QTOUCH_STUDIO_MASKS
extern TOUCH_DATA_T SNS_array[2][2];
extern TOUCH_DATA_T SNSK_array[2][2];
#endif

struct Keys_t EKeys;

// =========================== Implementation ==================================
void QTouchInit(void) {
    SFIOR |= (1<<PUD);
    #ifdef QTOUCH_STUDIO_MASKS
    SNS_array[0][0]= 0x55;
    SNS_array[0][1]= 0x0;
    SNS_array[1][0]= 0x0;
    SNS_array[1][1]= 0x0;

    SNSK_array[0][0]= 0xaa;
    SNSK_array[0][1]= 0x0;
    SNSK_array[1][0]= 0x0;
    SNSK_array[1][1]= 0x0;
    #endif

    // Configure the Sensors as keys
    qt_enable_key( CHANNEL_0, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_1, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_2, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_3, AKS_GROUP_1, 10u, HYST_6_25 );

    //  Set the parameters like recalibration threshold, Max_On_Duration etc in this function by the user
    /*  This will be modified by the user to different values   */
    qt_config_data.qt_di              = DEF_QT_DI;
    qt_config_data.qt_neg_drift_rate  = DEF_QT_NEG_DRIFT_RATE;
    qt_config_data.qt_pos_drift_rate  = DEF_QT_POS_DRIFT_RATE;
    qt_config_data.qt_max_on_duration = DEF_QT_MAX_ON_DURATION;
    qt_config_data.qt_drift_hold_time = DEF_QT_DRIFT_HOLD_TIME;
    qt_config_data.qt_recal_threshold = DEF_QT_RECAL_THRESHOLD;
    qt_config_data.qt_pos_recal_delay = DEF_QT_POS_RECAL_DELAY;

    qt_init_sensing();  // initialise touch sensing

    /*  Address to pass address of user functions   */
    /*  This function is called after the library has made capacitive measurements,
    *   but before it has processed them. The user can use this hook to apply filter
    *   functions to the measured signal values.(Possibly to fix sensor layout faults)    */
    qt_filter_callback = 0;
}

FORCE_INLINE void Task_Sensors(void) {
    // Disable sensors when beeping, as beeping makes powerful interference
    if(EBeep.IsYelling) return;
    if(DelayElapsed(&EKeys.Timer, QT_MEASUREMENT_PERIOD)) {
        // update the current time
        current_time_ms_touch += QT_MEASUREMENT_PERIOD;
        SensorsMeasure();
    }
}

void SensorsMeasure(void) {
    QTouchActivityDetected();
    // ============= Check if key event triggered =========
    if(SensorIsTouched(3) && !EKeys.KeyDownPressed) {
        EKeys.KeyDownPressed = true;
        DelayReset(&EKeys.KeypressTimer);
        EKeys.KeypressDelay = KEY_REPEAT_TIMEOUT;
        EVENT_AnyKey();
        EVENT_KeyDown();
    }
    else if(!SensorIsTouched(3) && EKeys.KeyDownPressed) EKeys.KeyDownPressed = false;

    if(SensorIsTouched(2) && !EKeys.KeyUpPressed) {
        EKeys.KeyUpPressed = true;
        DelayReset(&EKeys.KeypressTimer);
        EKeys.KeypressDelay = KEY_REPEAT_TIMEOUT;
        EVENT_AnyKey();
        EVENT_KeyUp();
    }
    else if(!SensorIsTouched(2) && EKeys.KeyUpPressed) EKeys.KeyUpPressed = false;

    // For MENU key, no need to repeat it when holding long
    if(SensorIsTouched(1) && !EKeys.KeyMenuPressed) {
        EKeys.KeyMenuPressed = true;
        EVENT_AnyKey();
        EVENT_KeyMenu();
    }
    else if(!SensorIsTouched(1) && EKeys.KeyMenuPressed) EKeys.KeyMenuPressed = false;

    if(SensorIsTouched(0) && !EKeys.KeyAquaPressed) {
        EKeys.KeyAquaPressed = true;
        EVENT_AnyKey();
        EVENT_KeyAquaPressed();
    }
    else if(!SensorIsTouched(0) && EKeys.KeyAquaPressed) {
        EKeys.KeyAquaPressed = false;
        EVENT_KeyAquaDepressed();
    }

    // ============= Check if continuous keypress =========
    if(EKeys.KeyUpPressed) if(DelayElapsed(&EKeys.KeypressTimer, EKeys.KeypressDelay)) {
        EKeys.KeypressDelay = KEY_REPEAT_DELAY;
        EVENT_AnyKey();
        EVENT_KeyUp();
    }
    if(EKeys.KeyDownPressed) if(DelayElapsed(&EKeys.KeypressTimer, EKeys.KeypressDelay)) {
        EKeys.KeypressDelay = KEY_REPEAT_DELAY;
        EVENT_AnyKey();
        EVENT_KeyDown();
    }
}

bool QTouchActivityDetected(void) {
    // status flags to indicate the re-burst for library
    uint16_t status_flag = 0u;
    bool burst_flag = false;
    do {
        status_flag = qt_measure_sensors(current_time_ms_touch);
        burst_flag  = status_flag & QTLIB_BURST_AGAIN;
        wdt_reset();
    } while (burst_flag);
    return (qt_measure_data.qt_touch_status.sensor_states[0] != 0x00);
}

FORCE_INLINE bool SensorIsTouched(uint8_t ASensor) {
    return (qt_measure_data.qt_touch_status.sensor_states[0] & (1<<ASensor));
}
