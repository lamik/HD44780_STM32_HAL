/*
 * delays.c
 *
 *	The MIT License.
 *  Created on: 11.07.2018
 *      Author: Mateusz Salamon
 *      www.msalamon.pl
 *
 */
#include "main.h"
#include "stm32f4xx_hal.h"
#include "tim.h"

#include "delays.h"

void Delay_us(uint16_t us)
{
	htim3.Instance->CNT = 0;
	while(htim3.Instance->CNT <= us);
}
