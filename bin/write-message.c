/* XXX: THIS IS JUST A PRELIMINARY SCRIPT, USED FOR TESTING */

/*
 * Copyright (C) 2004-2007 Centro de Computacao Cientifica e Software Livre
 * Departamento de Informatica - Universidade Federal do Parana - C3SL/UFPR
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
 * USA.
 */


/* compiling with: 
 * gcc -Wall -g3 -O0 ttwrite1.c -o ttwrite `pkg-config --libs --cflags freetype2 x11 xft */

# include <stdlib.h>
# include <stdio.h>
# include <X11/Xlib.h>
# include <cairo.h>

int main (int argc, char *argv[] )
{
    Display *display = XOpenDisplay(NULL);
    Visual *visual;
    Window win;
    
    cairo_surface_t *surface; 
    cairo_text_extents_t extents;
    cairo_t *cr;
    
    int i;
    int screen_num = DefaultScreen(display);
    int display_width = DisplayWidth(display, screen_num);
    int display_height = DisplayHeight(display, screen_num);
    double x,y;
    unsigned int width, height;
    unsigned int win_x, win_y;
    unsigned int win_border_width;

    width = (display_width);
    height = (display_height);
    
    win_x = 0;
    win_y = 0;

    win_border_width = 3;

    /*win = XCreateSimpleWindow(display, 
                              RootWindow(display, screen_num),
                              win_x, 
                              win_y, 
                              width, 
                              height, 
                              win_border_width, 
                              BlackPixel(display, screen_num), 
                              WhitePixel(display, screen_num) ); */
    win = RootWindow(display, screen_num);
    
//    XSelectInput(display, win, ExposureMask |                                                ButtonPressMask | KeyPressMask);
    
    visual = DefaultVisual(display, screen_num);

    surface = cairo_xlib_surface_create (display, 
                                              win, 
                                              visual, 
                                              width, 
                                              height );	
    cr = cairo_create (surface);
    
    //XMapWindow(display, win);
    
    cairo_select_font_face (cr, "arial", 
                            CAIRO_FONT_SLANT_NORMAL, 
                            CAIRO_FONT_WEIGHT_NORMAL);
    
    cairo_set_font_size (cr, 32.0);
    cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);

                cairo_text_extents(cr, argv[1], &extents);
                x = width/2 - (extents.width/2 + extents.x_bearing);
                y = height/2 - (extents.height/2 + extents.y_bearing);
    
                cairo_move_to (cr, x, y );
                
                for (i = argc; i > 0; i--)
                {
                    cairo_text_extents(cr, argv[i], &extents);
                    cairo_move_to (cr, x, y );
                    cairo_show_text (cr, argv[i]);
                    
                    y = y + (extents.height/2 + extents.y_bearing*2 );
                }
                
/*    while (1) 
    {
        XNextEvent(display, &event);
        switch (event.type) 
        {
           case Expose:
                cairo_text_extents(cr, argv[1], &extents);
                x = width/2 - (extents.width/2 + extents.x_bearing);
                y = height/2 - (extents.height/2 + extents.y_bearing);
    
                cairo_move_to (cr, x, y );
                
                for (i = argc; i > 0; i--)
                {
                    cairo_text_extents(cr, argv[i], &extents);
                    cairo_move_to (cr, x, y );
                    cairo_show_text (cr, argv[i]);
                    
                    y = y + (extents.height/2 + extents.y_bearing*2 );
                }
                
                break;
           
           case ButtonPress:
                exit(0);
           
           case KeyPress:
                exit(0);
        }
    
   }        
*/  

    XFlush(display); 
    XCloseDisplay(display);


   return 0; 
     
}   
