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