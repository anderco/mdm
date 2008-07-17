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
/* This program creates a "parent window" for xephyr      
 * and waits forever.
 * parameters: widthxheight+x+y, window name. 
 */ 

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <X11/Xlib.h>
# include <X11/Xutil.h>

int main(int argc, char *argv[])
{
    int screen;
    int x, y;
    int rc;
    unsigned int width, height;
    
    Display *display = XOpenDisplay(NULL);
    if (display == NULL) 
    {
       fprintf(stderr, "Cannot open display.\n");
       exit(1);
    }
    
    screen = DefaultScreen(display);

    sscanf(argv[1], "%dx%d+%d+%d", &width, &height, &x, &y); 

    Window win = XCreateSimpleWindow (display, DefaultRootWindow(display),
                                      x, y, width, height, 0,
                                      WhitePixel(display, screen),
                                      BlackPixel(display, screen));
    
    
    XTextProperty window_name_property; 
    
    rc = XStringListToTextProperty(&argv[2], 1, &window_name_property);
    if (rc == 0) 
    {
        fprintf(stderr, "XStringListToTextProperty - out of memory\n");
        exit(1);
    }

    XSetWMName(display, win, &window_name_property);    
    
    XMapWindow(display, win);
    XFlush(display);

    pause();
    
    XCloseDisplay(display);

    return 0;
}
