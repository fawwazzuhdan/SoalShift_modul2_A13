#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

int main(int argc, char const *argv[]) {
    pid_t pid, sid;
    pid = fork();
    if(pid < 0)
    {
      exit(EXIT_FAILURE);
    }
    if(pid > 0)
    {
      exit(EXIT_SUCCESS);
    }
    umask(0);
    sid = setsid();
    if(sid < 0)
    {
      exit(EXIT_FAILURE);
    }
    if((chdir("/")) < 0)
    {
      exit(EXIT_FAILURE);
    }
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    DIR *d;
    struct dirent *dir;
    while(1)
    {
    	d = opendir(argv[1]);
    	if (d)
    	{
        	while ((dir = readdir(d)) != NULL)
        	{
          		char *extensions = strrchr(dir->d_name, '.');
      			if (extensions != NULL)
      			{
	      			if(strcmp(extensions, ".png") == 0){
	                    chdir(argv[1]);
	                    char source_directory[100];
	                    strcpy(source_directory,dir->d_name);
	                    dir->d_name[strlen(dir->d_name)-4] = '\0';
	                    strcat(dir->d_name,"_grey.png");
	                    char destionation_directory[100] = "/home/fawwaz/modul2/gambar/";
	                    strcat(destionation_directory,dir->d_name);
	                    pid_t child_id;
    					child_id = fork();
	      				if (child_id != 0) {
	                       char *argv[4] = {"mv", source_directory, destionation_directory, NULL};
	                       execv("/bin/mv", argv);
	                   }
	      			}
      			}           
        	}
        	closedir(d);
    	}
    }
	exit(EXIT_SUCCESS);
}