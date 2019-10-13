/**
 * who2.c -- read utmp information and list info
 *         - suppresses empty records
 *         - formats time nicely
 */
#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define SHOWHOST

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
    struct utmp current_record;         //read user info
    int utmpfd;
    int reclen = sizeof(current_record);

    if ((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1) {
        perror(UTMP_FILE);
        exit(1);
    }
    while (read(utmpfd, &current_record, reclen) == reclen)
        show_info(&current_record);
    close(utmpfd);
    return 0;
}
