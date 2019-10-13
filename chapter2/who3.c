/**
 * who3.c -- who with buffered reads
 *         - suppresses empty records
 *         - formats time nicely
 *         - buffered input
 */
#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>

#define SHOWHOST
#define NERCS 16
#define NULLUT ((struct utmp*)NULL)
#define UTSIZE (sizeof(struct utmp))

static char utmpbuf[NERCS*UTSIZE];      //storage buffer
static int num_recs;                    //num records
static int cur_rec;                     //next record index
static int utmp_fd;                     //read from

int utmp_open(char*);
struct utmp *utep_next(void);
int utmp_reload();
void utmp_close(void);

int utmp_open(char * filename) {
    utmp_fd = open(filename, O_RDONLY);
    cur_rec = num_recs = 0;
    return utmp_fd;
}

struct utmp *utmp_next(void) {
    struct utmp *record;
    if (utmp_fd == -1)
        return NULLUT;
    if (num_recs == cur_rec && utmp_reload() == 0)
        return NULLUT;
    
    record = (struct utmp *) &utmpbuf[cur_rec*UTSIZE];
    cur_rec++;
    return record;
}

int utmp_reload() {
    int read_size = read(utmp_fd, utmpbuf, NERCS*UTSIZE);
    num_recs = read_size / UTSIZE;
    cur_rec = 0;
    return num_recs;
}

void utmp_close() {
    if (utmp_fd != -1)
        close(utmp_fd);
}


void show_time(time_t time) {
    struct tm* stime = localtime(&time);
    printf("%d-%d-%d %d:%d ",stime->tm_year+1900,stime->tm_mon+1,stime->tm_mday,stime->tm_hour,stime->tm_min);
}

/**
 * show_info()
 * displays contents of the utmp struct in human readable form
 *  *note* these sizes should not be hardwired
 */ 
void show_info(struct utmp *utmpbuf) {
    if (utmpbuf->ut_type != USER_PROCESS)
        return;
    printf("%-8.8s", utmpbuf->ut_user);
    printf(" ");
    printf("%-8.8s", utmpbuf->ut_line);
    printf(" ");
    show_time(utmpbuf->ut_tv.tv_sec);
    printf(" ");
#ifdef SHOWHOST
    printf("(%s)", utmpbuf->ut_host);
#endif
    printf("\n");
}

int main(int argc, char const *argv[])
{
    struct utmp* current_record;         //read user info
    
    if (utmp_open(UTMP_FILE) == -1) {
        perror(UTMP_FILE);
        exit(1);
    }

    while ((current_record = utmp_next()) != NULLUT)
        show_info(current_record);
    utmp_close();
    return 0;
}
