#include <cglm/mat4.h>
#include <gizmo.h>

static GLuint vao, vbo, shader;
static GLuint shader_var_model;
static mat4 gizmoTrans;
void gizmo_init(){
    float gizmo_vertices[] = {
        -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
    };

    glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(
        GL_ARRAY_BUFFER, 
        sizeof(float) * 9 * GIZMO_VERTEX_COUNT,
        gizmo_vertices,
        GL_STATIC_DRAW
    );

    shader = glshader_load( "../shaders/gzimo.vertexshader.glsl", "../shaders/gzimo.fragmentshader.glsl" );
    shader_var_model = glGetUniformLocation(shader, "model");

    glm_mat4_identity(&gizmoTrans[0]);
    for(int a = 0; a < 16; a++){
        printf("%.2f\t", gizmoTrans[a]);
        if(a % 4 == 3)
        printf("\n");
    }
}

void gizmo_draw(GLuint radius){
    
    /*glm_mat4_scale(gizmoTrans, 30.0);

    vec3 translation = {10, 10, 10};
	glm_translate(gizmoTrans, translation);*/

    glBindVertexArray(vao);
    glUseProgram(shader);
    glUniformMatrix4fv(shader_var_model, 1, GL_FALSE, &gizmoTrans[0][0]);

    glDisable(GL_CULL_FACE);

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

    glDrawArrays(GL_TRIANGLES, 0, GIZMO_VERTEX_COUNT * 3);

    glDisableVertexAttribArray(0);
}

void gizmo_delete(){
    glDeleteProgram(shader);
    glDeleteVertexArrays(1, &vao);
}