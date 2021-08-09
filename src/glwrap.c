//#include <glwrap.h>
#include "../inc/glwrap.h"
#include <cglm/affine.h>
#include <cglm/cam.h>
#include <cglm/mat4.h>
#include <cglm/types.h>
#include <cglm/vec3.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


static GtkWidget* glarea;
static const char* TAG = "GTK GL";

static char stl_filename[128] = "";

static struct MetaSTL stl_mesh;
static GLuint colour_location;

static GLuint shaderID;
static GLuint shader_var_projection, shader_var_view, shader_var_model, shader_var_viewPos;
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

static double tentativeOriginX = 0, tentativeOriginY = 0;
static bool clicked = false;
static vec3 mycolour = {0.9f, 0.9f, 0.0f};
static vec3 translation = {0.0, 0.0, -20.0};

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

static void handle_perspective(mat4 transform){
	vec3 eye = {60,45,-45};
	vec3 direction = {0,0,0};
	vec3 up = {0,1,0};

	vec3 axis = {1.0f, 0.0f, 0.0f};
	mat4 identity, view, projection, buffer;
	glm_mat4_identity(identity);
	glm_lookat(eye, direction, up, view);
	glm_perspective(M_PI/4, window_ratio, 1.0f, 500.0f, projection);
	//Rotate Cube
	glm_rotate(identity, -M_PI/2, axis);

	glm_translate(identity, translation);

	//Multiplication will be done on the GPU
	glUniformMatrix4fv(shader_var_projection, 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(shader_var_view, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(shader_var_model, 1, GL_FALSE, &identity[0][0]);
	glUniform3fv(shader_var_viewPos, 1, eye);
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
	// Performace upgrade
	glEnable(GL_CULL_FACE);
    //Test Triangle
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

    shaderID = glshader_load( "../shaders/vertexshd1.glsl", "../shaders/fragmentshd1.glsl" );
	shader_var_projection = glGetUniformLocation(shaderID, "projection");
	shader_var_view = glGetUniformLocation(shaderID, "view");
	shader_var_model = glGetUniformLocation(shaderID, "model");
	shader_var_viewPos = glGetUniformLocation(shaderID, "viewPos");

	colour_location = glGetUniformLocation(shaderID, "material_colour");
	/*---------------------------------*/
    /*       Initialize Buffers        */
    /*---------------------------------*/
	stl_model_init(&stl_mesh,stl_filename);

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
    glUseProgram(shaderID);
	handle_perspective(transformation_matrix);
	
	glUniform3fv(colour_location, 1, mycolour);
	// Clear canvas:
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	stl_model_draw(&stl_mesh);
    
    return true;
}

static void on_resize(GtkGLArea *area, int width, int height){
	window_width = width;
	window_height = height;
	window_ratio = (float)width / (float)height;
}

static bool on_button_press (GtkWidget *widget, GdkEventButton *event){
	if (event->button == 1){
		clicked = true;
		tentativeOriginX = event->x;
		tentativeOriginY = event->y;
	}
	return FALSE;
}

static bool on_button_release (GtkWidget *widget, GdkEventButton *event){
	if (event->button == 1){
		clicked = false;
	}
	return FALSE;
}

static bool on_motion_notify (GtkWidget *widget, GdkEventMotion *event){
	if(clicked){
		translation[0] += (tentativeOriginX - event->x)/1000.0;
		translation[2] += (tentativeOriginY - event->y)/1000.0;
	}
	return FALSE;
}

static bool on_scroll (GtkWidget* widget, GdkEventScroll *event){
	switch (event->direction)
	{
	case GDK_SCROLL_UP:{
			mycolour[1] += 0.05;
			if(mycolour[1] > 1.0)
			mycolour[1] = 1.0;
		}
		break;

	case GDK_SCROLL_DOWN:{
			mycolour[1] -= 0.05;
			if(mycolour[1] < 0.0)
			mycolour[1] = 0.0;
		}
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

int glwrap_load_mesh(const char* filename){
	stl_model_free(&stl_mesh);
	return stl_model_init(&stl_mesh, filename);
}

int glwrap_init_gl(const char* filename, GtkWidget *window, GtkWidget *parentLayout){
	stl_mesh.success = false;
	strcpy(stl_filename, filename);
    /*---------------------------------*/
    /*           GTK+                  */
    /*---------------------------------*/
    glarea = gtk_gl_area_new();
    gtk_box_pack_start(GTK_BOX(parentLayout), glarea, TRUE, TRUE, 0);
    connect_glarea_signals(glarea);
    /*---------------------------------*/
    /*           OpenGL                */
    /*---------------------------------*/
    
    return EXIT_SUCCESS;
}

int glwrap_cleanup(){
    stl_model_free(&stl_mesh);
    glDeleteVertexArrays(1, &vertexArrayID);
    glDeleteProgram(shaderID);
}