#include "bmcli.h"

bool bare_cli_running;

void bare_cli() {
    clear(0x637a87);
    cli_running = false;
    bare_cli_running = true;

    while (bare_cli_running) {
        draw_string(fb, fb_width, 0, curLine, "-> ", colorWhite);
        userIn[0] = '\0';
        buffer_flush();
        getline(userIn);
        execute_commandb(userIn);
    }
}

typedef void (*cmd_func_t)(void);

typedef struct {
    const char* name;
    cmd_func_t func;
} command_t;

command_t commandsb[] = {
    {"load-main-cli", load_main_cli},
    {"shutdown", shutdownf},
    {"reboot", reboot8042},
    {"help", helpb},
    {NULL, NULL}
};

void execute_commandb(char *userIn) {

    for (int i = 0; commandsb[i].name != NULL; i++) {
        if (strcmp(userIn, commandsb[i].name) == 0) {
            commandsb[i].func();
            return;
        }
    }
    draw_string(fb, fb_width, 0, curLine, "Unknown command\n", colorWhite);
}