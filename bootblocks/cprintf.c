
#include <stdarg.h>

static unsigned char * __numout(long i, int base);

cputchar(ch)
int ch;
{
   if(ch == '\n') bios_putc('\r');
   return bios_putc(ch);
}

cprintf(char * fmt, ...)
{
   register int c;
   int count = 0;
   int type, base;
   long val;
   char * cp;
   char padch=' ';
   int  minsize = 0;
   va_list ap;

   va_start(ap, fmt);

   while(c=*fmt++)
   {
      if(c!='%')
      {
	 cputchar(c);
	 count++;
      }
      else
      {
	 type=1;
	 padch = *fmt;
	 minsize=0;
	 if(padch == '-') fmt++;

	 for(;;)
	 {
	    c=*fmt++;
	    if( c<'0' || c>'9' ) break;
	    minsize*=10; minsize+=c-'0';
	 }

	 while( c=='.' || (c>='0' && c<='9')) { c=*fmt++; }

	 if( padch == '-' ) minsize = -minsize;
	 else
	 if( padch == '0' ) padch='0'; else padch=' ';

	 if( c == 0 ) break;
	 if(c=='h')
	 {
	    c=*fmt++;
	    type = 0;
	 }
	 else if(c=='l')
	 {
	    c=*fmt++;
	    type = 2;
	 }

	 switch(c)
	 {
	    case 'x': base=16;   if(0) {
	    case 'o': base= 8; } if(0) {
	    case 'd': base=10; }
	       switch(type)
	       {
		  case 0: val=va_arg(ap, short); break; 
		  case 1: val=va_arg(ap, int);   break;
		  case 2: val=va_arg(ap, long);  break;
		  default:val=0; break;
	       }
	       cp = __numout(val,base);
	       if(0) {
	    case 's':
	          cp=va_arg(ap, char *);
	       }
	       if( minsize > 0 )
	       {
		  minsize -= strlen(cp);
		  while(minsize>0) { cputchar(padch); minsize--; }
		  minsize=0;
	       }
	       if( minsize < 0 ) minsize= -minsize-strlen(cp);
	       while(*cp)
		  cputchar(*cp++);
	       while(minsize>0) { cputchar(' '); minsize--; }
	       break;
	    case 'c':
	       cputchar(va_arg(ap, int));
	       break;
	    default:
	       cputchar(c);
	       break;
	 }
      }
   }
   va_end(ap);
   return count;
}

static char nstring[]="0123456789ABCDEF";

static unsigned char *
__numout(long i, int base)
{
   static unsigned char out[16];
   int n;
   int flg = 0;
   unsigned long val;

   if (i<0 && base==10)
   {
      flg = 1;
      i = -i;
   }
   val = i;

   for (n = 0; n < 15; n++)
      out[n] = ' ';
   out[15] = '\0';
   n = 14;
   do
   {
      out[n] = nstring[val % base];
      n--;
      val /= base;
   }
   while(val);
   if(flg) out[n--] = '-';
   return &out[n+1];
}