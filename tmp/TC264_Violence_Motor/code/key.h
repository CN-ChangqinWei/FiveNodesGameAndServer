/*
 * key.h
 *
 *  Created on: 2023Äê12ÔÂ29ÈÕ
 *      Author: Roronoa zoro
 */

#ifndef CODE_KEY_H_
#define CODE_KEY_H_
#include "zf_common_headfile.h"

#define Key_ON          P33_12
#define Key_Down        P32_4
#define Key_Left        P33_11
#define Key_Right       P33_10
#define Key_Center      P33_13

#define _DEF_KEYS_1     (P33_6)
#define _DEF_KEYS_2     (P33_7)
#define _DEF_KEYS_3     (P33_8)
#define _DEF_KEYS_4     (P33_9)


#define BUTTON_1    0x08
#define BUTTON_2    0x01
#define BUTTON_3    0x04
#define BUTTON_4    0x10
#define BUTTON_5    0x20



typedef struct
{
        uint8   Trig;
        uint8   Cont;
        uint8   Long;
}Button;

extern Button Key;
extern int StartFlag;
extern uint8 Keys[4];
extern uint8 LastKeys[4];


void Key_read(Button* key);
void Key_process(void);



#endif /* CODE_KEY_H_ */
