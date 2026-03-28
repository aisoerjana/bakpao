#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>

void kapital (char *str)
{
	for (int i = 0; str[i] != '\0'; i++)
	{
		str[i] = toupper(str[i]);
	}
}

int main(void)
{
	// FORK A
	char message[100];
	pid_t pid_b, pid_c, pid_d, pid_e;
	
	printf("[Process A] Jendral Rusdi : ");
	scanf(" %[^\n]", message);
	
	printf("[Process A] Jendral Rusdi (PID %d) : Pesan diterima\n", getpid());
	
	// FORK B
	pid_b = fork();
	if (pid_b < 0)
	{
		printf("Fork B gagal");
		exit(EXIT_FAILURE);
	}
	
	if (pid_b == 0)
	{
		printf("[Process B] Mayor Reza Kecap (PID %d) : Pesan diterima\n", getpid());
		printf("[Process B] Mayor Reza Kecap : Saya lagi sibuk ngurusin sawit, kerjaan diberikan ke Fork D dan Fork E\n");
	
		// buat FORK D di dalam FORK B
		pid_d = fork();
		if (pid_d < 0)
		{
			printf("Fork D gagal");
			exit(EXIT_FAILURE);
		}
		
		if (pid_d == 0)
		{
			printf("[Process D] Sersan Ambatukam (PID %d) : Pesan diterima\n", getpid());
			printf("[Process D] Sersan Ambatukam : Siap laksanakan\n");
			exit(EXIT_SUCCESS);
		}	
		
		// B harus nunggu D selesai dulu sebelum FORK E dibuat
		waitpid (pid_d, NULL, 0);
		
		// buat FORK E di dalam FORK B
		pid_e = fork();
		if (pid_e < 0)
		{
			printf("Fork E gagal");
			exit(EXIT_FAILURE);
		}
		
		if (pid_e == 0)
		{
			printf("[Process E] Sersan Ambatublow (PID %d) : Pesan diterima\n", getpid());
			printf("[Process E] Sersan Ambatublow : Siap laksanakan\n");
			exit(EXIT_SUCCESS);
		}	
		// B nunggu E selesai
		waitpid(pid_e, NULL, 0);
		exit(EXIT_SUCCESS);
	}
	
	// A harus menunggu B dulu sebelum membuat FORK C
	waitpid(pid_b, NULL, 0);
	
	// FORK C dibuat
	pid_c = fork();
	if (pid_c < 0)
	{
		printf("Fork C gagal");
		exit(EXIT_FAILURE);
	}
	
	if (pid_c == 0)
	{
		printf("[Process C] Mayor Epstein (PID %d) : Pesan diterima\n", getpid());
		kapital(message);
		printf("[Process C] Mayor Epstein : Pesan telah diubah ke format rahasia\n");
		printf("%s\n", message);
		exit(EXIT_SUCCESS);
	}
	// A menunggu C selesai
	waitpid(pid_c, NULL, 0);
	
	printf("[Process A] Jendral Rusdi : Semua komandi selesai dilaksanakan\n");
	return EXIT_SUCCESS;
}
	
		
	
