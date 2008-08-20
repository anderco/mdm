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

#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <cairo.h>
#include <cairo-xlib.h>

int main(int argc, char *argv[])
{
    int i;
    int screen_num;
    int display_width;
    int display_height;
    double y_all_extents = 0.0;
    double x,y;
    
    Display *display;
    Visual  *visual;
    Window  win;
    
    cairo_text_extents_t extents;
    cairo_surface_t *surface;
    cairo_t *cr;
    
    display = XOpenDisplay(NULL);
    if(display == NULL)
    {
        fprintf(stderr, "Cannot open diplay.\n");
        exit(1);
    }
    
    screen_num = DefaultScreen(display);
    
    win = DefaultRootWindow(display);
    display_width = DisplayWidth(display, screen_num);
    display_height = DisplayHeight(display, screen_num);
    visual = DefaultVisual(display, screen_num);

    surface = cairo_xlib_surface_create (display, 
                                         win, 
                                         visual, 
                                         display_width, 
                                         display_height );	
    
    cr = cairo_create (surface);
    
    /*Set black backgroud*/
    cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
    cairo_paint (cr);
    
    cairo_select_font_face (cr, "Arial", 
                            CAIRO_FONT_SLANT_NORMAL, 
                            CAIRO_FONT_WEIGHT_NORMAL);
    
    cairo_set_font_size (cr, 32.0);
    cairo_set_source_rgb (cr, 1.0, 1.0, 1.0);
    
    for (i = 1; i <= argc; i++)
    {
        cairo_text_extents(cr, argv[i], &extents);
        y_all_extents += (extents.height/2 + extents.y_bearing*2);
    }
    
    y_all_extents = y_all_extents/2.0;
    
    cairo_text_extents(cr, argv[1], &extents);
    x = display_width/2 - (extents.width/2 + extents.x_bearing);
    y = display_height/2 - (extents.height/2 + extents.y_bearing);
    
    y += y_all_extents;
    
    cairo_move_to(cr, x, y );
                
    for (i = 1; i < argc; i++)
    {
    	cairo_text_extents(cr, argv[i], &extents);
    	x = display_width/2 - (extents.width/2 + extents.x_bearing);
    	
	    cairo_move_to (cr, x, y );
    	cairo_show_text (cr, argv[i]);
                    
    	y -= (extents.height/2 + extents.y_bearing*2 );
    }
                
    XCloseDisplay(display);

   return 0; 
}   
