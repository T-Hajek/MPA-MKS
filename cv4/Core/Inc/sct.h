/*
 * sct.h
 *
 *  Created on: Oct 9, 2024
 *      Author: 260592
 *
 *      This is a header of my SCT library
 *
 */

#ifndef INC_SCT_H_
#define INC_SCT_H_

#include "main.h"


void sct_init();
void sct_led(uint32_t value);
void sct_value(uint16_t value, uint16_t LED);

#endif /* INC_SCT_H_ */
