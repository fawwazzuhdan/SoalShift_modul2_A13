#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]) {
  pid_t child_id;
  int status;
  FILE *fp;

  child_id = fork();

  if (child_id == 0) {
    // this is child
    chdir(argv[1]);

    char *arg[3] = {"unzip", "campur2.zip", NULL};
    execv("/usr/bin/unzip", arg);
  } else {
    // this is parent

    // the parent waits for all the child processes
    while ((wait(&status)) > 0);

    char dir[100];
    strcpy(dir,argv[1]);
    strcat(dir,"campur2");
    // printf("%s\n",dir);

    pid_t pid_1,pid_2;
    int fd[2];

    char *arg1[] = {"ls", dir, NULL};
    char *arg2[] = {"grep", ".txt$", NULL};

    pipe(fd);
    fp = freopen("daftar.txt", "w+", stdout);
    
    pid_1 = fork();
    if (pid_1 == 0)
    {
    	dup2(fd[1], STDOUT_FILENO);
    	close(fd[0]);
    	execvp(arg1[0],arg1);
    	perror("exec");
    	return 1;
    }
    pid_2 = fork();
    if (pid_2 == 0)
    {
    	dup2(fd[0], STDIN_FILENO);
    	close(fd[1]);
    	execvp(arg2[0],arg2);
    	perror("exec");
    	return 1;
    }
    close(fd[0]);
    close(fd[1]);
    fclose(fp);
  }
}