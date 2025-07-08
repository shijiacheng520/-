/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-06-27     Qiong       the first version
 */
#include <rtthread.h>
#include <board.h>
#include <rtdevice.h>
#include <my-application.h>

rt_uint16_t ir, als, ps;
//extern struct rt_mailbox mb;
//extern int mb_bool[128];
#define AP_IIC_DEV_NAME "i2c2"
#define AP_IIC_ADD 0x1e
static struct rt_i2c_bus_device *i2c_bus=RT_NULL;
uint16_t als11;
static struct rt_thread thread1;
static char thread1_stack[2048];

static rt_err_t ap3216_write(struct rt_i2c_bus_device *bus, rt_uint8_t reg, rt_uint8_t data)//根据通信协议编写对应的写操作
{
    struct rt_i2c_msg msgs;
    rt_uint16_t buf_size=1;
    rt_uint8_t send_buf[2];
    int x;
    send_buf[0]=reg;
    if(data!=RT_NULL)
    {
        send_buf[1]=data;
        buf_size=2;
    }

    msgs.addr=AP_IIC_ADD;
    msgs.flags=RT_I2C_WR;
    msgs.buf=send_buf;
    msgs.len=buf_size;
    //rt_thread_mdelay(500);
    x=rt_i2c_transfer(bus, &msgs, 1);
    if(x==1)
        return RT_EOK;
    else
    {
        rt_kprintf("%d",x);
        rt_kprintf("error when send %d %d", reg, data);
        return -RT_ERROR;
    }

}

static rt_err_t ap3216_read(struct rt_i2c_bus_device *bus,rt_uint8_t reg, rt_uint8_t *buf)//根据通信协议编写相应的读操作
{
    struct rt_i2c_msg msgs[2];
    msgs[0].addr=AP_IIC_ADD;
    msgs[0].buf=&reg;
    msgs[0].flags=RT_I2C_WR;
    msgs[0].len=1;
    msgs[1].addr=AP_IIC_ADD;
    msgs[1].buf=buf;
    msgs[1].flags=RT_I2C_RD;
    msgs[1].len=1;

    if(rt_i2c_transfer(bus, msgs, 2)==2)
    {
        return RT_EOK;
    }
    else
    {
        rt_kprintf("read failed regesiter %d", reg);
        return -RT_ERROR;
    }

}

static rt_err_t ap3216_init(void)
{

    rt_uint8_t temp;
    rt_uint8_t addr;
    rt_uint8_t data;

    addr=0x00;
    data=0x04;
    ap3216_write(i2c_bus,addr,data);//重启传感器
    rt_thread_mdelay(50);
    data=0x03;
    ap3216_write(i2c_bus,addr,data);//开启ALS、PS+IR
    rt_thread_mdelay(50);
    ap3216_read(i2c_bus,addr, &temp);
    rt_kprintf("temp=%d",temp);
    if(temp == 0x03)
        return RT_EOK;
    else{
        rt_kprintf("init failed\n");
        return -RT_ERROR;
    }
}

static void ap3216_entry(void *parameter)
{
    rt_uint8_t i;
    rt_uint8_t buf[6];
    rt_uint8_t mail[]={0};


    i2c_bus=(struct rt_i2c_bus_device *)rt_device_find(AP_IIC_DEV_NAME);
    if(i2c_bus==RT_NULL)
    {
        rt_kprintf("can't find s% device!\n",AP_IIC_DEV_NAME);
        return;
    }
    else
        rt_kprintf("find s%!\n",AP_IIC_DEV_NAME);
    ap3216_init();

    while(1){
        for(i = 0; i < 6; i++){
            ap3216_read(i2c_bus,0x0A+i, buf+i);
        }
        if(buf[0] & 0x80)
            ir=0;                        //IR_OF位为1,则数据无效
        else
            ir=((rt_uint16_t)buf[1]<<2)|(buf[0]&0x03);     //读取IR传感器的数据
        als=((rt_uint16_t)buf[3]<<8)|buf[2];                //读取ALS传感器的数据
        als11=((rt_uint16_t)buf[3]<<8)|buf[2];
        if(buf[4]&0x40)
            ps=0;                        //IR_OF位为1,则数据无效
        else
            ps=((rt_uint16_t)(buf[5] & 0x3F)<<4)|(buf[4]&0x0F); //读取PS传感器的数据
       //rt_kprintf("ALS:%d\r\n",als);//光线感应

        rt_thread_mdelay(500);
    }

}

int ap_thread_init(void)
{
    rt_thread_init(&thread1,"ap3216_thread", ap3216_entry, RT_NULL, &thread1_stack[0], sizeof(thread1_stack), 5, 10);
    rt_thread_startup(&thread1);
    return 0;
}
