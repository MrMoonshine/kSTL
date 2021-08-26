#include <stl.h>
#ifdef __cplusplus
extern "C"
{
#endif

static int vec3_to_int_2_10_10_10_rev(float *vector){
    int result = 0;
    result |= (int)(STL_GL_10_MAX * vector[2]) << 0;
    result |= (int)(STL_GL_10_MAX * vector[1]) << 10;
    result |= (int)(STL_GL_10_MAX * vector[0]) << 20;
    result |= (int)(0b11) << 30;
    return result;
}

static const char* TAG = "STL";
int stl_model_init(const char *filename, float* vertices, size_t* vertexSize, float* normals, size_t *normalSize){
    char header[STL_HEADER_SIZE] = "";
    unsigned int number_of_vertices;

    FILE* stlfp;
    stlfp = fopen(filename,"r");
    if(stlfp){
        fread(header, STL_HEADER_SIZE, 1, stlfp);
        fread(&number_of_vertices, sizeof(unsigned int), 1, stlfp);

        if(number_of_vertices < 1){
            fprintf(stderr, "[%s] Invalid STL Mesh!",TAG);
            fclose(stlfp);
            return EXIT_FAILURE;
        }
        //If the pointers are NULL then return size
        if(vertices == NULL || normals == NULL){
            *vertexSize = number_of_vertices * STL_VERTEX_FLOAT_COUNT * sizeof(float);
            *normalSize = number_of_vertices * 3 * 3 * sizeof(unsigned int);
            return EXIT_SUCCESS;
        }

        //memset(normals, 0, *normalSize);
        //memset(vertices, 0, *vertexSize);
        for(unsigned int v = 0; v < number_of_vertices; v++){
            float normal[3];
            fread(normal, sizeof(float) * 3, 1, stlfp);
            //int valuebuffer = vec3_to_int_2_10_10_10_rev(normal);
            for(int a = 0; a < 3; a++){
                memcpy((normals + v * 9 + a*3), normal, 3);
            }

            for(int b = 0; b < STL_VERTEX_FLOAT_COUNT; b++){
                unsigned int tentative_index = v * STL_VERTEX_FLOAT_COUNT + b;
                fread(
                    vertices + (tentative_index),
                    sizeof(float),
                    1,
                    stlfp
                );
            }
            //Remaining 2 bytes must be read to continue
            unsigned short attributes;
            fread(&attributes, sizeof(unsigned short), 1, stlfp);
        }

        printf("[%s] STL %s:\n\tIt has %d vertices\n",TAG, header, number_of_vertices);

        fclose(stlfp);
    }else{
        errno = EIO;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
#ifdef __cplusplus
} // extern "C"
#endif
