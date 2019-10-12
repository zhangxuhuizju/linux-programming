/**
 * who1.c -- a first version of the who program
 * open, read UTMP file, and show results
 */
#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define SHOWHOST

/**
 * show_info()
 * displays contents of the utmp struct in human readable form
 *  *note* these sizes should not be hardwired
 */ 
void show_info(struct utmp *utmpbuf) {
    printf("%-8.8s", utmpbuf->ut_user);
    printf(" ");
    printf("%-8.8s", utmpbuf->ut_line);
    printf(" ");
    printf("%10ld", utmpbuf->ut_tv.tv_sec);
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
