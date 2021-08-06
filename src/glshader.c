//#include <glshader.h>
#include "../inc/glshader.h"
#include <GL/glext.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static const char* TAG = "GLSL";
//if code_buffer is NULL this function can be used to determine a buffer size
static int get_raw_code(const char* file, size_t* code_buffer_size, char* code_buffer){
    FILE *fp;
    fp = fopen(file,"r");
    if(fp){        
        if(code_buffer == NULL){
            fseek(fp, 0L, SEEK_END);
            *code_buffer_size = ftell(fp);
            fclose(fp);
            return EXIT_SUCCESS;
        }

        char buff;
        while((buff = fgetc(fp)) != EOF){
            //printf("%c",buff);
            strncat(code_buffer, &buff,1);
        }

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
    char* vtxcode = "";
    size_t code_buffer_size = 0;
    get_raw_code(vrtxshdrf, &code_buffer_size, NULL);
    vtxcode = (char*)malloc(code_buffer_size);
    strcpy(vtxcode, "");
    get_raw_code(vrtxshdrf, &code_buffer_size, vtxcode);
    glShaderSource(vsid, 1, (const char**)&vtxcode, NULL);
    glCompileShader(vsid);
    dump_shader_status(vsid);
    //printf("[%s] Vertex shader DONE!...\n",TAG);
    /*---------------------------------*/
    /*       Fragmentshader            */
    /*---------------------------------*/
    char* fgmtcode = "";
    code_buffer_size = 0;
    get_raw_code(frgmntshdrf, &code_buffer_size, NULL);
    fgmtcode = (char*)malloc(code_buffer_size);
    strcpy(fgmtcode, "");
    get_raw_code(frgmntshdrf, &code_buffer_size, fgmtcode);
    glShaderSource(fsid, 1, (const char**)&fgmtcode, NULL);
    glCompileShader(fsid);
    dump_shader_status(fsid);
    //printf("[%s] Fragment shader DONE!...\n",TAG);
    /*---------------------------------*/
    /*            Linking              */
    /*---------------------------------*/
    //printf("[%s] Linking shaders...\n",TAG);
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vsid);
	glAttachShader(programID, fsid);
	glLinkProgram(programID);
    dump_shader_status(programID);
    glDetachShader(programID, vsid);
    glDetachShader(programID, fsid);

    free(vtxcode);
    free(fgmtcode);
    glDeleteShader(vsid);
    glDeleteShader(fsid);
    return programID;
}