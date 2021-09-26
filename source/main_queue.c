#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "console.h"

#define QUEUE_TASK_STACK_SIZE  1024
#define SENDER_TASK_PRIORITY    2
#define RECV_TASK_PRIORITY      3

static QueueHandle_t xQueue;

void sender1_task(void *para)
{
    BaseType_t ret;
    int32_t send_val = (int32_t)para;
    const TickType_t block_time = pdMS_TO_TICKS(1000);
    TickType_t cur_time = xTaskGetTickCount();

    for (;;) {
        xTaskDelayUntil(&cur_time, block_time);
        ret = xQueueSendToBack(xQueue, &send_val, 0); 
        if (ret != pdPASS) {
            console_print("Queue send1 error\n!");
        }
    }
}

void sender2_task(void *para)
{
    BaseType_t ret;
    int32_t send_val = (int32_t)para;
    const TickType_t block_time = pdMS_TO_TICKS(2000);
    TickType_t cur_time = xTaskGetTickCount();

    for (;;) {
        xTaskDelayUntil(&cur_time, block_time);
        ret = xQueueSendToBack(xQueue, &send_val, 0);
        if (ret != pdPASS) {
            console_print("Queue send2 error!\n");
        }
    }
}

void recv_task(void *para)
{
    (void) para;
    int32_t recv_val;
    BaseType_t ret;
    TickType_t recv_max_wait_tick = pdMS_TO_TICKS(3000);

    for (;;) {
        if (uxQueueMessagesWaiting(xQueue) != 0) {
            console_print("Queue should be empty!\n");
        }
        ret = xQueueReceive(xQueue, &recv_val, recv_max_wait_tick);
        if (ret != pdPASS) {
            console_print("Queue receive error!\n");
        } else {
            console_print("Queue receive value is %d\n", recv_val);
        }
    }
}

void main_queue(void)
{
    xQueue = xQueueCreate(2, sizeof(int32_t));
    if (xQueue != NULL) {
        // careate two sender tasks one receiver and one queue
        xTaskCreate(sender1_task, "SENDER1", QUEUE_TASK_STACK_SIZE, (void *)1000, SENDER_TASK_PRIORITY, NULL);
        xTaskCreate(sender2_task, "SENDER2", QUEUE_TASK_STACK_SIZE, (void *)2000, SENDER_TASK_PRIORITY, NULL);
        xTaskCreate(recv_task, "RECV", QUEUE_TASK_STACK_SIZE, NULL, RECV_TASK_PRIORITY, NULL);
    } else {
        console_print("Queue Create error!\n");
    }
    
    // vTaskStartScheduler();
}

