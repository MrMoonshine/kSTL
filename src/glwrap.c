//#include <glwrap.h>
#include "../inc/glwrap.h"
#include <cglm/cam.h>
#include <cglm/mat4.h>
#include <cglm/types.h>
#include <stdlib.h>

static GtkWidget* glarea;
static const char* TAG = "OpenGL";
static GLuint vertexbuffer;
static GLuint shaderID;
static GLuint mvpID;
static GLuint vertexArrayID;
static GLfloat window_ratio = 1.0;
// Hold init data for GTK signals:
struct signal {
	const gchar	*signal;
	GCallback	 handler;
	GdkEventMask	 mask;
};

static void handle_perspective(){
	mat4 origin;
	glm_mat4_zero(origin);
	mat4 projection;
	glm_perspective_default(window_ratio, projection);
	mat4 view;
	vec3 eye = {-2,-3,-1};
	vec3 center = {-2,-3,-1};
	vec3 up = {0,0,-1};
	glm_lookat(eye, center, up, view);

	mat4 MVP;
	glm_mat4_mul(view, projection, MVP);
	//glm_mat4_mul(MVP, origin, MVP);

	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &MVP[0][0]);
}

static void on_realize(GtkGLArea *glarea){
    // Make current:
	gtk_gl_area_make_current(glarea);
    //Dump info string
    const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	printf("[%s] Renderer: %s\n", TAG, renderer);
	printf("[%s] Version: %s\n", TAG, version);

    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "[%s] Failed to initialize GLEW\n",TAG);
        return;
    } 
    // Enable depth buffer:
	gtk_gl_area_set_has_depth_buffer(glarea, TRUE);
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
    //init background
    //background_init();
    // Dark purple background
	glClearColor(0.4f, 0.0f, 0.4f, 0.0f);
	// For testing purposes:
	glDisable(GL_CULL_FACE);
    //Test Triangle
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

    shaderID = glshader_load( "../shaders/vertexshd1.glsl", "../shaders/fragmentshd1.glsl" );
	mvpID = glGetUniformLocation(shaderID, "MVP");

    static const GLfloat g_vertex_buffer_data[] = { 
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,
	};

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
}

static bool on_render(){
	handle_perspective();
    // Clear canvas:
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderID);
    // 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

	glDisableVertexAttribArray(0);
    return true;
}

static void on_resize(GtkGLArea *area, int width, int height){
	window_ratio = (float)width / (float)height;
}

static void connect_signals (GtkWidget *widget, struct signal *signals, size_t members){
	for(int a = 0; a < members; a++){
        gtk_widget_add_events(widget, signals[a].mask);
        g_signal_connect(widget, signals[a].signal, signals[a].handler, NULL);
    }
}

int glwrap_init_gl(GtkWidget *parentLayout){
	printf("Size of mat4 is %d\n", sizeof(mat4));
    /*---------------------------------*/
    /*           GTK+                  */
    /*---------------------------------*/
    glarea = gtk_gl_area_new();
    gtk_box_pack_start(GTK_BOX(parentLayout), glarea, TRUE, TRUE, 0);

    const size_t callbackCount = 3;
    struct signal signals[] = {
		{ "realize",			G_CALLBACK(on_realize),		0			},
		{ "render",			G_CALLBACK(on_render),		0			},
		{ "resize",			G_CALLBACK(on_resize),		0			},
		//{ "button-press-event",		G_CALLBACK(glwrap_mouse_click),	GDK_BUTTON_PRESS_MASK	},
		/*{ "button-release-event",	G_CALLBACK(on_button_release),	GDK_BUTTON_RELEASE_MASK	},
		{ "motion-notify-event",	G_CALLBACK(on_motion_notify),	GDK_BUTTON1_MOTION_MASK	},*/
	};
    connect_signals(glarea, signals, callbackCount);
    /*---------------------------------*/
    /*           OpenGL                */
    /*---------------------------------*/
    
    return EXIT_SUCCESS;
}

int glwrap_cleanup(){
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteVertexArrays(1, &vertexArrayID);
    glDeleteProgram(shaderID);
}