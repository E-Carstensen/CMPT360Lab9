#include "findme.h"
#include <unistd.h>
#include <ctype.h>
#include <getopt.h>


int main(int argc, char *argv[]){
    char dir[32];
    char type = '\0';
    int depth = 0;
    char usr[32] = "\0";
    char filename[32] = "\0";

    int opt;
    while ((opt = getopt(argc, argv, "d:t:D:u:f:")) != -1) {
        switch (opt) {
            case 'd':
                snprintf(dir, sizeof(dir), "%s", optarg);
                break;
            case 't':
                type = optarg[0];
                break;
            case 'D':
                depth = atoi(optarg);
                break;
            case 'u':
                snprintf(usr, sizeof(usr), "%s", optarg);
                break;
            case 'f':
                snprintf(filename, sizeof(filename), "%s", optarg);
                break;
            default: 
                fprintf(stderr, "Usage: %s -d directory -t type -D depth -u user -f filename (f=regular file, d=directory, s=symbolic link, c=character device, b=block device\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    
    }

    printf("%s", filename);
    char result[128];

    findme(dir, type, depth, usr, filename, result);
    printf("%s", result);

    return 1;
}