#include <error.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>


int to_utf8(unsigned short code_point, unsigned char utf8_bytes[])
{
    int len=0;
    int n=-1;
    if((((code_point)>>11)&-1)!=0){
        len = 3;
        utf8_bytes[0]=((14<<4)|((code_point)>>12));
        utf8_bytes[1]=((1<<7)|(((code_point)>>6)&(~((n)<<6))));
        utf8_bytes[2]=((1<<7)|((code_point)&(~((n)<<6))));
    }else if((((code_point)>>7)&-1)!=0){
        utf8_bytes[0]=((6<<5)|((code_point)>>6));
        utf8_bytes[1]=((1<<7)|((code_point)&(~((n)<<6))));
        len = 2;

    }else {
        utf8_bytes[0]=0|(code_point);
        len = 1;
    }

	return len;
}

void print_utf8(unsigned short cp, unsigned char seq[], int len)
{
    int i;
    printf("U+%04X   Hex:", cp);
    for (i = 0; i < len; i++)
       printf(" %02x", seq[i]);
    for ( ; i < 4; i++) printf("   ");
    seq[len] = '\0';
    printf("Character: %s  \n", seq);  
}

unsigned short convert_arg(const char *str,
                           unsigned short low, 
                           unsigned short high)
{
    char *end;
    long n = strtol(str, &end, 0);
    if (*end != '\0')
        error(1, 0, "Invalid code point '%s'", str);
    if (n < low || n > high)
        error(1, 0, "Code point %s not in range [%u, %u]", str, low, high);
    return n;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
        error(1, 0, "Missing argument. Please supply one or more unicode code"
                    " points in decimal or hex.");
    
    for (int i = 1; i < argc; i++) {
        unsigned short cp = convert_arg(argv[i], 0, USHRT_MAX);
        unsigned char seq[4];
        int len = to_utf8(cp, seq);
        print_utf8(cp, seq, len);  
    }
    return 0;
}
