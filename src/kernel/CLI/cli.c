#include "cli.h"

char userIn[LINE_BUFFER_SIZE];

int argc = 0;
char *argv[MAX_ARGS];

bool cli_running = true;

void parse_args(char *input) {
    argc = 0;

    // Skip leading spaces
    while (*input == ' ')
        input++;

    while (*input && argc < MAX_ARGS) {
        argv[argc++] = input; // Start of token

        // Find the end of this token
        while (*input && *input != ' ')
            input++;

        // If end of string, break
        if (!*input)
            break;

        // Null-terminate token
        *input = '\0';
        input++;

        // Skip spaces before next token
        while (*input == ' ')
            input++;
    }
}

void draw_prompt() {
    draw_string(fb, fb_width, 0, curLine, ">> ", colorWhite);
}

void cli() {
    cli_running = true;

    while (cli_running == true) {
        curX = 3;
        draw_prompt();
        userIn[0] = '\0';
        buffer_flush();
        getline(userIn);
        execute_command(userIn);
    }
}

void getline(char *out) {
    int len = 0;

    draw_cursor(fb, fb_width, 0xFFFFFF);

    while (1) {
    char ch = keyboard_getchar();

    erase_cursor(fb, fb_width, 0x637a87);

        if (ch == '\n') {
            out[len] = '\0';

            if (curLine + CHAR_H >= (int)fb_height - 28) {
                clear(0x637a87);
                // scroll_down(0x637a87);  // your background color
            } else {
                curLine += CHAR_H;
            }

            curX = 0;
            return;
        }

        else if (ch == '\b') {
            if (len > 0) {
            len--;
            backspaceFB(fb, fb_width, 0x637a87);
            }
        }
        else {
            if (len < LINE_BUFFER_SIZE - 1) {
                out[len++] = ch;
                draw_char(fb, fb_width, curX, curLine, ch, colorWhite);
            }
        }

        draw_cursor(fb, fb_width, 0xFFFFFF);
    }

}

typedef void (*cmd_func_t)(int argc, char *argv[]);

typedef struct {
    const char* name;
    cmd_func_t func;
} command_t;

command_t commands[] = {
    {"echo", echof},
    {"clear", clearf},
    {"ver", verf},
    {"help", helpf},
    {"shutdown", shutdownf},
    {"reboot", reboot8042},
    {"uptime", uptimef},
    {"panic", panicf},
    {"colors", colorsf},
    {"time", timef},
    {"beep", beepf},
    {"music", musicf},
    {"rd", rdf},
    {"regdump", regdumpf},
    {"exit", exitf},
    {NULL, NULL}
};

void execute_command(char *userIn) {
    parse_args(userIn);

    if (argc == 0)
        return;

    for (int i = 0; commands[i].name != NULL; i++) {
        if (strcmp(argv[0], commands[i].name) == 0) {
            commands[i].func(argc, argv);
            return;
        }
    }
    draw_string(fb, fb_width, 0, curLine, "Unknown command\n", colorWhite);
}