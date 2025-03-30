#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "pico/stdlib.h"
#include "defs.h"
#include "command.h"

static char *readline(char *buff, size_t size)
{
    size_t idx = 0;
    while (true)
    {
        uint8_t c;

        switch (c = getc(stdin))
        {
            // ignore CR
        case '\r':
            break;
            // EOL
        case '\n':
            return buff;
            // anything else goes in the buffer
        default:
            buff[idx++] = c;
            buff[idx] = '\0';
            if (idx >= size)
                return buff;
            break;
        }
    }
}

static void command_task(void *param)
{
    char buff[32];
    while (true)
    {
        printf("\n>> ");
        fflush(stdout);

        readline(buff, sizeof(buff));
        if (!strcmp(buff, "run"))
        {
            printf("-- running\n");
            valve.force(true);
        }
        else if (!strcmp(buff, "stop"))
        {
            printf("-- stopped\n");
            valve.force(false);
        }
        else
            printf("[%s]\n", buff);
    }
}

void command_initialize()
{

    xTaskCreate(command_task, "CMD_Task", 256, NULL, 1, NULL);
}
