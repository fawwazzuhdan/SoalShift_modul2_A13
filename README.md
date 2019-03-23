# SoalShift_modul2_A13
  - [Nomor 1](#nomor-1)
  - [Nomor 2](#nomor-2)
  - [Nomor 3](#nomor-3)
  - [Nomor 4](#nomor-4)
  - [Nomor 5](#nomor-5)


## Nomor 1
### Soal :
Elen mempunyai pekerjaan pada studio sebagai fotografer. Suatu hari ada seorang klien yang bernama Kusuma yang meminta untuk mengubah nama file yang memiliki ekstensi .png menjadi “[namafile]_grey.png”. Karena jumlah file yang diberikan Kusuma tidak manusiawi, maka Elen meminta bantuan kalian untuk membuat suatu program C yang dapat mengubah nama secara otomatis dan diletakkan pada direktori /home/[user]/modul2/gambar.
Catatan : Tidak boleh menggunakan crontab.

### Jawaban :
Menggunakan program [soal1.c](soal1/soal1.c). Isi dari program tersebut adalah 
```
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
                if (dir->d_type == 8) {
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
        	}
        	closedir(d);
    	}
    }
	exit(EXIT_SUCCESS);
}
```
Program dijalankan dengan tambahan argumen, argumen tersebut adalah direktori file yang akan menjadi source, jika direktori itu ada makan akan masuk ke dalam kondisi if. Kemudian mengecek semua file yang berada dalam direktori source, setelah itu mendapatkan extensi file. Setelah mendapatkan extensi file dicek apakah file tersebut memiliki extensi .png dan apakah file tersebut adalah file bukan merupakan suatu direktori jika iya maka nama file itu disimpan ke suatu variabel dan dihapus extensi filenya kemudian ditambahkan kata *_grey.png* diakhir nama file. setelah itu dipindahkan file tersebut menggunakan *exec()* yaitu mv, dengan sourcenya yaitu nama awal file dan destinasinya yaitu direktori *"/home/[user]/modul2/gambar"* diikuti dengan nama file yang baru yang tersimpan dalam variabel tadi.

## Nomor 2
### Soal :
Pada suatu hari Kusuma dicampakkan oleh Elen karena Elen dimenangkan oleh orang lain. Semua kenangan tentang Elen berada pada file bernama “elen.ku” pada direktori “hatiku”. Karena sedih berkepanjangan, tugas kalian sebagai teman Kusuma adalah membantunya untuk menghapus semua kenangan tentang Elen dengan membuat program C yang bisa mendeteksi owner dan group dan menghapus file “elen.ku” setiap 3 detik dengan syarat ketika owner dan grupnya menjadi “www-data”. Ternyata kamu memiliki kendala karena permission pada file “elen.ku”. Jadi, ubahlah permissionnya menjadi 777. Setelah kenangan tentang Elen terhapus, maka Kusuma bisa move on.
Catatan: Tidak boleh menggunakan crontab

### Jawaban :
Menggunakan program [soal2.c](soal2/soal2.c). Isi dari program tersebut adalah
```
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
					    if (strcmp(pw->pw_name,"www-data") == 0 && strcmp(gr->gr_name, "www-data") == 0)
					    {
					    	stat(file, &sb);
					    	int statchmod = sb.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);
							snprintf(outstr, sizeof(outstr), "%o", statchmod);
					    	if(strcmp(outstr,"777")==0){
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
```
Pertama-tama jalankan program dari *soal2.c* menggunakan parameter *fullpath* letak direktori *hatiku*. Setelah itu mengecek semua file di direktori *hatiku* jika menemukan file bernama *elen.ku* maka permissionnya diganti 777 setelah itu mendapatkan user owner dan groupnya. Jika user owner dan groupnya adalah "*www-data*" maka file tersebut akan dihapus.


## Nomor 3
### Soal :
Diberikan file campur2.zip. Di dalam file tersebut terdapat folder “campur2”. 
Buatlah program C yang dapat :

i)  mengekstrak file zip tersebut.

ii) menyimpan daftar file dari folder “campur2” yang memiliki ekstensi .txt ke dalam file daftar.txt. 
Catatan:  
- Gunakan fork dan exec.
- Gunakan minimal 3 proses yang diakhiri dengan exec.
- Gunakan pipe
- Pastikan file daftar.txt dapat diakses dari text editor

### Jawaban :
Menggunakan program [soal3.c](soal3/soal3.c). Isi dari program tersebut adalah
```
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
```
Program dijalankan dengan tambahan argumen, argumen tersebut adalah letak direktori file *campur2.zip*. Pertama-tama file *campur2.zip* di-unzip terlebih dahulu. Setelah di-unzip, di-list semua filenya dan di-pipe dengan grep agar bisa mendapatkan file yang memiliki extensi *.txt*. hasil outputan dari command *ls* dan *grep* disimpan dalam file *daftar.txt*


## Nomor 4
### Soal :
Dalam direktori /home/[user]/Documents/makanan terdapat file makan_enak.txt yang berisikan daftar makanan terkenal di Surabaya. Elen sedang melakukan diet dan seringkali tergiur untuk membaca isi makan_enak.txt karena ngidam makanan enak. Sebagai teman yang baik, Anda membantu Elen dengan membuat program C yang berjalan setiap 5 detik untuk memeriksa apakah file makan_enak.txt pernah dibuka setidaknya 30 detik yang lalu (rentang 0 - 30 detik).
Jika file itu pernah dibuka, program Anda akan membuat 1 file makan_sehat#.txt di direktori /home/[user]/Documents/makanan dengan '#' berisi bilangan bulat dari 1 sampai tak hingga untuk mengingatkan Elen agar berdiet.

Contoh:
File makan_enak.txt terakhir dibuka pada detik ke-1
Pada detik ke-10 terdapat file makan_sehat1.txt dan makan_sehat2.txt

Catatan: 
- dilarang menggunakan crontab
- Contoh nama file : makan_sehat1.txt, makan_sehat2.txt, dst

### Jawaban :


## Nomor 5
### Soal :
Kerjakan poin a dan b di bawah:

a.Buatlah program c untuk mencatat log setiap menit dari file log pada syslog ke /home/[user]/log/[dd:MM:yyyy-hh:mm]/log#.log
Ket:
- Per 30 menit membuat folder /[dd:MM:yyyy-hh:mm]
- Per menit memasukkan log#.log ke dalam folder tersebut
‘#’ : increment per menit. Mulai dari 1

b. Buatlah program c untuk menghentikan program di atas.
NB: Dilarang menggunakan crontab dan tidak memakai argumen ketika menjalankan program.

### Jawaban :



# Kendala
Minggu-minggu ini badan saya kurang sehat ditambah lagi ada deadline progress proyekan jadi banyak yang belum saya kerjakan.