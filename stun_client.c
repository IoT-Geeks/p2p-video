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
#include <netdb.h>
#include <sys/socket.h>

#include "stun_client.h"
#include "peer.h"

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
int stun_client(struct credential_t *localRequest)
{
	struct sockaddr_in my_addr, stun_addr;
	unsigned char buf[1024];
	int stun_fd;
	int recvlen;
	int socket_len = sizeof(struct sockaddr_in);
	
	if ((stun_fd = socket(AF_INET, SOCK_DGRAM, 0))==-1)
		printf("socket created\n");

	memset((char *)&my_addr, 0, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	my_addr.sin_port = htons(0);

	if (bind(stun_fd, (struct sockaddr *)&my_addr, sizeof(my_addr)) < 0) {
		perror("bind failed");
		return 0;
	}       


	memset((char *) &stun_addr, 0, sizeof(stun_addr));
	stun_addr.sin_family = AF_INET;
	stun_addr.sin_port = htons(STUN_SERVER_PORT);
	
	if (inet_aton(STUN_SERVER, &stun_addr.sin_addr)==0) {
		fprintf(stderr, "inet_aton() failed\n");
		exit(1);
	}

	sprintf(buf, "0001000801e896640ad1bf1a5bdb8b22630bd5660003000400000000");
	unsigned char dlen = str_to_bin(buf,strlen(buf));
	

	if (sendto(stun_fd, buf, dlen, 0, (struct sockaddr *)&stun_addr, socket_len)==-1) {}
	
	while(1) 
	{
		usleep(100);
		
		struct timeval t_v;
		t_v.tv_sec = 0;  t_v.tv_usec = 1; 
		setsockopt(stun_fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&t_v,sizeof(struct timeval));
		
		recvlen = recvfrom(stun_fd, buf, 1024, 0,NULL,NULL);
        if (recvlen > 20) 
        {
			struct stun_bind_response_t *stun_resp;
			
			stun_resp = (struct stun_bind_response_t *)buf;
			
			localRequest->publicPORT = (stun_resp->attr_mapped[6] << 8) + stun_resp->attr_mapped[7];
			localRequest->publicIP[0] = stun_resp->attr_mapped[8];
			localRequest->publicIP[1] = stun_resp->attr_mapped[9];
			localRequest->publicIP[2] = stun_resp->attr_mapped[10];
			localRequest->publicIP[3] = stun_resp->attr_mapped[11];
			
			close(stun_fd);
			
			return 1;
			
		}
       
	}
	close(stun_fd);
}
