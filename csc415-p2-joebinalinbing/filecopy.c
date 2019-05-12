#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//You may also add more include directives as well.

// THIS VALUE CANNOT BE CHANGED.
// You should use this value when creating your buffer.
// And also to limit the amount of bytes each read CAN do.

#define BUFF_MAX 21
// ABOVE VALUE CANNOT BE CHANGED //



/*
 * Credits to http://www.techytalk.info/linux-system-programming-open-file-read-file-and-write-file/
 *
 * */

int main(int argc, char const *argv[]) {
    int input_fd, output_fd;    /* Input and output file descriptors */
    char buffer[BUFF_MAX];      /* Character buffer */
    char source[59];
    char destination[61];
    ssize_t bytes = 0, returnedIn, returnedOut;    /* Number of bytes returned by read() and write() */

    printf("Welcome to the File Copy Program by Joe Binalinbing!\n");
    printf("Enter the name of the file to copy from:");
    scanf("%s", &source);
    printf("Enter the name of the file to copy to:");
    scanf("%s", &destination);


    /* Create input file descriptor */
    input_fd = open(source, O_RDONLY);
    if (input_fd == -1) {
        perror("open");
        return 2;
    }

    /* Create output file descriptor */
    output_fd = open(destination, O_WRONLY | O_CREAT, 0644);
    if (output_fd == -1) {
        perror("open");
        return 3;
    }

    /* Copy process */
    while ((returnedIn = read(input_fd, &buffer, BUFF_MAX)) > 0) {
        returnedOut = write(output_fd, &buffer, (ssize_t) returnedIn);
        if (returnedOut != returnedIn) {
            /* Write error */
            perror("write");
            return 4;
        }
        bytes = bytes + returnedOut;
    }



    ssize_t bytes_formatted, bytes_written;
    bytes_formatted = sprintf(buffer, "bytes copied:  %i!\n", bytes);
    if (bytes_formatted < 0) {
        perror("sprintf has failed: ");
        return 5;
    }
    bytes_written = write(1, buffer, 59);
    if (bytes_written < 0) {
        perror("write has failed: ");
        return 5;
    }

    /* Close file descriptors */
    close(input_fd);
    close(output_fd);

    return (EXIT_SUCCESS);
}

