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

static void printVec3(vec3 vector, const char* label){
    printf("%s = (%.2f|%.2f|%.2f)\n", label, vector[0], vector[1], vector[2]);
}

static void errorInfoGL(){
    GLenum err;
    printf("OpenGL eroor: %x\n", glGetError());
}

static int stl_binary_file_load(GLuint* vbo, GLuint *normals, GLuint *vertexCount, const char *filename){
    char header[STL_HEADER_SIZE] = "";
    uint32_t number_of_vertices;
    float *vertex_buffer, *normals_buffer;
    
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
        normals_buffer = (float*)malloc(number_of_vertices * 3 * sizeof(float));
        if(vertex_buffer == NULL || normals_buffer == NULL){
            fprintf(stderr, "[%s] Failed to allocate Memory!",TAG);
            fclose(stlfp);
            return EXIT_FAILURE;
        }

        for(int v = 0; v < number_of_vertices; v++){
            //vec3 normal;
            //fread(normal, sizeof(float) * 3, 1, stlfp);
            //printVec3(normal, "Normalvektor");
            fread(normals_buffer + (v*3), sizeof(float) * 3, 1, stlfp);
            
            for(int b = 0; b < STL_VERTEX_FLOAT_COUNT; b++){
                fread(
                    vertex_buffer + (v*STL_VERTEX_FLOAT_COUNT + b),
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
        /*for(int a = 0; a < number_of_vertices * STL_VERTEX_FLOAT_COUNT; a++){
            printf("%.2f\t",vertex_buffer[a]);
            if(a%3 == 2){
                printf("\n");
            }           
        }*/

        glGenBuffers(1, vbo);
	    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	    glBufferData(
            GL_ARRAY_BUFFER, 
            sizeof(float) * STL_VERTEX_FLOAT_COUNT * number_of_vertices,
            vertex_buffer,
            GL_STATIC_DRAW
        );

        glGenBuffers(1, normals);
	    glBindBuffer(GL_ARRAY_BUFFER, *normals);
	    glBufferData(
            GL_ARRAY_BUFFER, 
            number_of_vertices * 3 * sizeof(float),
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

    *vertexCount = number_of_vertices;
    return EXIT_SUCCESS;
}

void stl_model_init(GLuint* vbo, GLuint *normals, GLuint *vertexCount, const char *filename){
    stl_binary_file_load(vbo, normals, vertexCount, filename);
    //Cube data from: https://github.com/opengl-tutorials/ogl/blob/master/tutorial04_colored_cube/tutorial04.cpp
	/*const GLfloat test_cube_vertex_buffer_data[] = {
        0.00,   0.00,   0.00,
        0.00,   0.00,   10.00,
        0.00,   10.00,  0.00,
        0.00,   10.00,  0.00,
        0.00,   0.00,   10.00,
        0.00,   10.00,  10.00,
        10.00,  0.00,   10.00,
        10.00,  0.00,   0.00,
        10.00,  10.00,  0.00,
        10.00,  0.00,   10.00,
        10.00,  10.00,  0.00,
        10.00,  10.00,  10.00,
        10.00,  0.00,   0.00,
        10.00,  0.00,   10.00,
        0.00,   0.00,   0.00,
        0.00,   0.00,   0.00,
        10.00,  0.00,   10.00,
        0.00,   0.00,   10.00,
        10.00,  10.00,  10.00,
        10.00,  10.00,  0.00,
        0.00,   10.00,  0.00,
        10.00,  10.00,  10.00,
        0.00,   10.00,  0.00,
        0.00,   10.00,  10.00,
        0.00,   10.00,  0.00,
        10.00,  10.00,  0.00,
        0.00,   0.00,   0.00,
        0.00,   0.00,   0.00,
        10.00,  10.00,  0.00,
        10.00,  0.00,   0.00,
        10.00,  10.00,  10.00,
        0.00,   10.00,  10.00,
        0.00,   0.00,   10.00,
        10.00,  10.00,  10.00,
        0.00,   0.00,   10.00,
        10.00,  0.00,   10.00,
	};

	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(test_cube_vertex_buffer_data), test_cube_vertex_buffer_data, GL_STATIC_DRAW);*/
}

void stl_model_draw(GLuint vbo, GLuint normals, GLuint vertexCount){
    /*---------------------------------*/
    /*       Render Vertices           */
    /*---------------------------------*/
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
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
    glBindBuffer(GL_ARRAY_BUFFER, normals);
    glVertexAttribPointer(
        1,                                // attribute
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );
	// Draw the triangles!
	glDrawArrays(GL_TRIANGLES, 0, vertexCount*3); // 3 indices starting at 0 -> 1 triangle

	glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}