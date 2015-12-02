#include <stdio.h>

int main()
{
	int count = 0;
	char c;
	while ( (c=getchar()) != EOF) {
		if ( c ==  '"') {
				c = (count++ & 1)? '\'' : '`';
				putchar(c);
		}
		putchar(c);
	}
}

