#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#include "FreeRTOS.h"
#include "task.h"

#include "console.h"

#include "FreeRTOS_CLI.h"

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
    ssize_t ret = 0;
    sigset_t prev_mask, all_mask;
    const char * const pcCommandInput = cCliInputBuf;
    char * pcWriteBuffer = cCliOutputBuf;
    size_t xWriteBufferLen = CLI_OP_BUF_LEN;

    // Init mask all signal set
    sigfillset(&all_mask);

    for (;;) {
        // output "#"
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
        while ((ret = read(STDIN_FILENO, &ch, 1)) != 0) {
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
        if (ret != 0) {
            fprintf(stderr, "ERRNO[%d]:%s\n", errno, strerror(errno));
            ret = 0;
        }

        // Restore previous masked signal set
        if (pthread_sigmask(SIG_SETMASK, &prev_mask, NULL) < 0)
            perror("pthread_sigmask restore");

        // printf("idx:%d str:%s\n", idx, cCliInputBuf);

        // Interpret input command by CLI interpreter
        FreeRTOS_CLIProcessCommand(pcCommandInput, pcWriteBuffer, xWriteBufferLen);
        // Print output of command
        fprintf(stdout, "%s", pcWriteBuffer);
        // restor pointer and counter
        idx = 0;
        pcWriteBuffer = cCliOutputBuf;
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

