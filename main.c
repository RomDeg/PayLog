#include <gtk/gtk.h>
#include <cairo.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "paylog.h"

/*
====================
MAIN

====================
*/
int main( int argc, char **argv ) {
    //Local variables
    GtkWidget *window;
    GtkWidget *mainBox;
    GtkWidget *graphBox;
    GtkWidget *graphBoxArea;
    GtkWidget *selectBox;
    GtkWidget *buttonBox;
    GtkWidget *productTypeCB;
    GtkWidget *productPayDateButton;
    GtkWidget *productNameEntry;
    GtkWidget *productCostEntry;
    GtkWidget *addButton;
    GtkWidget *settingsButton;
    gpointer   addButtonP[3];
    time_t     currentTime;
    struct tm *timeStruct;

    //Function logic
    //Get current time
    currentTime = time( NULL );
    timeStruct = localtime( &currentTime );
    year = timeStruct->tm_year + 1900;
    month = timeStruct->tm_mon + 1;
    day = timeStruct->tm_mday;

    //Запуск GTK+
    gtk_init( &argc, &argv );

    //Создание нового окна
    window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    //Задание окну заголовока
    gtk_window_set_title( GTK_WINDOW( window ), "Pay List" );
    //Установка внутренних границ
    gtk_container_set_border_width( GTK_CONTAINER( window ), 10 );
    //Установка размеров
    gtk_window_set_default_size( GTK_WINDOW( window ), WIDTH, HEIGHT );
    //Запретить изменение размеров
    gtk_window_set_resizable( GTK_WINDOW( window ), FALSE );
    //Когда пользователь закроет окно, то выйти из приложения, вызвать функцию gtk_main_quit()
    g_signal_connect( G_OBJECT( window ), "destroy", G_CALLBACK( gtk_main_quit ), NULL );

    //Creating objects
    mainBox = gtk_fixed_new(  );
    graphBox = gtk_fixed_new(  );
    graphBoxArea = gtk_drawing_area_new(  );
    gtk_container_add( GTK_CONTAINER(graphBox), graphBoxArea );
    selectBox = gtk_box_new( GTK_ORIENTATION_HORIZONTAL, 10 );
    buttonBox = gtk_box_new( GTK_ORIENTATION_HORIZONTAL, 10 );
    productTypeCB = gtk_combo_box_text_new(  );
    //productTypeCB = gtk_combo_box_new(  );
    gtk_combo_box_text_append( GTK_COMBO_BOX_TEXT( productTypeCB ), NULL, "Products" );
    gtk_combo_box_text_append( GTK_COMBO_BOX_TEXT( productTypeCB ), NULL, "Dining room" );
    gtk_combo_box_text_append( GTK_COMBO_BOX_TEXT( productTypeCB ), NULL, "Restaurant" );
    gtk_combo_box_text_append( GTK_COMBO_BOX_TEXT( productTypeCB ), NULL, "Household chemicals" );
    gtk_combo_box_text_append( GTK_COMBO_BOX_TEXT( productTypeCB ), NULL, "Payments" );
    gtk_combo_box_text_append( GTK_COMBO_BOX_TEXT( productTypeCB ), NULL, "Clothing" );
    gtk_combo_box_text_append( GTK_COMBO_BOX_TEXT( productTypeCB ), NULL, "Entertainment" );
    gtk_combo_box_text_append( GTK_COMBO_BOX_TEXT( productTypeCB ), NULL, "For home" );
    gtk_combo_box_text_append( GTK_COMBO_BOX_TEXT( productTypeCB ), NULL, "Health" );
    productPayDateButton = gtk_button_new_with_label( "Change date" );
    productNameEntry = gtk_entry_new(  );
    //gtk_entry_set_max_width_chars( GTK_ENTRY( productNameEntry ), 2048 );
    gtk_entry_set_placeholder_text( GTK_ENTRY( productNameEntry ), "Subject of purchase" );
    productCostEntry = gtk_entry_new(  );
    gtk_entry_set_input_purpose( GTK_ENTRY( productCostEntry ), GTK_INPUT_PURPOSE_DIGITS );
    gtk_entry_set_placeholder_text( GTK_ENTRY( productCostEntry ), "Purchase price" );
    addButton = gtk_button_new_with_label( "Add" );
    //gtk_toggle_button_set_active( addButton, TRUE );
    settingsButton = gtk_button_new_with_label( "Settings" );

    //Placing objects
    gtk_container_add( GTK_CONTAINER( window ), mainBox );
    gtk_fixed_put( GTK_FIXED( mainBox ), graphBox, 10, 10 );
    gtk_fixed_put( GTK_FIXED( mainBox ), selectBox, 10, 390 );
    gtk_widget_set_size_request( productTypeCB, 180, 30 );
    gtk_box_pack_start( GTK_BOX( selectBox ), productTypeCB, 1, 1, 0 );
    gtk_widget_set_size_request( productPayDateButton, 120, 30 );
    gtk_box_pack_start( GTK_BOX( selectBox ), productPayDateButton , 1, 1, 0 );
    gtk_box_pack_start( GTK_BOX( selectBox ), productNameEntry, 1, 1, 0 );
    gtk_box_pack_start( GTK_BOX( selectBox ), productCostEntry, 1, 1, 0 );
    gtk_fixed_put( GTK_FIXED( mainBox ), buttonBox, 10, 450 );
    gtk_widget_set_size_request( addButton, 180, 30 );
    gtk_widget_set_size_request( settingsButton, 120, 30 );
    gtk_box_pack_start( GTK_BOX( buttonBox ), addButton, 1, 1, 0 );
    gtk_box_pack_start( GTK_BOX( buttonBox ), settingsButton, 1, 1, 0 );

    //Actions
    addButtonP[0] = productTypeCB;
    addButtonP[1] = productNameEntry;
    addButtonP[2] = productCostEntry;
    g_signal_connect( GTK_BUTTON( productPayDateButton ), "clicked", G_CALLBACK( productPayDateButtonAction ), NULL );
    g_signal_connect( GTK_BUTTON( addButton ), "clicked", G_CALLBACK( addButtonAction ), addButtonP );
    gtk_widget_set_size_request( graphBoxArea, WIDTH, HEIGHT - 80 );
    g_signal_connect( G_OBJECT( graphBoxArea ), "draw", G_CALLBACK( graphBoxAreaAction ), NULL );

    //Показывать все виджеты
    gtk_widget_show_all( window );

    //Передача управления GTK+
    gtk_main(  );

    return 0;
}

