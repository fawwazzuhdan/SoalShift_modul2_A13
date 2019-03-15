#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

int int main()
{
	DIR *d;
    struct dirent *dir;
    char directory[100]="/home/fawwaz/modul2/hatiku";
	d = opendir(directory);
	return 0;
}