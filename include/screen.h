#ifndef SCREEN_H
#define SCREEN_H
#define VIDEO_ADDRESS 0x800b8000
#define MAX_ROWS 80
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f
#define RED_ON_WHITE 0Xf4

#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

void clear_screen();
void kprint_at(char * message,int col,int row);
void kprint(char *message);
void kprintf(const char * fmt,...);
void kprint_backspace();

#endif