#include <dirent.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include <string.h>
#include <pwd.h>


/**
 * Scans a directory and displays all files that pass given checks
 * If a directory is found will call itself recursively and decrement depth
 * 
*/
void scanDirectory(char *dir, int depth, char type, char *usr, char *filename, char *result){

    if (strcmp(dir, "..") == 0 || depth < 0){
        return; // Reached max depth, end recursion
    }


    printf("Scanning %s\n", dir);

   

    DIR *d;
    struct dirent* data;
    struct stat buf;


    if( (d=opendir(dir))==NULL){
	    printf("Could not open directory: %s\n", dir);
	    exit(EXIT_FAILURE);
    }

    while( (data = readdir(d))!=NULL  ){

	    if(stat(data->d_name,&buf) < 0){
	        printf("Cannot open file %s\n", data->d_name);
            continue;
        }

        if (S_ISDIR(buf.st_mode)){
            printf("FOUND A DIR?: %s", data->d_name);
            if (strcmp(dir, "..") == 0 || strcmp(dir, ".") == 0 || strcmp(dir, ".git") == 0){continue;} // Skip Same dir and prev dir
            scanDirectory(data->d_name, --depth, type, usr, filename, result);
            continue;
        }


        // If user filter given
        if (usr[0] != '\0'){
            struct passwd *pws;
            pws = getpwuid(buf.st_uid); // Get username from uid
            if (strcmp(pws->pw_name, usr) > 0){continue;} // Check against filter
        }

        switch (type){
            case '\0': // If no input
                break;
            case 'f': // If current file not specified type, continue to next
                if (!S_ISREG(buf.st_mode)){continue;}
            case 'd':
                if (!S_ISDIR(buf.st_mode)){continue;}
            case 's':
                if(!S_ISLNK(buf.st_mode)){continue;}
            case 'c':
                if (!S_ISCHR(buf.st_mode)){continue;}
            case 'b':
                if (!S_ISBLK(buf.st_mode)){continue;}
        }


        // If specified filename not default
        if (strcmp(filename, "\0") != 0){
            if (strcmp(filename, data->d_name) != 0){
                continue; // if file does not match
            }
        }

    printf("%s\n", data->d_name);

    }



}

void findme(char *dir, char type, int depth, char *usr, char *filename, char *result){

    struct stat buff;

    if(stat(dir,&buff) < 0){
	    printf("Cannot find file\n");
	    exit(EXIT_FAILURE);
    }


    if(!S_ISDIR(buff.st_mode)){
        printf("Not a directory: %d\n", S_ISDIR(buff.st_mode));
        exit(EXIT_FAILURE);
    }


    scanDirectory(dir, depth, type, usr, filename, result);

}


