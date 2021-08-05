#include <stl.h>
#include <stdio.h>
#include <stdlib.h>

static const char* TAG = "STL";
static const char* inputfile = "assets/cube.ast";
void stl_recon(){
    printf("[%s] Open: %s\n", TAG, inputfile);
    FILE* stlfp;
    stlfp = fopen(inputfile,"r");
    if(stlfp){
        char buff;
        while ((buff = fgetc(stlfp)) != EOF) {
            printf("%c", buff);
        }
        fclose(stlfp);
    }
}

static int stl_ascii_count_vertices(){
    int counter = 0;
    return counter;
}

int stl_load_ascii(const char* filename, size_t* buffsize, GLfloat* data){
    fprintf(stderr, "[%s] ASCII STL not implemented yet...", TAG);
    return EXIT_FAILURE;

    FILE *stlfp;
    stlfp = fopen(inputfile,"r");
    if(!stlfp)
    return EXIT_FAILURE;

    fclose(stlfp);
    return EXIT_SUCCESS;
}