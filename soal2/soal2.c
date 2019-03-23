#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>

int main(int argc, char const *argv[])
{
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
    char directory[100];
    strcpy(directory,argv[1]);
    strcat(directory,"/hatiku");
  	char *file = "elen.ku";
    while(1)
    {
		d = opendir(directory);
		// printf("tes\n");
		if(d){
			while ((dir = readdir(d)) != NULL){
				if ( strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0 )
				{	
					if(strcmp(dir->d_name, file)==0)
					{
						chdir(directory);
						struct stat sb;
					    char outstr[200];

					    stat(file, &sb);

					    struct passwd *pw = getpwuid(sb.st_uid);
					    struct group  *gr = getgrgid(sb.st_gid);
					    
				    	chmod(file,0777);
					    // printf("%s %s\n", pw->pw_name, gr->gr_name);
					    if (strcmp(pw->pw_name,"www-data") == 0 && strcmp(gr->gr_name, "www-data") == 0)
					    {
					    	// printf("tes\n");
					    	stat(file, &sb);
					    	int statchmod = sb.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);
							// printf("chmod: %o\n", statchmod);
							snprintf(outstr, sizeof(outstr), "%o", statchmod);
							// strcpy(outstr,statchmod);
							// printf("%s\n",outstr );
					    	if(strcmp(outstr,"777")==0){
					    		// printf("tes\n");
					    		remove(file);
					    	}
					    }
					}
				}
			}
			closedir(d);

		}
		sleep(3);
	}
	exit(EXIT_SUCCESS);
}