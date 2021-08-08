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
static const char* TAG = "GTK GL";

static GLuint vertexbuffer, colourbuffer;

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


// Hold init data for GTK signals:
struct signal {
	const gchar	*signal;
	GCallback	 handler;
	GdkEventMask	 mask;
};

static void dump_mat4(float *mat, const char* title){
	printf("+----------------------+\n");
	printf("|   Dumping Matrix     |\n");
	printf("+----------------------+\n");
	printf("Begin:\t%s\n",title);
	for(int a = 0; a < 16; a++){
		printf("%.2f\t",mat[a]);
		if(a % 4 == 3)
		printf("\n");
	}
	printf("End:\t%s\n",title);
	printf("O----------------------O\n");
}

static void init_test_cube_buffers(){
	//Cube data from: https://github.com/opengl-tutorials/ogl/blob/master/tutorial04_colored_cube/tutorial04.cpp
	const GLfloat test_cube_vertex_buffer_data[] = { 
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
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

	const GLfloat test_cube_color_buffer_data[] = { 
		0.583f,  0.771f,  0.014f,
		0.609f,  0.115f,  0.436f,
		0.327f,  0.483f,  0.844f,
		0.822f,  0.569f,  0.201f,
		0.435f,  0.602f,  0.223f,
		0.310f,  0.747f,  0.185f,
		0.597f,  0.770f,  0.761f,
		0.559f,  0.436f,  0.730f,
		0.359f,  0.583f,  0.152f,
		0.483f,  0.596f,  0.789f,
		0.559f,  0.861f,  0.639f,
		0.195f,  0.548f,  0.859f,
		0.014f,  0.184f,  0.576f,
		0.771f,  0.328f,  0.970f,
		0.406f,  0.615f,  0.116f,
		0.676f,  0.977f,  0.133f,
		0.971f,  0.572f,  0.833f,
		0.140f,  0.616f,  0.489f,
		0.997f,  0.513f,  0.064f,
		0.945f,  0.719f,  0.592f,
		0.543f,  0.021f,  0.978f,
		0.279f,  0.317f,  0.505f,
		0.167f,  0.620f,  0.077f,
		0.347f,  0.857f,  0.137f,
		0.055f,  0.953f,  0.042f,
		0.714f,  0.505f,  0.345f,
		0.783f,  0.290f,  0.734f,
		0.722f,  0.645f,  0.174f,
		0.302f,  0.455f,  0.848f,
		0.225f,  0.587f,  0.040f,
		0.517f,  0.713f,  0.338f,
		0.053f,  0.959f,  0.120f,
		0.393f,  0.621f,  0.362f,
		0.673f,  0.211f,  0.457f,
		0.820f,  0.883f,  0.371f,
		0.982f,  0.099f,  0.879f
	};

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(test_cube_vertex_buffer_data), test_cube_vertex_buffer_data, GL_STATIC_DRAW);

	glGenBuffers(1, &colourbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colourbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(test_cube_color_buffer_data), test_cube_color_buffer_data, GL_STATIC_DRAW);
}

static void handle_perspective(mat4 transform){
	GLfloat deltaX, deltaY;
	//gmouse_get_movement_offset(&deltaX, &deltaY);
	deltaX = *movement_offset_buffer[0];
	deltaY = *movement_offset_buffer[1];

	if(deltaX != 0 || deltaY != 0){
		movement_angle_horizontal = movement_angle_horizontal + (movement_mouse_speed * deltaX);
		movement_angle_vertical = movement_angle_vertical + (movement_mouse_speed * deltaY);
	}

	vec3 eye = {4,3,-3};
	vec3 direction = {0,0,0};
	vec3 up = {0,1,0};

	mat4 identity, view, projection, buffer;
	glm_mat4_identity(identity);
	glm_lookat(eye, direction, up, view);
	glm_perspective(M_PI/4, window_ratio, 0.1f, 100.0f, projection);
	glm_mat4_mul(projection, view, buffer);
	glm_mat4_mul(buffer, identity, transform);
	//dump_mat4((float*)transform, "Transform");
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
	/*---------------------------------*/
    /*       Initialize Buffers        */
    /*---------------------------------*/
	init_test_cube_buffers();

	/*---------------------------------*/
    /*       Init GTK stuff            */
    /*---------------------------------*/
	GdkGLContext *glcontext = gtk_gl_area_get_context(glarea);
	GdkWindow *glwindow = gdk_gl_context_get_window(glcontext);
	GdkFrameClock *frame_clock = gdk_window_get_frame_clock(glwindow);

	g_signal_connect_swapped(
		frame_clock,
		"update",
		G_CALLBACK(gtk_gl_area_queue_render),
		glarea
	);

	// Start updating:
	gdk_frame_clock_begin_updating(frame_clock);
}

