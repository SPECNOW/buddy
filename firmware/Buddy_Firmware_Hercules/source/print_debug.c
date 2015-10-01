/*
 * print_debug.c
 *
 *  Created on: Mar 2, 2015
 *      Author: Charanpreet
 */

#include "print_debug.h"

void print_info( char * module, char * str, ...)
{
	char str_out[256] = {0};
	va_list args;

	strcat(str_out, "INFO: ");
	strcat(str_out, (const char*)module);
	strcat(str_out, ": ");

	va_start(args, str);
	vsprintf(str_out+strlen(str_out), str, args);
	va_end(args);

	strcat(str_out, "\r\n");

	sciSend(scilinREG, strlen(str_out), (uint8*)str_out);
}

void print_warn( char * module, char * str, ...)
{
	char str_out[256] = {0};
	va_list args;

	strcat(str_out, "WARN: ");
	strcat(str_out, (const char*)module);
	strcat(str_out, ": ");

	va_start(args, str);
	vsprintf(str_out+strlen(str_out), str, args);
	va_end(args);

	strcat(str_out, "\r\n");

	sciSend(scilinREG, strlen(str_out), (uint8*)str_out);
}

void print_debug( char * module, char * str, ...)
{
	char str_out[256] = {0};
	va_list args;

	strcat(str_out, "DEBG: ");
	strcat(str_out, (const char*)module);
	strcat(str_out, ": ");

	va_start(args, str);
	vsprintf(str_out+strlen(str_out), str, args);
	va_end(args);

	strcat(str_out, "\r\n");

	sciSend(scilinREG, strlen(str_out), (uint8*)str_out);
}

void print_error( char * module, char * str, ...)
{
	char str_out[256] = {0};
	va_list args;

	strcat(str_out, "EROR: ");
	strcat(str_out, (const char*)module);
	strcat(str_out, ": ");

	va_start(args, str);
	vsprintf(str_out+strlen(str_out), str, args);
	va_end(args);

	strcat(str_out, "\r\n");

	sciSend(scilinREG, strlen(str_out), (uint8*)str_out);
}
