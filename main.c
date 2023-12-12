#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <omp.h>

#define SIZE    50000   // Macro for Total Number of Words

// You may find this Useful
char * delim = "\"\'.“”‘’?:;-,—*($%)! \t\n\x0A\r";

// Structure to store the Word, it's frequency and Length
struct wordlist {
        char wstring[50];
        int count;
        int len;
};

struct wordlist word[SIZE]; // Array of Structure to Store Words
pthread_mutex_t lock;  // Mutex Lcok Variable
int fd;         // File Descriptor
int threadCount;        // Thread Count
long int fsize;         // File Size 
long int chunkSize;     // Chunk Size to Read by Each Thread
long int remainChunkSize; // Left Over Chunk Size to read by last thread
int windex = 0;
char *buffer;

void processWordsfromFile(long int fsize)
{
        //int index = *((int *)ptr);

        off_t off;      // offset variable
        int ret;
        int bytes_read;
        int size = fsize + 1;

        #if 0
        #pragma omp parallel
        {
                int nthreads = omp_get_num_threads();
                int thread_id = omp_get_thread_num();
                printf("thread %d of %d checking in. \n", thread_id, nthreads);
        }
        #endif

        buffer = (char *) malloc(sizeof(char) * (size));
        if(buffer == NULL)
        {
                printf("Memory Allocation Not Successful\n");
                exit(-6);
        }

        bytes_read = read(fd, buffer, size);

        char *rest = buffer;
        char *token = strtok_r(rest, delim, &rest);

		int count = 0;
		char *arr[SIZE];
        for (; token; token = strtok_r(rest, delim, &rest)) {
                if ((int)strlen(token) >= 6) {
                // if (windex == 0) {
                //         strcpy(word[windex].wstring, token);
                //         word[windex].len = strlen(word[windex].wstring);
                //         word[windex].count++;
                //         windex++;
                // } else {
                //         int res = -1;
                //         for (int p = 0; p < windex; p++) {
                //                 res = strcmp(word[p].wstring, token);
                //                 if (res == 0) {
                //                 word[p].count++;
                //                 break;
                //         }
                //     }

                //         if (res != 0) {
                //                 strcpy(word[windex].wstring, token);
                //                 word[windex].len = strlen(word[windex].wstring);
                //                 word[windex].count++;
                //                 windex++;
                //          }
                //     }
				arr[count] = token;
				count++;
               }
        }

		
	#pragma omp parallel for
	for(int i = 0; i < count; i++)
	{
		for(int j=0;j<=windex;j++)
		{
                        printf("i = %d\n",i);
		}
	}
	

}

int main (int argc, char *argv[])
{
    #pragma omp parallel
    {
        printf("Hello from process: %d\n", omp_get_thread_num());
    }        
    return 0;
}
