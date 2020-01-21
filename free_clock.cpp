#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <chrono>
#include <algorithm> 
#include <vector>
#define ST_BASE (0x3F003000)
#define TIMER_OFFSET (4)
using namespace std;


int main(int argc, char *argv[]) {  
    auto time1 = chrono::high_resolution_clock::now();
    auto time2 = chrono::high_resolution_clock::now();
    
    vector<int> values(10000); 
    
    long long int t, prev, *timer; // 64 bit timer
    int fd;
    void *st_base; // byte ptr to simplify offset math
 
    // get access to system core memory
    if (-1 == (fd = open("/dev/mem", O_RDONLY))) {
        fprintf(stderr, "open() failed.\n");
        return 255;
    }
 
    // map a specific page into process's address space
    if (MAP_FAILED == (st_base = mmap(NULL, 4096,
                        PROT_READ, MAP_SHARED, fd, ST_BASE))) {
        fprintf(stderr, "mmap() failed.\n");
        return 254;
    }
 
    // set up pointer, based on mapped page
    timer = (long long int *)((char *)st_base + TIMER_OFFSET);
    
    // read initial timer
    double chronoTime;
    
    prev = *timer;
    time1 = chrono::high_resolution_clock::now();
      
    while (1==1) { // forever
        // read new timer
        t = *timer;

        if (t - prev >= 1000000){
            printf("Timer diff = %lld    \n", t - prev);
            prev = t;
            time2 = chrono::high_resolution_clock::now();
            auto interval = time2 - time1;
            chronoTime = chrono::duration_cast<chrono::microseconds>(interval).count();
            printf("Chronotime = %f    \n", chronoTime);
            fflush(stdout);
            time1 = time2;
            
            // Generate Random values 
            auto f = []() -> int { return rand() % 10000; }; 
            // Fill up the vector 
            generate(values.begin(), values.end(), f);
            sort(values.begin(), values.end()); 
            
            
        }       
    }
    // will never get here
    return 0;
}
