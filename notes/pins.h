// Pin assignments
#define DPIN_RLED_SW 2 // both red led and switch.  pinMode OUTPUT = led, pinMode INPUT = switch
#define DPIN_GLED 5
#define DPIN_PWR 8
#define DPIN_DRV_MODE 9
#define DPIN_DRV_EN 10
#define APIN_TEMP 0
#define APIN_CHARGE 3
#define APIN_BAND_GAP 14
#define BUTTON_FILTER 7 // 00000111 (this is applied at read time)
#define BUTTON_JUST_OFF(state) (state==4)  // 100 = just off
#define BUTTON_JUST_ON(state) (state==1)   // 001 = just on
#define BUTTON_STILL_OFF(state) (state==0) // 000 = still off
// 010, 011, 101, 110, 111 = still on
#define BUTTON_STILL_ON(state) !(BUTTON_JUST_OFF(state) | BUTTON_JUST_ON(state) | BUTTON_STILL_OFF(state))
#define BUTTON_ON(state) (state & 3) // either of right most bits is on: 00000011 (not 100 or 000)
#define BUTTON_OFF(state) !BUTTON_ON(state)
#define CLICK_OFF 0
#define CLICK_ACTIVE 1
#define CLICK_WAIT 2
#define ACCEL_NONE   0 // nothing
#define ACCEL_TWIST  1 // return degrees - light axis remains constant
#define ACCEL_TURN   2 // return degrees - light axis changes
#define ACCEL_DROP   3 // return change of velocity - period of no acceleration before impact?
#define ACCEL_TAP    4 // return change of velocity - acceleration before impact
