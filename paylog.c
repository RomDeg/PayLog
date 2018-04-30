#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "paylog.h"

/*
====================


====================
*/
int sumCost( double *productCostPerC, int *maxProductCost, int *sumProductCost ) {
    //Local variables
    FILE  *logFile;
    int    currentProductCost = 0;
    char   buffer[2048];
    char  *word;
    char   lastWord[256];
    double productCost[9] = {0};

    //Function logic
    maxProductCost[0] = 0;
    sumProductCost[0] = 0;
    if ( ( logFile = fopen( "/home/rom/MyProjects/GTK/Pay.log", "r" ) ) == NULL ) {
        printf( "Erorr Open file\n" );
        return -1;
    }
    while ( !feof(logFile) ) {
        if ( fgets( buffer, 2048, logFile ) ) {
            word = strtok ( buffer, " " );
            while ( word ) {
                word = strtok ( NULL, " " );
                if ( word ) {
                    strncpy( lastWord, word, 256 );
                }
            }
            currentProductCost = atoi( lastWord );

            memset( &lastWord, 0, sizeof( lastWord ) );
            strncat( lastWord, buffer + 12, 6 );
            if ( !strncmp( lastWord, "Produc", 6 ) ) {
                productCost[0] += currentProductCost;
            } else if ( !strncmp( lastWord, "Dining", 6 ) ) {
                productCost[1] += currentProductCost;
            } else if ( !strncmp( lastWord, "Restau", 6 ) ) {
                productCost[2] += currentProductCost;
            } else if ( !strncmp( lastWord, "Househ", 6 ) ) {
                productCost[3] += currentProductCost;
            } else if ( !strncmp( lastWord, "Paymen", 6 ) ) {
                productCost[4] += currentProductCost;
            } else if ( !strncmp( lastWord, "Clothi", 6 ) ) {
                productCost[5] += currentProductCost;
            } else if ( !strncmp( lastWord, "Entert", 6 ) ) {
                productCost[6] += currentProductCost;
            } else if ( !strncmp( lastWord, "For", 3 ) ) {
                productCost[7] += currentProductCost;
            } else if ( !strncmp( lastWord, "Health", 6 ) ) {
                productCost[8] += currentProductCost;
            }

            sumProductCost[0] = sumProductCost[0] + currentProductCost;
            if ( currentProductCost > maxProductCost[0] ) {
                maxProductCost[0] = currentProductCost;
            }
        }
    }
    if ( DEBUG ) {
        printf( "Sum: %d Max: %d\n", sumProductCost[0], maxProductCost[0] );
    }
    analiticCost( sumProductCost[0], productCost, productCostPerC );
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
int analiticCost( int sumProductCost, double *productCost, double *productCostPerC ) {
    //Local variables

    //Function logic
    productCostPerC[0] = (productCost[0] / sumProductCost) * 100;
    productCostPerC[1] = (productCost[1] / sumProductCost) * 100;
    productCostPerC[2] = (productCost[2] / sumProductCost) * 100;
    productCostPerC[3] = (productCost[3] / sumProductCost) * 100;
    productCostPerC[4] = (productCost[4] / sumProductCost) * 100;
    productCostPerC[5] = (productCost[5] / sumProductCost) * 100;
    productCostPerC[6] = (productCost[6] / sumProductCost) * 100;
    productCostPerC[7] = (productCost[7] / sumProductCost) * 100;
    productCostPerC[8] = (productCost[8] / sumProductCost) * 100;
    if (DEBUG) {
        printf( "%.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n",
                                             productCost[0],
                                             productCost[1],
                                             productCost[2],
                                             productCost[3],
                                             productCost[4],
                                             productCost[5],
                                             productCost[6],
                                             productCost[7],
                                             productCost[8] );
        printf( "%.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n", 
                                             productCostPerC[0],
                                             productCostPerC[1],
                                             productCostPerC[2],
                                             productCostPerC[3],
                                             productCostPerC[4],
                                             productCostPerC[5],
                                             productCostPerC[6],
                                             productCostPerC[7],
                                             productCostPerC[8] );
    }
    return 1;
}

/*
====================


====================
*/
int leftDays(  ) {
    //Local variables
    time_t     currentTime;
    char       buffer[100];
    struct tm *timeStruct;
    int        month = 0;
    int        days = 0;
    int        leftDays = 0;

    //Function logic
    currentTime = time( NULL );
    timeStruct = localtime( &currentTime );
    strftime( buffer, 80, "%m", timeStruct );
    month = atoi( buffer );
    switch( month ){
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            days = 31;
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            days = 30;
            break;
        case 2:
            days = 28;
            break;
        default:
            printf( "Erorr Get Month\n" );
            return -1;

    }
    strftime( buffer, 80, "%d", timeStruct );
    leftDays = days - (atoi( buffer ));

    return leftDays;
}
