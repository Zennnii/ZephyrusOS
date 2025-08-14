#include "shell.h"

char userIn[LINE_BUFFER_SIZE];

int argc = 0;
char *argv[MAX_ARGS];

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

void shell() {

    while (1) {
        print("> ");
        userIn[0] = '\0';
        buffer_flush();
        getline(userIn);
        execute_command(userIn);
    }
}

void getline(char *out) {
    int len = 0;

    while (1) {
    char ch = keyboard_getchar();

        if (ch == '\n') { // If enter
            out[len] = '\0'; // Null terminate the string
            return;
        }
        else if (ch == '\b') {
            if (len > 0) {
            len--;
            }
        }
        else {
            if (len < LINE_BUFFER_SIZE - 1) {
                out[len++] = ch;
            }
        }
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
    print("Unknown command\n");
}