/*
====================
void productPayDateButtonAction( GtkButton *, gpointer )

    This function change the year, the month, the day to those that the user chose
====================
*/
void productPayDateButtonAction( GtkButton *button, gpointer data ) {
    //Local variables
    GtkWidget *dateDialog;
    GtkWidget *dateArea;
    GtkWidget *date;
    struct tm  localTime;

    //Function logic
    memset( &localTime, 0, sizeof( localTime ) );

    //Creating objects
    dateDialog = gtk_dialog_new_with_buttons( "Change Date",
                                              NULL,
                                              GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                              "_OK",
                                              GTK_RESPONSE_ACCEPT,
                                              NULL );
    dateArea = gtk_dialog_get_content_area( GTK_DIALOG( dateDialog ) );
    date = gtk_calendar_new(  );
    gtk_calendar_get_display_options( GTK_CALENDAR( date ) );

    //Placing objects
    gtk_container_add( GTK_CONTAINER( dateArea ), date );

    gtk_widget_show( date );
    gtk_dialog_run( GTK_DIALOG( dateDialog ) );

    gtk_calendar_get_date( GTK_CALENDAR( date ), &localTime.tm_year, &localTime.tm_mon, &localTime.tm_mday );
    year  = localTime.tm_year;
    month = localTime.tm_mon + 1;
    day   = localTime.tm_mday;

    gtk_widget_destroy( dateDialog );
}

/*
====================
void addButtonAction( GtkButton *, gpointer )

    This function retrieves data entered by the user from widgets
    input: addButtonP[0] = productTypeCB;
           addButtonP[1] = productNameEntry;
           addButtonP[2] = productCostEntry;
====================
*/
void addButtonAction( GtkButton *button, gpointer data ) {
    //Local variables
    gpointer  *data_t           = (gpointer*)data;
    GtkWidget *productTypeCB    = (GtkWidget*)data_t[0];
    GtkWidget *productNameEntry = (GtkWidget*)data_t[1];
    GtkWidget *productCostEntry = (GtkWidget*)data_t[2];
    int        productCost = 0;
    char      *productType;
    char       productName[2048];
    char       buffer[1024];

    //Function logic
    productType = gtk_combo_box_text_get_active_text( GTK_COMBO_BOX_TEXT( productTypeCB ) );
    strncpy( productName, gtk_entry_get_text( GTK_ENTRY( productNameEntry ) ), 1024 );
    strncpy( buffer, gtk_entry_get_text( GTK_ENTRY( productCostEntry ) ), 1024 );
    productCost = atoi( buffer );
    if ( productCost < 0 ) {
        gtk_entry_set_text( GTK_ENTRY( productCostEntry ), "" );
        gtk_entry_set_placeholder_text( GTK_ENTRY( productCostEntry ), "Wrong cost" );
        return;
    }
    gtk_entry_set_placeholder_text( GTK_ENTRY( productNameEntry ), "Subject of purchase" );
    gtk_entry_set_placeholder_text( GTK_ENTRY( productCostEntry ), "Purchase price" );

    if ( !(writeToFile( year, month, day, productType, productName, productCost )) ) {
        printf( "Erorr Write to file\n" );
    }
}

