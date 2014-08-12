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
#include <string.h>
#include "file_packet_parser.h"

int file_packet_parser(unsigned char *, int);

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
int file_packet_parser(unsigned char *data, int dataLen)
{
	switch(data[0])
	{
		case START_PACKET:
			memset(&file_property,0,sizeof(struct file_property_t));
			memcpy(&data[1],&file_property,dataLen - 1);
			printf("File %s size %d and block size %d\n",file_property.file_name,file_property.size,file_property.data_packet_size);
			return START_PACKET;
		case START_ACK:
			return START_ACK;
		case DATA_PACKET:
			return DATA_PACKET;
		case DATA_ACK:
			return DATA_ACK;
		case EOF_PACKET:
			return EOF_PACKET;
		case EOF_ACK:
			return EOF_ACK;
		default:
			return FILE_PACKET_UNKNOWN;
	}
	
	return FILE_PACKET_UNKNOWN;
}
