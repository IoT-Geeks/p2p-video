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
#include <stdio.h>
#include "peer_parser.h"

int peer_parser(unsigned char *, int);

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
int peer_parser(unsigned char *data, int dataLen)
{
	if( (data[1] + data[2] + data[3] != 0) || dataLen < 4)
		return PACKET_UNKNOWN;
	
	switch(data[0])
	{
		case PACKET_PING:
			return PACKET_PING;
		case PACKET_PONG:
			return PACKET_PONG;
		case PACKET_CHAT:
			return PACKET_CHAT;
		default:
			return PACKET_UNKNOWN;
	}
	
	return PACKET_UNKNOWN;
}