/*
====================
int writeToFile( int, int, int, char *, char *, int );

    
====================
*/
int writeToFile( int year, int month, int day, char *productType, char *productName, int productCost ) {
    //Local variables
    FILE *logFile;

    //Function logic
    if ( ( logFile = fopen( "/home/rom/Projects/PayLog/Pay.log", "a" ) ) == NULL ) {
        printf( "Erorr Open file\n" );
        return -1;
    }
    if ( DEBUG ) {
        printf( "%d.%d.%d: %s %s %d\n", year, month, day, productType, productName, productCost );
    }
    fprintf( logFile, "%d.%02d.%02d: %s %s %d\n", year, month, day, productType, productName, productCost );
    if ( fclose( logFile ) ) { 
        printf( "Erorr Close file\n" );
        return -1;
    }

    return 1;
}


/*
====================


====================
*/
gboolean graphBoxAreaAction( GtkWidget *widget, cairo_t *cr, gpointer data ) {
    //Local variables
    guint   width;
    guint   height;
    double  xc;
    double  yc;
    double  x;
    double  y;
    double  R[PTCOUNT] = {1., 0.1, 0, 1., 1., 1., 0, 0.35, 1.};
    double  G[PTCOUNT] = {0, 0, 1., 0.98, 0, 0.44, 1., 0.17, 0.48};
    double  B[PTCOUNT] = {0, 1., 0.17, 0, 0.87, 0, 0.84, 0, 0.71};
    double  radius;
    double  angleDeg1[PTCOUNT];
    double  angleDeg2[PTCOUNT];
    int     i;
    double  angle1;
    double  angle2;
    double  productCostPerC[PTCOUNT];
    int     maxProductCost;
    int     sumProductCost;
    char    buffer[256];

    //Function logic
    width  = gtk_widget_get_allocated_width( widget );
    height = gtk_widget_get_allocated_height( widget );
    xc = (double)width / 2;
    yc = (double)height / 2;
    radius = RADIUS;
    sumCost( productCostPerC, &maxProductCost, &sumProductCost );
    //Переводим проценты в углы
    angleDeg1[0] = 0;
    angleDeg2[0] = (360 / 100) * productCostPerC[0];
    for ( i = 1; i < PTCOUNT; i++ ) {
        angleDeg1[i] = angleDeg2[i-1];
        angleDeg2[i] = angleDeg2[i-1] + (360. / 100.) * productCostPerC[i];
    }
    if ( angleDeg2[PTCOUNT - 1] < 360 ) {
        angleDeg2[PTCOUNT - 1] = 360;
    }
    sprintf( buffer, "Days to the month's end: %d", leftDays(  ) );
    cairo_set_font_size( cr, 20 );
    cairo_set_source_rgba( cr, 1, 1., 1., 1. );
    cairo_select_font_face( cr, "Times New Roman", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL );
    cairo_move_to( cr, 0, 20 );
    cairo_show_text( cr, buffer );
    sprintf( buffer, "Max cost pay: %d", maxProductCost );
    cairo_move_to( cr, 0, 50 );
    cairo_show_text( cr, buffer );
    sprintf( buffer, "Sum: %d", sumProductCost );
    cairo_move_to( cr, 0, 80 );
    cairo_show_text( cr, buffer );
    cairo_stroke( cr );
    //Draw
    cairo_set_line_width( cr, 7.0 );
    cairo_set_font_size( cr, 20 );
    cairo_select_font_face( cr, "Times New Roman", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL );
    for ( i = 0; i < PTCOUNT; i++ ) {
        angle1 = angleDeg1[i] * (G_PI / 180.0);
        angle2 = angleDeg2[i] * (G_PI / 180.0);
        cairo_arc( cr, xc, yc, radius, angle1, angle2 );
        cairo_get_current_point( cr, &x, &y );
        cairo_set_source_rgba( cr, R[i], G[i], B[i], 1. );
        cairo_fill( cr );
        cairo_stroke( cr );

        cairo_arc( cr, xc, yc, radius+1., angle1, angle1 );
        cairo_line_to( cr, xc, yc );
        cairo_arc( cr, xc, yc, radius+1., angle2, angle2 );
        cairo_fill( cr );
        cairo_line_to( cr, xc, yc );
        cairo_stroke( cr );

        if ( x > width/2 ) {
            cairo_arc( cr, xc, yc, radius + 20., angle1+(angle2-angle1)/2, angle1+(angle2-angle1)/2 );
        } else if ( (x < width/2) && (y < height/2) ) {
            cairo_arc( cr, xc, yc, radius + 50., angle1+(angle2-angle1)/2, angle1+(angle2-angle1)/2 );
        } else {
            cairo_arc( cr, xc, yc, radius + 40., angle1+(angle2-angle1)/2, angle1+(angle2-angle1)/2 );
        }
        cairo_set_source_rgba( cr, 1, 1., 1., 1. );
        sprintf( buffer, "%0.f %%", productCostPerC[i] );
        cairo_show_text( cr, buffer );
        cairo_stroke( cr );
    }

    cairo_fill( cr );

    return FALSE;
}
