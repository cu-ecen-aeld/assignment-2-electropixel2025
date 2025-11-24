#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <syslog.h>


int main(int argc, char *argv[]) {

    int rc=-1;

    const char *writefile = argv[1];
    const char *writestr  = argv[2];

    openlog("My_logs", LOG_PID, LOG_USER);

    syslog(LOG_DEBUG, "Writing %s to %s", writestr,  writefile);

    if (argc != 3)
    {
        syslog(LOG_DEBUG, "Error : argument missing\n");
        return 1;
    }

    if (access(writefile, F_OK) == 0) {
         // file exists
         if (unlink(writefile) == -1) {
              syslog(LOG_ERR, "Error unlinking file: errno: %d - %s\n", errno, strerror( errno ) );
         }
    }

    // Create a file (overwrite it if it already exists)
    int fd=open(writefile, O_CREAT | O_RDWR, S_IRWXU|S_IRWXG|S_IRWXO);

    if (fd == -1)
    {
        syslog(LOG_ERR, "Error : can't generate the file  Error %d (%s) opening %s\r\n", errno,strerror(errno),writefile);
    }
    else {
           ssize_t written = write(fd, writestr, (strlen(writestr)));

           if (written == -1) {
              syslog(LOG_ERR, "Error writing text: errno: %d - %s\n", errno, strerror( errno ) );
           }
           else {
                  rc = 0;
                }
         }

    if (close(fd) == -1) {
        syslog(LOG_ERR, "Error closing file: errno: %d - %s\n", errno, strerror( errno ) );
        rc = -1;
    }

    closelog();

    return rc;
}



