#include "keyboard.h"

bool shift_pressed = false;
bool caps_lock = false;
char key_buffer[KEYBOARD_BUFFER_SIZE];
int key_buffer_head = 0;
int key_buffer_tail = 0;

void init_keyboard() {
    shift_pressed = false;
    caps_lock = false;
    buffer_flush();

    LOG_INFO("PS/2 Keyboard Driver Initialized Successfully");
}

inline bool buffer_is_empty() {
    return key_buffer_head == key_buffer_tail;
}

inline bool buffer_is_full() {
    return ((key_buffer_head + 1) % KEYBOARD_BUFFER_SIZE) == key_buffer_tail;
}

static void buffer_push(char c) {
    if (!buffer_is_full()) {
        key_buffer[key_buffer_head] = c;
        key_buffer_head = (key_buffer_head + 1) % KEYBOARD_BUFFER_SIZE;
    }
}

static char buffer_pop() {
    char c = 0;
    if (!buffer_is_empty()) {
        c = key_buffer[key_buffer_tail];
        key_buffer_tail = (key_buffer_tail + 1) % KEYBOARD_BUFFER_SIZE;
    }
    return c;
}

void buffer_flush(void) {
    key_buffer_head = 0;
    key_buffer_tail = 0;
}

char scancode_map[128] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=','\b','\t',
    'q','w','e','r','t','y','u','i','o','p','[',']','\n',0,'a','s',
    'd','f','g','h','j','k','l',';','\'','`',0,'\\','z','x','c','v',
    'b','n','m',',','.','/',0,'*',0,' ',0,0,0,0,0,0,0
};

char scancode_shift_map[128] = {
    0,27,'!','@','#','$','%','^','&','*','(',')','_','+','\b','\t',
    'Q','W','E','R','T','Y','U','I','O','P','{','}','\n',0,'A','S',
    'D','F','G','H','J','K','L',':','"','~',0,'|','Z','X','C','V',
    'B','N','M','<','>','?',0,'*',0,' ',0,0,0,0,0,0,0
};

void keyboard_isr_handler() {
    uint8_t scancode = inb(KBD_DATA_PORT);
    bool key_released = scancode & 0x80;
    uint8_t keycode = scancode & 0x7F;

    // Handle shift keys
    if (keycode == 0x2A || keycode == 0x36) {
        shift_pressed = !key_released;
        return;
    }

    // Handle caps lock
    if (!key_released && keycode == 0x3A) {
        caps_lock = !caps_lock;
        return;
    }

    if (!key_released) {
        char c = shift_pressed ^ caps_lock
                 ? scancode_shift_map[keycode]
                 : scancode_map[keycode];
        if (c) buffer_push(c); // Only push to buffer
    }
}

char keyboard_getchar() {
    while (buffer_is_empty()) {
        asm volatile ("hlt"); // Halt until next interrupt
    }
    return buffer_pop();
}
