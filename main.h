#ifndef MAIN_H
#define MAIN_H

#define WIDTH   640
#define HEIGHT  480
#define PTCOUNT 9
#define RADIUS  100
#define DEBUG   1

/*
====================
Global variables

====================
*/
int year;
int month;
int day;


/*
====================
Functions

====================
*/
void productPayDateButtonAction( GtkButton *, gpointer );
void addButtonAction( GtkButton *, gpointer );
int writeToFile( int, int, int, char *, char *, int );
gboolean graphBoxAreaAction( GtkWidget *, cairo_t *, gpointer );

#endif
