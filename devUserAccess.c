// Author: Emmanuel Odeke <odeke@ualberta.ca>

#include <pwd.h>
#include <stdio.h>
#include <errno.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

typedef enum {
    False=0, True=1
} Bool;

inline struct passwd *getPasswdInfo(const uid_t queryUID) {
    return getpwuid(queryUID);
}

int provideUserAccess(const char *path, const int queryAccessMode, const int permsToSet) {
    if (path == NULL) {
        fprintf(stderr, "\033[91mNULL paths not acceptable\033[00m") && fflush(stderr);
        return -1;
    }

    if (! (access(path, queryAccessMode|F_OK))) // That file also has to exist
        return 0;
    else {
        int err = errno;
        if (err == ENOENT) {
            fprintf(stderr, "[%s:%d] \033[91m%s does not exist\n\033[00m", __FILE__, __LINE__, path)\
            && fflush(stderr);
            return -1;
        }
        else if (err == EACCES) {
            int procEUID = geteuid();
            struct passwd *passwdInfo = getPasswdInfo(procEUID);
            fprintf(stderr, "No access to %s for %s\n", path,  passwdInfo->pw_name) && fflush(stderr);

            int origUID = getuid(), result=-1;
            // Let's now set the permissions

            Bool uidsNeedChange = origUID != procEUID, readyToSet=True;

            // Give access if setuid succeeds
            if (uidsNeedChange)
                readyToSet = ! setuid(procEUID) ? True: False;

            if (readyToSet) 
                result = chmod(path, permsToSet);

            fprintf(stderr, "\033[%s set permissions %d for %s for user: %s\033[00m\n",
                result ? "91mFailed to": "92mSuccessfully", permsToSet, path, passwdInfo->pw_name);

            // Regardless let's revert the original UID
            uidsNeedChange && setuid(origUID) && fprintf(
                stderr,  "Err [%s:%d]\033[91mWhile resetting originalUID from %d back to %d\033[00m\n",
                __FILE__, __LINE__, procEUID, origUID
            );
            return result;
        }
    }
}

int main(int argc, char *argv[]) {
    const char *path = "/dev/tty7";
    if (argc >= 2)
        path = argv[1];

#ifdef DEBUG
    printf("argc: %d path: %s\n", argc, path);
#endif // DEBUG

    provideUserAccess(path, R_OK, S_IRUSR);
    return 0;
}
