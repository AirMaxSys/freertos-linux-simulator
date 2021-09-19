#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

#include "console.h"

#include "FreeRTOS_CLI.h"

#define CLI_PRIORITY    255
#define CLI_STACK_SIZE  2048

#define CLI_IP_BUF_LEN  512
#define CLI_OP_BUF_LEN  1024


//char cCliInputBuf[CLI_IP_BUF_LEN];
char cCliOutputBuf[CLI_OP_BUF_LEN];

// Test registed simple commands
extern void vRegisterSampleCLICommands( void );

static void vConsoleInterpreterTask(void *para);

static void vConsoleStart(void)
{
   xTaskCreate(vConsoleInterpreterTask,
               "CLI",
               CLI_STACK_SIZE,
               NULL,
               CLI_PRIORITY,
               NULL);
}

static void vConsoleInterpreterTask(void *para)
{
    char cCliInputBuf[50];
    const char * const pcCommandInput = cCliInputBuf;
    char * pcWriteBuffer = cCliOutputBuf;
    size_t xWriteBufferLen = CLI_OP_BUF_LEN;
    char ch = 0;
    int idx = 0;

    for (;;) {
        // output "#"
        putchar('#');
        fflush(stdout);
#if 0
        // get input command
        if (fgets(cCliInputBuf, CLI_IP_BUF_LEN, stdin) == NULL) {
            fprintf(stderr, "INPUT ERROR!\n");
            continue;
        }
#endif
        while (read(STDIN_FILENO, &ch, 1) != 0) {
            if (ch != '\n') {
                cCliInputBuf[idx] = ch;
                printf("%c\n", cCliInputBuf[idx]);
                idx++;
            } else {
                printf("DEBUG:%d\n", idx);
                for (int i = 0; i < 10; ++i)
                    putchar(cCliInputBuf[i]);
                puts("");
                cCliInputBuf[idx] = '\0';
                break;
            }
        }
        // printf("%s", cCliInputBuf);
        //FreeRTOS_CLIProcessCommand(pcCommandInput, pcWriteBuffer, xWriteBufferLen);
        // output write buffer
        //fprintf(stdout, "%s", pcWriteBuffer);
        idx = 0;
        while (1);
    }
}

void main_cli(void)
{
    vRegisterSampleCLICommands();
    vConsoleStart();

    vTaskStartScheduler();

    for (;;) {
    
    }
}

