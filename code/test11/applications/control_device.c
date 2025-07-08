#include <rtthread.h>
#include <board.h>
#include <rtdevice.h>
#include <my-application.h>
#include <control_device.h>
#define THREAD_PRIORITY         25
#define THREAD_STACK_SIZE       512
#define THREAD_TIMESLICE        5

/*　使用静态线程时，线程的栈需要设置字节对齐　*/
static struct rt_thread Motor_thread;
static char Motor_stack[512];

extern int motor_enabled;
extern int motor_flag;

/* 遮光线程的入口函数 */
static void Motor_entry(void *parameter)
{
    if (motor_flag == 0)
    {
        motor_enabled = 1;Motor_Run(1, 11, 1);rt_thread_delete(&Motor_thread);
    }
    else
    {
        motor_enabled = 1;Motor_Run(0, 11, 1);rt_thread_delete(&Motor_thread);
    }
}

int start_Motor_thread(void)
{
    /* 静态创建线程 */

    /* 初始化线程 1，名称是 thread1，入口是 thread1_entry*/
    rt_thread_init(&Motor_thread,
                   "Motor_thread",
                   Motor_entry,
                   RT_NULL,
                   Motor_stack,
                   THREAD_STACK_SIZE,
                   THREAD_PRIORITY,
                   THREAD_TIMESLICE);
        /* 启动线程 */
        rt_thread_startup(&Motor_thread);

    return 0;
}

