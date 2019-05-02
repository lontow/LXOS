#include "string.h"
#include "x86.h"
#include "types.h"

void int_to_ascii(int n,char str[]){
		int i,sign;
		if((sign=n)<0) n=-n;
		i=0;
		do{
				str[i++] = n%10 +'0';
		}while ((n/=10)>0);
		str[i]='\0';

		reverse(str);
}

void hex_to_ascii(int n, char str[]){
		append(str,'0');
		append(str,'x');
		char zeros=0;
		int32_t tmp;
		int i;
		for(i=28;i>0;i-=4){
				tmp=(n>>i)&0xF;
				if(tmp==0 && zeros==0)continue;
				zeros =1;
				if(tmp >0xA) append(str,tmp -0xA +'a');
				else append(str,tmp+'0');
		}
		tmp =n& 0xF;
		if(tmp>= 0xA) append(str,tmp-0xA+'a');
		else append(str,tmp+'0');
}

void reverse(char s[]){
		int c,i,j;
		for(i=0,j=strlen(s)-1;i<j;i++,j--){
				c=s[i];
				s[i]=s[j];
				s[j]=c;
		}
}


int strlen(char s[]){
		int i=0;
		while(s[i]!='\0') ++i;
		return i;
}



void append(char s[],char n){
		int len=strlen(s);
		s[len]=n;
		s[len+1]='\0';
}


void backspace(char s[]){
		int len =strlen(s);
		s[len-1]='\0';
}


int strcmp(char s1[],char s2[]){
		int i;
		for (i=0;s1[i]==s2[i];i++){
			if(s1[i]=='\0') return 0;
		}
			return s1[i]-s2[i];
}

void*
memset(void *dst, int c, uint n)
{
  if ((int)dst%4 == 0 && n%4 == 0){
    c &= 0xFF;
    stosl(dst, (c<<24)|(c<<16)|(c<<8)|c, n/4);
  } else
    stosb(dst, c, n);
  return dst;
}
void*
memmove(void *dst, const void *src, uint n)
{
  const char *s;
  char *d;

  s = src;
  d = dst;
  if(s < d && s + n > d){
    s += n;
    d += n;
    while(n-- > 0)
      *--d = *--s;
  } else
    while(n-- > 0)
      *d++ = *s++;

  return dst;
}
