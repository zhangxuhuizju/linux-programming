#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define BUFFERSIZE 4096
#define COPYMODE 0644

void err_msg(const char *s1, const char *s2) {
    fprintf(stderr, s1, s2);
    exit(1);
}

int main(int argc, char const *argv[])
{
    int in_fd, out_fd, n_chars;
    char buf[BUFFERSIZE+1];

    if (strcmp(argv[1], argv[2]) == 0) {
        printf("cp: '%s' and '%s' are the same file\n", argv[1], argv[2]);
        exit(1);
    }

    if (argc != 3) {
        fprintf(stderr, "usage: %s -source -destination\n", *argv);
        exit(1);
    }

    if ((in_fd = open(argv[1], O_RDONLY)) == -1) 
        err_msg("cannot open file %s\n", argv[1]);

    if ((out_fd = creat(argv[2], COPYMODE)) == -1) 
        err_msg("cannot creat file %s\n", argv[2]);
        

    while ((n_chars = read(in_fd, buf, BUFFERSIZE)) > 0) {
        if (write(out_fd, buf, n_chars) != n_chars) 
            err_msg("write error to %s\n", argv[2]);
    }

    if (n_chars < 0) 
        err_msg("read error from %s\n", argv[1]);

    if (close(in_fd) == -1 || close(out_fd) == -1)
        err_msg("%sError closing files", "");

    return 0;
}
