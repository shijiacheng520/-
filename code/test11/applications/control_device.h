/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-07-03     Qiong       the first version
 */
#ifndef APPLICATIONS_CONTROL_DEVICE_H_
#define APPLICATIONS_CONTROL_DEVICE_H_

int start_Motor_thread(void);
void Motor_Run(uint32_t dir, uint32_t num, uint32_t speed);
#endif /* APPLICATIONS_CONTROL_DEVICE_H_ */
