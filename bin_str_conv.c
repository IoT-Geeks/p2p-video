/*****************************************************************************
The MIT License (MIT)

Copyright (c) 2014 IoT-Geeks

Contributors : Meet Shah, Utpal Solanki

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/****************************************************************************
 *
 * NAME:	This functin will accept a pointer to buffer where data 
 * 			in binary form is stored. User will also provide the len
 * 			of data
 *
 * DESCRIPTION:
 *
 * PARAMETERS:      Name            RW  Usage
 * None.
 *
 * RETURNS:		Buffer given in argument will be updated to string form.
 * 				And the length of this newly buffer will be return back
 * 				to user.
 * 				If it is 0, Converstion was a fail
 *
 * NOTES:
 * None.
 ****************************************************************************/
int bin_to_str(unsigned char *bin_buffer, int bin_buffer_len)
{
	if(bin_buffer_len <= 0 || bin_buffer_len > 256)
		return 0;
	
	unsigned char temp_bin_buffer[256];
	memcpy(temp_bin_buffer,bin_buffer,bin_buffer_len);
	
	int i;
	for(i=0;i<bin_buffer_len;i++)
	{
		sprintf(&bin_buffer[i*2],"%02x",temp_bin_buffer[i]);
	}
	
	return (i*2);
}
/****************************************************************************
 *
 * NAME:		This functin will accept a pointer to buffer where data 
 * 				in string form is stored. User will also provide the len
 * 				of data
 *
 * DESCRIPTION:
 *
 * PARAMETERS:      Name            RW  Usage
 * None.
 *
 * RETURNS:		Buffer given in argument will be updated to binary form.
 * 				And the length of this newly buffer will be return back
 * 				to user.
 * 				If it is 0, Converstion was a fail
 * None.
 *
 * NOTES:
 * None.
 ****************************************************************************/

int str_to_bin(unsigned char *str_buffer, int str_buffer_len)
{
	if(str_buffer_len <= 0 || str_buffer_len > 512)
		return 0;
	if(str_buffer_len%2 != 0)
		return 0;
	
	unsigned char temp_2[2]={0};
	int i;
	for(i=0;i<(str_buffer_len/2);i++)
	{
		memcpy(temp_2,&str_buffer[i*2],2);
		str_buffer[i] = strtol(temp_2,NULL,16);
	}
	
	return (i);
}

