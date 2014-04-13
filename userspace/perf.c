#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef struct __attribute__ ((__packed__)) { 
  unsigned int overhead;
  unsigned int start;
} stats;

static inline void init_perfcounters (int32_t do_reset, int32_t enable_divider)
{
  // in general enable all counters (including cycle counter)
  int32_t value = 1;

  // peform reset:  
  if (do_reset)
  {
    value |= 2;     // reset all counters to zero.
    value |= 4;     // reset cycle counter to zero.
  } 

  if (enable_divider)
    value |= 8;     // enable "by 64" divider for CCNT.

  value |= 16;

  // program the performance-counter control-register:
  asm volatile ("MCR p15, 0, %0, c9, c12, 0\t\n" :: "r"(value));  

  // enable all counters:  
  asm volatile ("MCR p15, 0, %0, c9, c12, 1\t\n" :: "r"(0x8000000f));  

  // clear overflows:
  asm volatile ("MCR p15, 0, %0, c9, c12, 3\t\n" :: "r"(0x8000000f));
}

static inline unsigned int get_cyclecount (void)
{
  unsigned int value;
  // Read CCNT Register
  asm volatile ("MRC p15, 0, %0, c9, c13, 0\t\n": "=r"(value));  
  return value;
}

int usage(char *arg)
{
  printf("%s: needs one argument\n", arg);
  printf(" start\n");
  printf(" stop\n");
  printf(" reset\n");
  printf(" dump\n");
  printf(" cycles\n");
  return 0;
}

int main (int argc, char **argv) 
{
  char str[32];
  stats s;
  int fd;

  if (argc != 2) return usage(argv[0]);
  
  fd = open ("/tmp/testfile", O_RDWR | O_CREAT);
  read (fd, &s, sizeof(stats));


  if (strcmp(argv[1], "reset") == 0) 
  {
    init_perfcounters (1, 0);

    unsigned int overhead = get_cyclecount();
    overhead = get_cyclecount() - overhead;  

    s.overhead = overhead;
    s.start = 0;
  }

  if (strcmp(argv[1], "start") == 0) 
  {
    unsigned int t = get_cyclecount();

    s.start = t;
  }

  if (strcmp(argv[1], "stop") == 0) 
  {
    unsigned int t = get_cyclecount();
    unsigned int start, overhead;

    start = s.start;
    overhead = s.overhead;

    t = t - start;

    printf ("took exactly %d cycles (including function call) \n", t - overhead);
  }

  if (strcmp(argv[1], "dump") == 0) 
  {
    printf("overhead %u\n", s.overhead);
    printf("start %u\n", s.start);
  }

  if (strcmp(argv[1], "cycles") == 0) 
  {
    unsigned int t = get_cyclecount();

    printf ("cycles %u \n", t);
  }

  if (strcmp(argv[1], "test") == 0) 
  {
    init_perfcounters (1, 0);

    unsigned int overhead = get_cyclecount();
    overhead = get_cyclecount() - overhead;  

    unsigned int t = get_cyclecount();

    int i;
    for (i = 0; i < 1000000; ) i = i + 1;

    t = get_cyclecount() - t - overhead;
    printf ("total cycles %u, cycles per iteration %2.2f \n", t, t/1000000.0);
  }

  lseek (fd, 0, SEEK_SET);
  write (fd, &s, sizeof(stats));
  close(fd);

  return 0;
}
