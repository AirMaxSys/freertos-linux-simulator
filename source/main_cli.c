#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#include "FreeRTOS.h"
#include "task.h"

#include "FreeRTOS_CLI.h"
#include "console.h"

#define CLI_PRIORITY    100
#define CLI_STACK_SIZE  1024

#define CLI_IP_BUF_LEN  512
#define CLI_OP_BUF_LEN  2048

extern int errno;

// command string and output string buffer
static char cCliInputBuf[CLI_IP_BUF_LEN];
static char cCliOutputBuf[CLI_OP_BUF_LEN];

// Test registed simple commands
extern void vRegisterSampleCLICommands( void );

// Task handler
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
    char ch = 0;
    int idx = 0;
    ssize_t read_ret = 0;
    BaseType_t xReturn = pdTRUE;
    sigset_t prev_mask, all_mask;
    const char * pcCommandInput = cCliInputBuf;
    char * pcWriteBuffer = cCliOutputBuf;
    size_t xWriteBufferLen = CLI_OP_BUF_LEN;

    // Init signal set and mask all
    sigfillset(&all_mask);

    for (;;) {
        // output #
        putchar('#');
        fflush(stdout);

        // mask all signal
        if (pthread_sigmask(SIG_SETMASK, &all_mask, &prev_mask) < 0)
            perror("pthread_sigmask set");

#if 0
        if (fgets(cCliInputBuf, CLI_IP_BUF_LEN, stdin) == NULL) {
            fprintf(stderr, "INPUT ERROR!\n");
            continue;
        }
#endif
        // Get input command
        while ((read_ret = read(STDIN_FILENO, &ch, 1)) != 0) {
            if (ch != '\n' && ch != '\0') {
                cCliInputBuf[idx] = ch;
                //putchar(ch);
                //printf("idx->bufVal:%d->%d\n", idx, cCliInputBuf[idx]);
                idx++;
            } else if (ch == '\n') {
                cCliInputBuf[idx] = '\0';
                break;
            } else {
                continue;
            }
        }
        if (read_ret < 0) {
            fprintf(stderr, "ERRNO[%d]:%s\n", errno, strerror(errno));
            read_ret = 0;
        }

        // Restore previous masked signal set
        if (pthread_sigmask(SIG_SETMASK, &prev_mask, NULL) < 0)
            perror("pthread_sigmask restore");

        // printf("idx:%d str:%s\n", idx, cCliInputBuf);

        // Interpret input command unitll without False
        // Print interpretor handl result
        do {
            xReturn = FreeRTOS_CLIProcessCommand(pcCommandInput, pcWriteBuffer, xWriteBufferLen);
            fprintf(stdout, "%s", pcWriteBuffer);
        } while (xReturn != pdFALSE);

        // restore pointer and counter
        idx = 0;
        pcWriteBuffer = cCliOutputBuf;
        pcCommandInput = cCliInputBuf;
    }
}

void main_cli(void)
{
    // Register commands and start interpret commands task
    vRegisterSampleCLICommands();
    vConsoleStart();

    vTaskStartScheduler();

    while (1);
}

