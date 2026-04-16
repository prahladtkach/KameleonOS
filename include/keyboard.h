#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "types.h"

#define KEYBOARD_DATA_PORT    0x60
#define KEYBOARD_STATUS_PORT  0x64

typedef void (*keyboard_callback_t)(char);

void keyboard_init(void);
void keyboard_set_callback(keyboard_callback_t cb);

#endif
