#include <stdarg.h>
#include "../cpu/ports.h"
#include "screen.h"
#include "../libc/mem.h"
#include "types.h"


int get_cursor_offset();
void set_cursor_offset(int offset);
int print_char(char c, int col,int row,char attr);
int get_offset(int col,int row);
int get_offset_col(int offset);
int get_offset_row(int offset);
void kputchar(char c);

void kprint_at(char * message,int col, int row){
		int offset;
		if (col >=0 && row >=0)
				offset=get_offset(col,row);
		else{
				offset=get_cursor_offset();
				row=get_offset_row(offset);
				col=get_offset_col(offset);
		}

		int i=0;
		while(message[i]!=0){
				offset=print_char(message[i++],col,row,WHITE_ON_BLACK);
				row = get_offset_row(offset);
				col = get_offset_col(offset);
		}


}

void kprint(char* message){
		kprint_at(message,-1,-1);
}

void kprint_backspace(){
		int offset =get_cursor_offset()-2;
		int row=get_offset_row(offset);
		int col =get_offset_col(offset);
		print_char(0x08,col,row,WHITE_ON_BLACK);
}

void kputchar(char c){
		print_char(c,-1,-1,WHITE_ON_BLACK);
}

static void
kprintint(int xx, int base, int sign)
{
  static char digits[] = "0123456789abcdef";
  char buf[16];
  int i;
  uint x;

  if(sign && (sign = xx < 0))
    x = -xx;
  else
    x = xx;

  i = 0;
  do{
    buf[i++] = digits[x % base];
  }while((x /= base) != 0);

  if(sign)
    buf[i++] = '-';

  while(--i >= 0)
    kputchar(buf[i]);
}

void kprintf(const char* fmt,...){
	if(fmt==0){
		kprint("error for kprintf");
	}
	char c;
	char* pstr;
	va_list arg;
	va_start(arg,fmt);
	while(*fmt){
			c=*fmt;
			if(c=='%')
			{
			   fmt++;
			  switch(*fmt)
			  {
			  case 's':
			  pstr=va_arg(arg,char*);
			  while(*pstr){
			  	kputchar(*pstr++);
			  }
			  break;
			  case 'c':
			  c=va_arg(arg,int);
			  kputchar(c);
			  break;
			  case 'd':
			  kprintint(va_arg(arg,int),10,1);
			  break;
			  case 'p':
			  kprintint(va_arg(arg,int),10,0);
			  break;
			  case 'x':
			  kprintint(va_arg(arg,int),16,1);
			  break;
			}
			}else{
			kputchar(c);
			}
			fmt++;
	}
}


int print_char(char c,int col,int row,char attr){
		unsigned char * vidmem =(unsigned char*)VIDEO_ADDRESS;
		if(!attr) attr =WHITE_ON_BLACK;
		int offset;
		if(col >= 0 && row>=0) offset=get_offset(col,row);
		else offset=get_cursor_offset();
		if(c=='\n'){
				row=get_offset_row(offset);
				offset=get_offset(0,row+1);
		}else if(c == 0x08){
				vidmem[offset]=' ';
				vidmem[offset+1]=attr;
		}
		else{
				vidmem[offset]=c;
				vidmem[offset+1]=attr;
				offset+=2;
		}

		if(offset >= MAX_COLS*MAX_ROWS*2){
				int i;
				for( i=1;i<MAX_ROWS;i++)
						memory_copy((uint8_t*)get_offset(0,i)+VIDEO_ADDRESS,
									(uint8_t *)get_offset(0,i-1)+VIDEO_ADDRESS,
									MAX_COLS*2);
				char* last_line =(char *)get_offset(0,MAX_ROWS-1)+VIDEO_ADDRESS;
				for(i=0;i<MAX_COLS*2;i++) last_line[i]=0;
				offset-=2*MAX_COLS;
		}
		if(col>= MAX_COLS||row>=MAX_ROWS){
				vidmem[2*MAX_ROWS*MAX_COLS-2]='E';
				vidmem[2*MAX_ROWS*MAX_COLS-1]=RED_ON_WHITE;
				return get_offset(col,row);
		}
		set_cursor_offset(offset);
		return offset;

}



int get_cursor_offset(){
		port_byte_out(REG_SCREEN_CTRL,14);
		int offset=port_byte_in(REG_SCREEN_DATA)<<8;
		port_byte_out(REG_SCREEN_CTRL,15);
		offset+= port_byte_in(REG_SCREEN_DATA);
		return offset*2;
}

void set_cursor_offset(int offset){
		offset/=2;
		port_byte_out(REG_SCREEN_CTRL,14);
		port_byte_out(REG_SCREEN_DATA,(unsigned char)(offset >>8));
		port_byte_out(REG_SCREEN_CTRL,15);
		port_byte_out(REG_SCREEN_DATA,(unsigned char)(offset & 0xff));

}

void clear_screen(){
		int screen_size=MAX_COLS*MAX_ROWS;
		int i;
		char *screen= (char *)VIDEO_ADDRESS;
		for (i=0;i<screen_size;i++){
				screen[i*2]=' ';
				screen[i*2+1]=WHITE_ON_BLACK;
		}
		set_cursor_offset(get_offset(0,0));

}

int get_offset(int col, int row){return 2*(row*MAX_COLS+col);}
int get_offset_row(int offset){return offset/(2*MAX_COLS);}
int get_offset_col(int offset){return (offset-(get_offset_row(offset)*2*MAX_COLS))/2;}

