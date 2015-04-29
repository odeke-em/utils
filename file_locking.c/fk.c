#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    char *path = "oxmd";
    if (argc >= 2)
        path = argv[1];

    int fd;
    
    while ((fd = open(path, O_RDONLY|O_WRONLY)) < 0 && errno == EAGAIN);

    if (fd < 0) {
        fprintf(stderr, "%s could not be opened\n", path);
        return -1;
    }

    struct flock f_lock = {
        .l_type     = F_WRLCK,
        .l_pid      = getpid(),
        .l_start    = 0,
        .l_len      = 0,
        .l_whence   = SEEK_SET,
    };

    int status;
    if ((status = fcntl(fd, F_SETLKW, &f_lock))) {
        fprintf(stderr, "fcntl:locking: %s got %d\n", path, status);
        goto done;
    }

    sleep(1000);

    f_lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &f_lock);

done:
    close(fd);
    return 0;
}
