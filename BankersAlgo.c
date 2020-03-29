#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>

int no_of_resources,
    no_of_processes;
int *resources;
int **allocated;
int **maxr;
int **need;
int *safeSeq;
int ran = 0;

pthread_mutex_t lock;
pthread_cond_t cond;

bool safaseq();
void* p_code(void* );


void Need()
{
	need = (int **)malloc(no_of_processes * sizeof(*need));
        for(int i=0; i<no_of_processes; i++)
                need[i] = (int *)malloc(no_of_resources * sizeof(**need));

        for(int i=0; i<no_of_processes; i++)
                for(int j=0; j<no_of_resources; j++)
                        need[i][j] = maxr[i][j] - allocated[i][j];


	safeSeq = (int *)malloc(no_of_processes * sizeof(*safeSeq));
        for(int i=0; i<no_of_processes; i++) safeSeq[i] = -1;

        if(!safaseq()) {
                printf("\nThe processes leads the system to a unsafe state.\n\n");
                exit(-1);
        }

        printf("\n\nSafe Sequence Found : ");
        for(int i=0; i<no_of_processes; i++) {
                printf("%-3d", safeSeq[i]+1);
        }

        printf("\nProcesses are being executed...\n\n");
        sleep(1);
	
	
	pthread_t processes[no_of_processes];
        pthread_attr_t attr;
        pthread_attr_init(&attr);

	int processNumber[no_of_processes];
	for(int i=0; i<no_of_processes; i++) processNumber[i] = i;

        for(int i=0; i<no_of_processes; i++)
                pthread_create(&processes[i], &attr, p_code, (void *)(&processNumber[i]));

        for(int i=0; i<no_of_processes; i++)
                pthread_join(processes[i], NULL);

        printf("\nAll Processes Finished\n");	
	
	
        free(resources);
        for(int i=0; i<no_of_processes; i++) {
                free(allocated[i]);
                free(maxr[i]);
		free(need[i]);
        }
        free(allocated);
        free(maxr);
	free(need);
        free(safeSeq);
}

bool safaseq() {
	
        int r_temp[no_of_resources];
        for(int i=0; i<no_of_resources; i++) 
	r_temp[i] = resources[i];

        bool finished[no_of_processes];
        for(int i=0; i<no_of_processes; i++) 
	finished[i] = false;
	
        int nfinished=0;
        while(nfinished < no_of_processes) {
                bool flag = false;

                for(int i=0; i<no_of_processes; i++) {
                        if(!finished[i]) {
                                bool pos = true;

                                for(int j=0; j<no_of_resources; j++)
                                        if(need[i][j] > r_temp[j]) {
                                                pos = false;
                                                break;
                                        }

                                if(pos) {
                                        for(int j=0; j<no_of_resources; j++)
                                                r_temp[j] += allocated[i][j];
                                        safeSeq[nfinished] = i;
                                        finished[i] = true;
                                        ++nfinished;
                                        flag = true;
                                }
                        }
                }

                if(!flag) {
                        for(int k=0; k<no_of_processes; k++) safeSeq[k] = -1;
                        return false; 
                }
        }
        return true; 
}


void* p_code(void *a) {
        int n = *((int *) a);

	
        pthread_mutex_lock(&lock);

       
        while(n != safeSeq[ran])
                pthread_cond_wait(&cond, &lock);

	
        printf("\n--> Process P%d", n); //n+1
        printf("\n\tAllocated : ");
        for(int i=0; i<no_of_resources; i++)
                printf("%3d", allocated[n][i]);

        printf("\n\tNeeded    : ");
        for(int i=0; i<no_of_resources; i++)
                printf("%3d", need[n][i]);

        printf("\n\tAvailable : ");
        for(int i=0; i<no_of_resources; i++)
                printf("%3d", resources[i]);

        printf("\n"); 
	sleep(1);

        printf("\tThe Resource has been Allocated!");
        printf("\n"); 
	sleep(1);
        printf("\tThe Process is Running...");
        printf("\n"); sleep(rand()%3 + 2); 
        printf("\tProcess Completed...");
        printf("\n"); 
	sleep(1);
        printf("\tProcess Releasing Resource...");
        printf("\n"); 
	sleep(1);
        printf("\tResources Released!");

	for(int i=0; i<no_of_resources; i++)
                resources[i] += allocated[n][i];

        printf("\n\tCurrentl Available : ");
        for(int i=0; i<no_of_resources; i++)
                printf("%3d", resources[i]);
        printf("\n\n");

        sleep(1);

        ran++;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&lock);
	pthread_exit(NULL);
}


int main(int argc, char** argv) {
	srand(time(NULL));
	printf("\nThis program is based on Banker's Algorithm using multithreading and mutex");
        printf("\nEnter The Number of Processes");
        scanf("%d", &no_of_processes);

        printf("\nEnter the Number of Resources ");
        scanf("%d", &no_of_resources);

        resources = (int *)malloc(no_of_resources * sizeof(*resources));
        printf("\nResources that are available right now ");
        for(int i=0; i<no_of_resources; i++)
                scanf("%d", &resources[i]);

        allocated = (int **)malloc(no_of_processes * sizeof(*allocated));
        for(int i=0; i<no_of_processes; i++)
                allocated[i] = (int *)malloc(no_of_resources * sizeof(**allocated));

        maxr = (int **)malloc(no_of_processes * sizeof(*maxr));
        for(int i=0; i<no_of_processes; i++)
                maxr[i] = (int *)malloc(no_of_resources * sizeof(**maxr));

        // allocated
        printf("\n");
        for(int i=0; i<no_of_processes; i++) {
                printf("\nResource allocated to process %d  ", i+1);
                for(int j=0; j<no_of_resources; j++)
                        scanf("%d", &allocated[i][j]);
        }
        printf("\n");

	// maxrimum required resources
        for(int i=0; i<no_of_processes; i++) {
                printf("\nmaximum resource that is required by process %d ", i+1);
                for(int j=0; j<no_of_resources; j++)
                        scanf("%d", &maxr[i][j]);
        }
        printf("\n");
	Need();
	return 0;


}



