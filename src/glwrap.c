//#include <glwrap.h>
#include "../inc/glwrap.h"
#include <cglm/cam.h>
#include <cglm/mat4.h>
#include <cglm/types.h>
#include <cglm/vec3.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


static GtkWidget* glarea;
static const char* TAG = "OpenGL";
static GLuint vertexbuffer;
static GLuint shaderID;
static GLuint mvpID;
static GLuint vertexArrayID;
static GLfloat window_ratio = 1.0;
static GLuint window_width = APP_WIDTH, window_height = APP_HEIGHT;
static clock_t previousRenderTime = 0;

//Field of view
static const float FOV = 45.0f;
static const float movement_mouse_sensitivity = 0.005f;
static const float movement_mouse_speed = 3.0f;

static float movement_angle_horizontal = 0.0f;
static float movement_angle_vertical = 0.0f;
static vec2 *movement_offset_buffer;
static vec3 movement_camera_position;
// Hold init data for GTK signals:
struct signal {
	const gchar	*signal;
	GCallback	 handler;
	GdkEventMask	 mask;
};

static void handle_perspective(clock_t delta_t){
	GLfloat deltaX, deltaY;
	//gmouse_get_movement_offset(&deltaX, &deltaY);
	deltaX = *movement_offset_buffer[0];
	deltaY = *movement_offset_buffer[1];

	if(deltaX != 0 || deltaY != 0){
		movement_angle_horizontal = movement_angle_horizontal + (movement_mouse_speed * deltaX);
		movement_angle_vertical = movement_angle_vertical + (movement_mouse_speed * deltaY);
	}
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
	mvpID = glGetUniformLocation(shaderID, "transform");

    static const GLfloat g_vertex_buffer_data[] = { 
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,
	};

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glm_vec3_zero(movement_camera_position);
	previousRenderTime = clock();
}

static bool on_render(){
	/*---------------------------------*/
    /*       Handle Render Times       */
    /*---------------------------------*/
	clock_t current_t, delta_t;
	current_t = clock();
	delta_t = current_t - previousRenderTime;
	previousRenderTime = current_t;
	//Get time unit in ms
	delta_t = (double)(delta_t * 1000.0f)/ CLOCKS_PER_SEC;
	handle_perspective(delta_t);
    // Clear canvas:
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderID);
	mat4 trans = {
		1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, 1, 1,
	};
	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &trans[0][0]);
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
	window_width = width;
	window_height = height;
	window_ratio = (float)width / (float)height;
	//printf("[on resize] Offset buffer was: %f %f\n", (*movement_offset_buffer)[0], (*movement_offset_buffer)[1]);
}

static void connect_signals (GtkWidget *widget, struct signal *signals, size_t members){
	for(int a = 0; a < members; a++){
        gtk_widget_add_events(widget, signals[a].mask);
        g_signal_connect(widget, signals[a].signal, signals[a].handler, NULL);
    }
}

int glwrap_init_gl(GtkWidget *window, GtkWidget *parentLayout){
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

	movement_offset_buffer = (vec2*)malloc(sizeof(*movement_offset_buffer));
	*movement_offset_buffer[0] = 0;
	*movement_offset_buffer[1] = 0;
	gmouse_init_mouse_events(window, movement_offset_buffer);
    /*---------------------------------*/
    /*           OpenGL                */
    /*---------------------------------*/
    
    return EXIT_SUCCESS;
}

int glwrap_cleanup(){
	free(movement_offset_buffer);
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteVertexArrays(1, &vertexArrayID);
    glDeleteProgram(shaderID);
}