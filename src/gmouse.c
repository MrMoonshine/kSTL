#include <cglm/types.h>
#include <gmouse.h>
#include <stdio.h>

static bool enable_motion_event = false;
static double tentativeOriginX = 0, tentativeOriginY = 0;

static gboolean gmouse_mouse_scroll(GtkWidget *widget, GdkEventScroll* event, gpointer user_data){
	if(event->direction == GDK_SCROLL_UP){
		printf("Scroll UP\n");
	}else if(event->direction == GDK_SCROLL_DOWN){
		printf("Scroll DOWN\n");
	}
	return TRUE;
}

static gboolean gmouse_mouse_click(GtkWidget* widget, GdkEventButton *event){
	if(event->button == 1){
        enable_motion_event = true;
        tentativeOriginX = event->x;
        tentativeOriginY = event->y;
		//printf("Clicked Left at: %f %f\n",event->x, event->y);
	}
	return TRUE;
}

static gboolean gmouse_mouse_release(GtkWidget* widget, GdkEventButton *event, vec2 *data){
	if(event->button == 1){
        enable_motion_event = false;
        tentativeOriginX = 0;
        tentativeOriginY = 0;
		//printf("Released Left at: %f %f\n",event->x, event->y);
        //printf("Offset buffer was: %f %f\n", (*data)[0], (*data)[1]);
        //(*data)[0] = 0;
        //(*data)[1] = 0;
	}
	return TRUE;
}

static gboolean gmouse_mouse_motion(GtkWidget* widget, GdkEventMotion *event, vec2 *data){
    if(enable_motion_event){
        (*data)[0] = event->x - tentativeOriginX;
        (*data)[1] = event->y - tentativeOriginY;
    }
	return TRUE;
}

void gmouse_init_mouse_events(GtkWidget *window, vec2* buffer){
    gtk_widget_add_events(window, GDK_BUTTON_PRESS_MASK);
  	gtk_widget_add_events(window, GDK_SCROLL_MASK);
	gtk_widget_add_events(window, GDK_BUTTON_RELEASE_MASK);
    gtk_widget_add_events(window, GDK_BUTTON1_MOTION_MASK);

	g_signal_connect(window, "scroll-event", G_CALLBACK(gmouse_mouse_scroll), NULL);
	g_signal_connect(window, "button-press-event", G_CALLBACK(gmouse_mouse_click), NULL);
	g_signal_connect(window, "button-release-event", G_CALLBACK(gmouse_mouse_release), buffer);
    g_signal_connect(window, "motion-notify-event", G_CALLBACK(gmouse_mouse_motion), buffer);
}

void gmouse_get_movement_offset(float *x, float *y){

}