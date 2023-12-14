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

        //char *rest = buffer;
        //char *token = strtok_r(rest, delim, &rest);

        #pragma omp parallel
        {
                int nthreads = omp_get_num_threads();
                printf("Total threads = %d\n",nthreads);
                int thread_id = omp_get_thread_num();
                printf("thread %d of %d checking in. \n", thread_id, nthreads);
        //}

                char *rest = buffer;
                char *token = strtok_r(rest, delim, &rest);
                #pragma omp for
                for (;token; token = strtok_r(rest, delim, &rest)) {
                        if ((int)strlen(token) >= 6) {
                                if (windex == 0) {
                                        strcpy(word[windex].wstring, token);
                                        word[windex].len = strlen(word[windex].wstring);
                                        word[windex].count++;
                                         windex++;
                                } else {
                                        int res = -1;
                                        for (int p = 0; p < windex; p++) {
                                        res = strcmp(word[p].wstring, token);
                                        if (res == 0) {
                                                word[p].count++;
                                                break;
                                                }
                                        }

                                        if (res != 0) {
                                                strcpy(word[windex].wstring, token);
                                                word[windex].len = strlen(word[windex].wstring);
                                                word[windex].count++;
                                                windex++;
                                        }
                                }
                        }
                }
        }
}

int main (int argc, char *argv[])
{
    //***TO DO***  Look at arguments, open file, divide by threads
    // Allocate and Initialize and storage structures

    char fileName[20] = {'\0'};

    int error;
    size_t start_offset = 0;
    size_t end_offset = -1;

    int m;

    if(argc < 2) {
                printf("Insufficient arguments on the command line\n");
        exit(-1);
    }

    //threadCount = atoi(argv[2]);
    strcpy(fileName, argv[1]);

        // Opening the File name to Read the Content
    fd = open(fileName, O_RDONLY);
    if(fd == -1)
    {
                printf("Failed to Open File\n");
                exit(-2);
    }

        // Calculating the start and end offset to detrmine the file size
    if (-1 == (start_offset = lseek(fd, 0, SEEK_SET))) {
        printf("lseek error\n");
                exit(-3);
    }
    if (-1 == (end_offset = lseek(fd, 0, SEEK_END))) {
        printf("lseek error\n");
                exit(-3);
    }

    fsize = end_offset - start_offset;
    printf("File Size: %ld byte\n", fsize);


    if (-1 == (start_offset = lseek(fd, 0, SEEK_SET))) {
        printf("lseek error\n");
    }

    // Initializing the Count for Each words to zero 
    for(int h=0; h<SIZE; h++)
    {
                word[h].count = 0;
    }

    //**************************************************************
    // DO NOT CHANGE THIS BLOCK
    //Time stamp start
    struct timespec startTime;
    struct timespec endTime;

    clock_gettime(CLOCK_REALTIME, &startTime);
    //**************************************************************
    // *** TO DO ***  start your thread processing
    //                wait for the threads to finish


    processWordsfromFile(fsize);

        //Sorting Words in Descending Order Based on Frequency Count
    struct wordlist temp_storage;
    for (int m = 0; m < windex; m++)
    {
       for (int n = m+1; n < windex; n++) {
            if (word[m].count < word[n].count)
                        {
                temp_storage = word[m];
                word[m] = word[n];
                word[n] = temp_storage;
            }
        }
    }

    // Printing top 10 word with string length of six or more charcaters
    printf("\n\n");
    printf("Word Frequency Count on %s with %d threads\n",
                                        argv[1], threadCount);
    printf("Printing top 10 words 6 characters or more.\n");

    for (int p = 0; p < 10; p++)
    {
        printf("Number %d is %s with a count of %d\n",
                                        p+1, word[p].wstring, word[p].count);
    }

    // ***TO DO *** Process TOP 10 and display

    //**************************************************************
    // DO NOT CHANGE THIS BLOCK
    //Clock output
    clock_gettime(CLOCK_REALTIME, &endTime);
    time_t sec = endTime.tv_sec - startTime.tv_sec;
    long n_sec = endTime.tv_nsec - startTime.tv_nsec;
    if (endTime.tv_nsec < startTime.tv_nsec)
    {
        --sec;
        n_sec = n_sec + 1000000000L;
    }

    printf("Total Time was %ld.%09ld seconds\n", sec, n_sec);
    //**************************************************************


    // ***TO DO *** cleanup
    close(fd); // closing File Descriptors
    free(buffer); // Releasing Buffer
        buffer = NULL;

        return 0;
}


