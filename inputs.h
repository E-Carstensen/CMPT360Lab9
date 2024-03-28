

struct inputs {
    char *dir;
    int depth;
    char type;
    char *usr;
    char *targetFileName;
    char *path;
};


struct inputs* deepCopyInputs(struct inputs input){
    struct inputs *newInput = malloc(sizeof(struct inputs));
    newInput->depth = input.depth;
    newInput->dir = input.dir;
    newInput->path=input.path;
    newInput->targetFileName = input.targetFileName;
    newInput->type = input.type;
    newInput->usr = input.usr;
    return newInput;
}