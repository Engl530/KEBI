#ifndef ALGORITHM_H
#define ALGORITHM_H


typedef enum {NORMAL=0, CHARGING=1, FAULT=2}   device_status_t_m;
typedef enum {ALL_OFF=0, DIRECT_POLARITY=1, REVERS_POLARITY=2, BOOTSTRAP_CHARGING=3}   mosfet_status_t_m;
typedef enum {COIL_OFF=0, COIL_ON=1}   coil_status_t_m;
typedef enum {BTN_ON=0, BTN_OFF=1}   btn_status_t_m;

#endif // ALGORITHM_H