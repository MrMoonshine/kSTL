#include <stdint.h>
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

static int vec3_to_int_2_10_10_10_rev(vec3 vector){
    int result = 0;
    result |= (int)(STL_GL_10_MAX * vector[2]) << 0;
    result |= (int)(STL_GL_10_MAX * vector[1]) << 10;
    result |= (int)(STL_GL_10_MAX * vector[0]) << 20;
    result |= (int)(0b11) << 30;
    return result;
}

int stl_model_init(struct MetaSTL* meta, const char *filename){
    meta->success = false; 
    char header[STL_HEADER_SIZE] = "";
    uint32_t number_of_vertices;
    float *vertex_buffer;
    int *normals_buffer;
    
    FILE* stlfp;
    stlfp = fopen(filename,"r");
    if(stlfp){
        fread(header, STL_HEADER_SIZE, 1, stlfp);
        fread(&number_of_vertices, sizeof(uint32_t), 1, stlfp);

        if(number_of_vertices < 1){
            fprintf(stderr, "[%s] Invalid STL Mesh!",TAG);
            fclose(stlfp);
            return EXIT_FAILURE;
        }

        //3* 3-Dimensional Points => 9 values per vertex 
        vertex_buffer = (float*)malloc(number_of_vertices * STL_VERTEX_FLOAT_COUNT * sizeof(float));
        normals_buffer = (int*)malloc(number_of_vertices * 3 * sizeof(int));
        if(vertex_buffer == NULL || normals_buffer == NULL){
            fprintf(stderr, "[%s] Failed to allocate Memory!",TAG);
            fclose(stlfp);
            return EXIT_FAILURE;
        }

        for(int v = 0; v < number_of_vertices; v++){
            vec3 normal;
            fread(normal, sizeof(float) * 3, 1, stlfp);
            int valuebuffer = vec3_to_int_2_10_10_10_rev(normal);
            for(int a = 0; a < 3; a++)
            normals_buffer[v*3 + a] = valuebuffer;      
            
            for(int b = 0; b < STL_VERTEX_FLOAT_COUNT; b++){
                uint32_t tentative_index = v * STL_VERTEX_FLOAT_COUNT + b; 
                fread(
                    vertex_buffer + (tentative_index),
                    sizeof(float),
                    1,
                    stlfp
                );
            }
            //Remaining 2 bytes must be read to continue
            uint16_t attributes;
            fread(&attributes, sizeof(uint16_t), 1, stlfp);
            if(attributes != 0){
                free(vertex_buffer);
                free(normals_buffer);
                fprintf(stderr, "[%s] This software can't handle Attributes!",TAG);
                fclose(stlfp);
                return EXIT_FAILURE;
            }
        }

        printf("[%s] STL %s:\n\tIt has %d vertices\n",TAG, header, number_of_vertices);

        glGenBuffers(1, &meta->vertexbuffer);
	    glBindBuffer(GL_ARRAY_BUFFER, meta->vertexbuffer);
	    glBufferData(
            GL_ARRAY_BUFFER, 
            sizeof(float) * STL_VERTEX_FLOAT_COUNT * number_of_vertices,
            vertex_buffer,
            GL_STATIC_DRAW
        );

        glGenBuffers(1, &meta->normalbuffer);
	    glBindBuffer(GL_ARRAY_BUFFER, meta->normalbuffer);
	    glBufferData(
            GL_ARRAY_BUFFER, 
            number_of_vertices * 3 * sizeof(int),
            normals_buffer,
            GL_STATIC_DRAW
        );

        fclose(stlfp);
        free(vertex_buffer);
        free(normals_buffer);
    }else{
        errno = EIO;
        return EXIT_FAILURE;
    }

    meta->vertices = number_of_vertices;
    meta->success = true; 
    return EXIT_SUCCESS;
}


void stl_model_draw(struct MetaSTL* meta){
    /*---------------------------------*/
    /*       Render Vertices           */
    /*---------------------------------*/
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, meta->vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
    //Handle Normals
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, meta->normalbuffer);
    glVertexAttribPointer(
        1,                                // attribute
        4,                                // size
        GL_INT_2_10_10_10_REV,            // type
        GL_TRUE ,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );
	// Draw the triangles!
	glDrawArrays(GL_TRIANGLES, 0, meta->vertices * 3); // 3 indices starting at 0 -> 1 triangle

	glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void stl_model_free(struct MetaSTL* meta){
    if(meta->success){
        glDeleteBuffers(1, &meta->vertexbuffer);
        glDeleteBuffers(1, &meta->normalbuffer);
        meta->success = false;
    }
}