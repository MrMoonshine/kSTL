//#include <glshader.h>
#include "../inc/glshader.h"
#include <GL/glext.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static const char* TAG = "GLSL";
static char* code_buffer = NULL;
//if code_buffer is NULL this function can be used to determine a buffer size
static int get_raw_code(const char* file, char* code){
    FILE *fp;
    fp = fopen(file,"r");
    if(fp){
        fseek(fp, 0L, SEEK_END);
        
        code_buffer = (char*)malloc(ftell(fp));
        if(!code_buffer)
        return EXIT_FAILURE;

        rewind(fp);
        char buff;
        while((buff = fgetc(fp)) != EOF){
            //printf("%c",buff);
            strncat(code_buffer, &buff,1);
        }
        code = code_buffer;
        //free(code_buffer);
        fclose(fp);
    }else{
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;  
}

static void dump_shader_status(GLuint shdrid){
    GLint result = GL_FALSE;
	int infoLogLength;

    // Check Vertex Shader
	glGetShaderiv(shdrid, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shdrid, GL_INFO_LOG_LENGTH, &infoLogLength);
    if(infoLogLength > 0){
        char text[infoLogLength + 1];
        glGetShaderInfoLog(shdrid, infoLogLength, NULL, text);
        fprintf(stderr, "[%s] %s", TAG, text);
    }
} 

GLuint glshader_load(const char* vrtxshdrf, const char* frgmntshdrf){
    //printf("Loading Shaders:\n%s\n%s\n",vrtxshdrf,frgmntshdrf);
    GLuint vsid = glCreateShader(GL_VERTEX_SHADER);
    GLuint fsid = glCreateShader(GL_FRAGMENT_SHADER);
    printf("[%s] Compiling shaders...\n",TAG);

    /*---------------------------------*/
    /*         Vertexshader            */
    /*---------------------------------*/
    get_raw_code(vrtxshdrf, code_buffer);
    glShaderSource(vsid, 1, (const char**)&code_buffer, NULL);
    glCompileShader(vsid);
    dump_shader_status(vsid);
    free(code_buffer);
    /*---------------------------------*/
    /*       Fragmentshader            */
    /*---------------------------------*/
    get_raw_code(frgmntshdrf, code_buffer);
    glShaderSource(fsid, 1, (const char**)&code_buffer, NULL);
    glCompileShader(fsid);
    dump_shader_status(fsid);
    free(code_buffer);
    /*---------------------------------*/
    /*            Linking              */
    /*---------------------------------*/
    printf("[%s] Linking shaders...\n",TAG);

    return 0;
}