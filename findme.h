#include <dirent.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include <string.h>
#include <pwd.h>
#include "thread.h"
#include "inputs.h"


/**
 * Scans a directory and displays all files that pass given checks
 * If a directory is found will call itself recursively and decrement depth
 * 
*/
void* scanDirectory(void *rawInput){

    struct inputs* input = (struct inputs*) rawInput;


    if (strcmp(input->dir, "..") == 0 || input->depth < 0){
        return NULL; // Reached max depth, end recursion
    }
   
    DIR *d;
    struct dirent* data;
    struct stat buf;
    pthread_t nextThread;
    

    // Attempt to open current directory
    if( (d=opendir(input->dir))==NULL){
	    printf("Could not open directory: %s\n", input->dir);
	    exit(EXIT_FAILURE);
    }

    // While there are files in the directory
    while( (data = readdir(d))!=NULL  ){
        // Combine file name with current path

        char filename[123];

        strcpy(filename, input->path);
        strcat(filename, data->d_name);


	    if(stat(filename,&buf) < 0){
	        printf("Cannot open file %s\n", filename);
            continue;
        }

        //printf("opening %s \n", data->d_name);
        if (S_ISDIR(buf.st_mode)){
            if (data->d_name[0] == '.'){continue;} // Skip Same dir and prev dir

            printf("NEW DIR - %s", data->d_name);
            // Append directory to path for next run

            char newpath[128];
            strcpy(newpath, input->path);
            strcat(newpath, data->d_name);

            // Need a slash between directory and next file name
            char slash[] = "/";
            strcat(newpath, slash);

            struct inputs *copy = deepCopyInputs(*input);
            copy->depth--;

            pthread_create(&nextThread, NULL, scanDirectory, (void *) copy);

            pthread_join(nextThread, NULL);
            free(copy);            
            //scanDirectory(newpath, --depth, type, usr, targetFileName, newpath);
            continue;
        }
        // If user filter given
        if (input->usr[0] != '\0'){ // TODO: student server uses ldapsearch
            struct passwd *pws;
            pws = getpwuid(buf.st_uid); // Get username from uid
            if (strcmp(pws->pw_name, input->usr) > 0){continue;} // Check against filter
        }

        switch (input->type){
            case '\0': // If no input
                break;
            case 'f': // If current file not specified type, continue to next
                if (!S_ISREG(buf.st_mode)){continue;}
                break;
            case 'd':
                if (!S_ISDIR(buf.st_mode)){continue;}
                break;
            case 's':
                if(!S_ISLNK(buf.st_mode)){continue;}
                break;
            case 'c':
                if (!S_ISCHR(buf.st_mode)){continue;}
                break;
            case 'b':
                if (!S_ISBLK(buf.st_mode)){continue;}
                break;
        }


        // If specified filename not default
        if (strcmp(input->targetFileName, "\0") != 0){
            printf("%s", input->targetFileName);
            if (strcmp(input->targetFileName, data->d_name) != 0){
                continue; // if file does not match
            }
        }

        printf("%s%s\n",input->path, data->d_name);

    }



return NULL;

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

    char path[strlen(dir)+2];
    char slash[] = "/";
    strcpy(path, dir);
    strcat(path, slash);

    struct inputs *input = (struct inputs*) malloc(sizeof(struct inputs));
    input->depth = depth;
    input->dir = dir;
    input->path=path;
    input->targetFileName = filename;
    input->type = type;
    input->usr = usr;
    
    struct inputs *copy;
    copy = deepCopyInputs(*input);

    scanDirectory(copy);

}


