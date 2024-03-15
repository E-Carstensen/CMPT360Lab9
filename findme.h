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
void scanDirectory(char *dir, int depth, char type, char *usr, char *targetFileName, char *path){

    if (strcmp(dir, "..") == 0 || depth < 0){
        return; // Reached max depth, end recursion
    }
   
    DIR *d;
    struct dirent* data;
    struct stat buf;

    // Attempt to open current directory
    if( (d=opendir(dir))==NULL){
	    printf("Could not open directory: %s\n", dir);
	    exit(EXIT_FAILURE);
    }

    // While there are files in the directory
    while( (data = readdir(d))!=NULL  ){

        char filename[123];
        strcpy(filename, path);
        //char slash = '/';
        //strncat(filename, &slash, 1);
        strcat(filename, data->d_name);


	    if(stat(filename,&buf) < 0){
	        printf("Cannot open file %s\n", filename);
            continue;
        }

        //printf("opening %s \n", data->d_name);
        if (S_ISDIR(buf.st_mode)){
            if (strcmp(data->d_name, "..") == 0 || strcmp(data->d_name, ".") == 0 || strcmp(data->d_name, ".git") == 0){continue;} // Skip Same dir and prev dir
            char newpath[128];
            strcpy(newpath, path);
            strcat(newpath, data->d_name);

            char slash[] = "/";
            strcat(newpath, slash);

            scanDirectory(newpath, --depth, type, usr, targetFileName, newpath);
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
        if (strcmp(targetFileName, "\0") != 0){
            printf("%s", targetFileName);
            if (strcmp(targetFileName, data->d_name) != 0){
                continue; // if file does not match
            }
        }

    printf("%s%s\n",path, data->d_name);

    }



}

void findme(char *dir, char type, int depth, char *usr, char *filename){

    struct stat buff;

    if(stat(dir,&buff) < 0){
	    printf("Cannot find file\n");
	    exit(EXIT_FAILURE);
    }


    if(!S_ISDIR(buff.st_mode)){
        printf("Not a directory: %d\n", S_ISDIR(buff.st_mode));
        exit(EXIT_FAILURE);
    }

    char path[128];
    char slash[] = "/";
    strcpy(path, dir);
    strcat(path, slash);
    

    scanDirectory(dir, depth, type, usr, filename, path);

}


