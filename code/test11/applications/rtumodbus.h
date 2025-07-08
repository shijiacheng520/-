/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-06-29     Qiong       the first version
 */
#ifndef APPLICATIONS_RTUMODBUS_H_
#define APPLICATIONS_RTUMODBUS_H_
///#include "dismasterdata.h"
#include <stdint.h>
#include "stdlib.h"
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
//CRC16 校验位自检
uint16_t CRC16_Modbus(uint8_t *_pBuf, uint16_t _usLen);
///void MODS_AnalyzeApp(int x,uint8_t *rs485buf);
//合法性校验
////void legality_check(int n,uint8_t *rs485buf,uint8_t rec_length_data);


#endif /* APPLICATIONS_RTUMODBUS_H_ */
