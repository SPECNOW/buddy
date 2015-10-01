/*
 * print_debug.h
 *
 *  Created on: Mar 2, 2015
 *      Author: Charanpreet
 */

#ifndef PRINT_DEBUG_H_
#define PRINT_DEBUG_H_

#include "sci.h"
#include "stdio.h"
#include "string.h"
#include "stdarg.h"

void print_info( char * module, char * str, ...);
void print_debug( char * module, char * str, ...);
void print_warn( char * module, char * str, ...);
void print_error( char * module, char * str, ...);

#endif /* PRINT_DEBUG_H_ */
