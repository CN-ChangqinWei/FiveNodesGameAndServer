/*
 * led_loop.h
 *
 *  Created on: 2024��4��27��
 *      Author: DELL
 */

#ifndef CODE_LED_H_
#define CODE_LED_H_

#include "zf_common_headfile.h"
#include "zf_device_tld7002.h"
#include "zf_device_dot_matrix_screen.h"
enum Dir{
forward,
back,
left,
right,
};

enum Ele{
cross,//ʮ��
cirque,//Բ��
barrier,//�ϰ�
normal
};
extern int nCross;
extern enum Dir direction;
extern enum Ele element;

void led_loop(void);


#endif /* CODE_LED_H_ */
