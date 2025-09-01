#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define CHILDS 3
#define GRANDCHILDS 2

enum {FATHER, CHILD, GRANDCHILD};
enum {ABORT = -1, OK};

void sleepd(float time) {
    sleep(time*0.3);
}

void waitChilds() {
    pid_t pid;
    int status;
    while((pid = wait(&status)) > 0){
	printf("Fin del proceso %d\n", pid);
    }
}

void makeChilds(char* self, short childs, int* currIdx) {
    for (int i = 0; i < childs; i++) {
        pid_t pid = fork();
	*currIdx += childs;

	switch (pid) {
	    case ABORT:
		perror("Error en fork");
		exit(EXIT_FAILURE);
	    case OK:
		*self += 1;
		printf("Soy el proceso ");
		*self == CHILD? printf("hijo") : printf("nieto");
		printf(" %d y mi padre es %d\n", getpid(), getppid());
		sleepd(*currIdx);
		return;
	}
    }
}

int sum(int num) {
    if (!num) return 0;

    return num + sum(num-1);
}

int main() {
    char self = FATHER;
    int currIdx = 0;
    
    pid_t pid; 
    
    printf("-- CREACION --\n");
    printf("> Creando %d hijos:\n", CHILDS);
    makeChilds(&self, CHILDS, &currIdx);
    sleepd(sum(CHILDS+1));

    if (self == FATHER) printf("> Creando %d nietos por cada hijo:\n", GRANDCHILDS);
    if (self == CHILD) makeChilds(&self, GRANDCHILDS, &currIdx);
    sleepd(sum(GRANDCHILDS+1));

    sleepd((CHILDS+1)*(GRANDCHILDS+1));
    if (self == FATHER) printf("-- FINALIZACION --\n");
    
    if (self == CHILD) {
	waitChilds();
	printf("> Mis nietos terminaron...\n", GRANDCHILDS);
    }
    sleepd(sum(CHILDS+2));

    if (self == FATHER) {
	waitChilds();
	printf("> Mis hijos terminaron...\n", GRANDCHILDS);
    }

    wait(NULL);

    return EXIT_SUCCESS;
}
