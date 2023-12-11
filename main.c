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
	
	for(int i = 0; i < count; i++)
	{
		int flag = 0;
		for(int j=0;j<=windex;j++)
		{
			if(windex!=0 && !strcmp(arr[i], word[j].wstring))
			{
				word[j].count++;
				flag=1;
				break;
			}
		}
		if(flag==0)
			{
				windex++;
				strcpy(word[windex].wstring, arr[i]);
				word[windex].len = strlen(word[windex].wstring);
				word[windex].count++;
				
			}
	}
	

}

#if 0
// Thread Function for Processing and Storing Words
void * processWords(void *ptr)
{
        //printf("Enter processWords\n");
        int index = *((int *)ptr);

        off_t off;      // offset variable
        int ret;
        buffer = (char *) malloc(sizeof(char) * (chunkSize + 1));
        if(buffer == NULL)
        {
                printf("Memory Allocation Not Successful\n");
                exit(-6);
        }

        // Starting of The Critical Section
        pthread_mutex_lock(&lock);

        // If First Thread then it has to read chunkSize data from 
        // Starting so offset is set to 0. If Intermediate thread
        // then it has to start from the new offset which is 
        // calculated based on the Chunk Size and Thread Index to
        // adjust Reading from the Proper Location. For Last 
        // Thread, it has to read left over Chunk Size and offset 
        // is adjusted accordingly. pread() helps to read data of
        // count bytes from the given offset without worrying 
        // to change the File Descriptor and stores into buffer
        if(index == 0)
        {
                off = 0;
                ret = pread(fd, buffer, chunkSize, off);
                if(ret == -1)
                {
                        printf("Error in Reading\n");
                        exit(-7);
                }
        }
        else if(index != 0 && (index == (threadCount -1)))
        {
                off = 0 + (index * chunkSize) + 1;
                ret = pread(fd, buffer, remainChunkSize, off);
                if(ret == -1)
        {
                printf("Error in Reading\n");
                        exit(-7);
        }

        }
        else if(index != 0 && (index <  (threadCount -1)))
        {
                off = 0 + (index * chunkSize) + 1;
                ret = pread(fd, buffer, chunkSize, off);
        if(ret == -1)
        {
                printf("Error in Reading\n");
                        exit(-7);
        }
        }

        // Processing of the Words From the Buffer and storing into
        // the Data Structure. strtok_r() function helps in getting 
        // word by splitting based on deliminiter. If the len of token
        // is greater or equal to 6, I will store the word in structure.
        // Also I keep track of the frequency of the Word. If it is new
        // word, I will store into the structure otherwise, I will just
        // update the count
        int res;
        char *rest = buffer;
        //char *token;
        //char * token = strtok(buffer, delim);
        // loop through the string to extract all other tokens
        //while( token != NULL )
#if 0
        while ((token = strtok_r(rest, delim, &rest))) 
        {
                if((int)strlen(token) >= 6)
                {

                        if(windex==0)
                        {       
                                strcpy(word[windex].wstring, token);    
                                word[windex].len = strlen(word[windex].wstring);
                                word[windex].count++;
                                windex++;
                                
                        }
                        else
                        {
                                for(int p=0; p<windex; p++)
                                {
                                        res = strcmp(word[p].wstring, token);
                                        if(res == 0)
                                        {
                                                word[p].count++;
                                                break;
                                        }
                                }
                                
                                if(res != 0)
                                {
                                        strcpy(word[windex].wstring, token);    
                                        word[windex].len = strlen(word[windex].wstring);
                                        word[windex].count++;
                                        windex++;

                                }
                        }
                        //token = strtok(NULL, delim);
                }       
        }
#endif
        char *token = strtok_r(rest, delim, &rest);
        //#pragma omp for
        for (; token; token = strtok_r(rest, delim, &rest)) {
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


        // Ending of the Critical Section
        //pthread_mutex_unlock(&lock);
        //printf("Exit processWords\n");

        return NULL;

}
#endif

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

        // Calculating Chunk Size to Read by Each Thread
        // Adding 1 to Approximate the Bytes equally among all threads
        // except last thread
    #if 0
    chunkSize = (fsize/threadCount) + 1;

        // If only one thread then it has to read whole file of chunk size
        // If more than one thread then last thread has to read left
        // over chunk Size
        //
    if(threadCount == 1)
    {
                remainChunkSize = 0;
    }    
    else if(threadCount == 2) 
    {
                remainChunkSize = fsize - chunkSize;
    } 
    else if(threadCount == 4)
    {
                remainChunkSize = fsize - (3 * chunkSize); 
    }
    else if(threadCount == 8)
    {
                remainChunkSize = fsize - (7 * chunkSize);
    }
    else if(threadCount == 16)
    {
                remainChunkSize = fsize - (15 * chunkSize);
    }
    else if(threadCount == 32)
    {
                remainChunkSize = fsize - (31 * chunkSize);
    }
    else if(threadCount == 64)
    {
                remainChunkSize = fsize - (63 * chunkSize);
    }
    #endif

    if (-1 == (start_offset = lseek(fd, 0, SEEK_SET))) {
        printf("lseek error\n");
    }

    // Initializing the Count for Each words to zero 
    for(int h=0; h<SIZE; h++)
    {
                word[h].count = 0;
    }

    #if 0
        // Initializing Mutex Lock      
    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("Mutex init failed\n");
                exit(-4);
    }    
    #endif
    //**************************************************************
    // DO NOT CHANGE THIS BLOCK
    //Time stamp start
    struct timespec startTime;
    struct timespec endTime;

    clock_gettime(CLOCK_REALTIME, &startTime);
    //**************************************************************
    // *** TO DO ***  start your thread processing
    //                wait for the threads to finish



        // Creating Threads based on thread Count
    //printf("ThreadCount = %d\n", threadCount);
    //pthread_t threadId[threadCount];
    // pthread_t threadId;
    //int *test[threadCount];
    //int *test = malloc(sizeof(int));
#if 0
    for(m=0; m<threadCount; m++)
    {
                test[m] = malloc(sizeof(int));
                *test[m] = m;
        error = pthread_create(&(threadId[m]),NULL,&processWords, 
                                                                        (void *)test[m]);
                if(error != 0)
                {
                        printf("Error in Creating the threads\n");
                        exit(-5);
                }
    }

        // Wait for all threads to End 
    for(int k=0; k<threadCount; k++)
    {
                pthread_join(threadId[k], NULL);
    }
#endif

    /*    error = pthread_create(&(threadId[m]),NULL,&processWords, 
                                                                        (void *)test[m]);
                if(error != 0)
                {
                        printf("Error in Creating the threads\n");
                        exit(-5);
                }*/

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

        // pthread_mutex_destroy(&lock); // Destroying The Mutex
    close(fd); // closing File Descriptors
    free(buffer); // Releasing Buffer
        buffer = NULL;

        // Freeing the malloc memory allocated during thread creation
        // for passing the index
        //for(int l=0; l<threadCount; l++)
        //{
        //      free(test[l]);
        //}

        return 0;
}