static bool on_render(){
	mat4 transformation_matrix;
	handle_perspective(transformation_matrix);
    // Clear canvas:
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderID);

	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &transformation_matrix[0][0]);
    /*---------------------------------*/
    /*       Render Vertices           */
    /*---------------------------------*/
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

	/*---------------------------------*/
    /*      Render Colours             */
    /*---------------------------------*/
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colourbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
	// Draw the triangles!
	glDrawArrays(GL_TRIANGLES, 0, 12*3); // 3 indices starting at 0 -> 1 triangle

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
    return true;
}

static void on_resize(GtkGLArea *area, int width, int height){
	window_width = width;
	window_height = height;
	window_ratio = (float)width / (float)height;
	//printf("[on resize] Offset buffer was: %f %f\n", (*movement_offset_buffer)[0], (*movement_offset_buffer)[1]);
}

static bool on_button_press (GtkWidget *widget, GdkEventButton *event){
	//GtkAllocation allocation;
	//gtk_widget_get_allocation(widget, &allocation);

	if (event->button == 1)
	printf("Button Press\nprev render time: %d\n",previousRenderTime);
		/*if (panning == FALSE) {
			panning = TRUE;
			model_pan_start(event->x, allocation.height - event->y);
		}*/

	return FALSE;
}

static bool on_button_release (GtkWidget *widget, GdkEventButton *event){
	if (event->button == 1)
	printf("Button Release\n");
	//panning = FALSE;

	return FALSE;
}

static bool on_motion_notify (GtkWidget *widget, GdkEventMotion *event){
	GtkAllocation allocation;
	gtk_widget_get_allocation(widget, &allocation);

	//if (panning == TRUE)
	//model_pan_move(event->x, allocation.height - event->y);
	printf("Motion Notify");

	return FALSE;
}

static bool on_scroll (GtkWidget* widget, GdkEventScroll *event){
	switch (event->direction)
	{
	case GDK_SCROLL_UP:
		printf("Scroll Up");
		break;

	case GDK_SCROLL_DOWN:
		printf("Scroll Down");
		break;

	default:
		break;
	}

	return FALSE;
}

static void connect_signals (GtkWidget *widget, struct signal *signals, size_t members){
	for(int a = 0; a < members; a++){
        gtk_widget_add_events(widget, signals[a].mask);
        g_signal_connect(widget, signals[a].signal, signals[a].handler, NULL);
    }
}

static void connect_glarea_signals(GtkWidget *glarea){
    struct signal signals[] = {
		{ "realize",			G_CALLBACK(on_realize),		0			},
		{ "render",			G_CALLBACK(on_render),		0			},
		{ "resize",			G_CALLBACK(on_resize),		0			},
		{ "scroll-event",		G_CALLBACK(on_scroll),		GDK_SCROLL_MASK		},
		{ "button-press-event",		G_CALLBACK(on_button_press),	GDK_BUTTON_PRESS_MASK	},
		{ "button-release-event",	G_CALLBACK(on_button_release),	GDK_BUTTON_RELEASE_MASK	},
		{ "motion-notify-event",	G_CALLBACK(on_motion_notify),	GDK_BUTTON1_MOTION_MASK	},
	};
	const size_t callbackCount = sizeof(signals)/sizeof(struct signal);
	//printf("Sizeof signals is: %d\n",callbackCount);
    connect_signals(glarea, signals, callbackCount);
}

int glwrap_init_gl(GtkWidget *window, GtkWidget *parentLayout){
    /*---------------------------------*/
    /*           GTK+                  */
    /*---------------------------------*/
    glarea = gtk_gl_area_new();
    gtk_box_pack_start(GTK_BOX(parentLayout), glarea, TRUE, TRUE, 0);
    connect_glarea_signals(glarea);

	movement_offset_buffer = (vec2*)malloc(sizeof(*movement_offset_buffer));
	*movement_offset_buffer[0] = 0;
	*movement_offset_buffer[1] = 0;
	//gmouse_init_mouse_events(window, movement_offset_buffer);
    /*---------------------------------*/
    /*           OpenGL                */
    /*---------------------------------*/
    
    return EXIT_SUCCESS;
}

int glwrap_cleanup(){
	free(movement_offset_buffer);
    glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colourbuffer);
    glDeleteVertexArrays(1, &vertexArrayID);
    glDeleteProgram(shaderID);
}