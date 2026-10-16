#ifndef LAB_CONFIG_H
#define LAB_CONFIG_H
/* Each separate project sets LAB_EXERCISE here: 31, 32, 33 or 4. */
#define LAB_EXERCISE 33
#define HAS_LEDS (LAB_EXERCISE != 33)
#define HAS_7SEG (LAB_EXERCISE == 32 || LAB_EXERCISE == 4)
#define HAS_IR (LAB_EXERCISE == 33 || LAB_EXERCISE == 4)
/* Example NEC codes. Confirm using Exercise 3.3 before demonstrating Exercise 4. */
#define REMOTE_KEY_1  0x0CU
#define REMOTE_KEY_2  0x18U
#define REMOTE_KEY_3  0x5EU
#define REMOTE_KEY_4  0x08U
#define REMOTE_KEY_OK 0x1CU
/* Command byte 0..255 is shown in hexadecimal 00..FF on two digits. */
#define LED_STEP_MS 150U
#endif
