#include <stl.h>
#ifdef __cplusplus
extern "C"
{
#endif

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

int stl_meta(struct MetaSTL *meta, float* vertices, size_t vertexSize){
    if(vertexSize < STL_VERTEX_FLOAT_COUNT * sizeof(float))
        return EXIT_FAILURE;

    meta->vertexCount = (unsigned int)(vertexSize/(STL_VERTEX_FLOAT_COUNT * sizeof(float)));
    //Set initial values
    meta->xmin = vertices[0];
    meta->xmax = vertices[0];
    meta->ymin = vertices[1];
    meta->ymax = vertices[1];
    meta->zmin = vertices[2];
    meta->zmax = vertices[2];

    for(unsigned int a = 0; a < vertexSize/sizeof(float); a++){
        switch (a % 3) {
        case STL_MOD_POS_X: {
            if(meta->xmin > vertices[a])
                meta->xmin = vertices[a];
            else if(meta->xmax < vertices[a])
                meta->xmax = vertices[a];
        } break;
        case STL_MOD_POS_Y: {
            if(meta->ymin > vertices[a])
                meta->ymin = vertices[a];
            else if(meta->ymax < vertices[a])
                meta->ymax = vertices[a];
        } break;
        case STL_MOD_POS_Z: {
            if(meta->zmin > vertices[a])
                meta->zmin = vertices[a];
            else if(meta->zmax < vertices[a])
                meta->zmax = vertices[a];
        } break;
        default:
            break;
        }
    }
    return EXIT_SUCCESS;
}

#ifdef __cplusplus
} // extern "C"
#endif
