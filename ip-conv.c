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
#include "ip-conv.h"

/****************************************************************************
 *
 * NAME:
 *
 * DESCRIPTION:
 *
 * PARAMETERS:      Name            RW  Usage
 * None.
 *
 * RETURNS:
 * None.
 *
 * NOTES:
 * None.
 ****************************************************************************/
unsigned char ip_str_to_byte(unsigned char *addrstr, unsigned char *ipaddr)
{
	unsigned char tmp;
	char c;
	unsigned char i, j;

	tmp = 0;

	for(i = 0; i < 4; ++i) 
	{
		j = 0;
		do
		{
			c = *addrstr;
			++j;
			if(j > 4) 
			{
				return 0;
			}
			if(c == '.' || c == 0) 
			{
				*ipaddr = tmp;
				++ipaddr;
				tmp = 0;
			} 
			else if(c >= '0' && c <= '9') 
			{
				tmp = (tmp * 10) + (c - '0');
			} 
			else 
			{
				return 0;
			}
			++addrstr;
			
		} while(c != '.' && c != 0);
	}
	return 1;
}
