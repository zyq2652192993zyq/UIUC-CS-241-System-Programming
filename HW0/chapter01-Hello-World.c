#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main() 
{
	char ch[] = "Hi! My name is Kylin Zhang\n";
	int len = strlen(ch);
	
	write(fileno(stdout), ch, len);
	
	return 0;
}
