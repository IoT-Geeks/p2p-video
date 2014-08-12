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
#include <pthread.h>

#include "peer.h"
#include "ip-conv.h"
#include "peer_parser.h"

#define MSG_NOBLOCK 0x01
#define BUFLEN 		2048
#define MSGS 		1	


struct sockaddr_in myaddr, remaddr;
int connection_count=0;
int connection_timeout=0;
int fd, i, slen=sizeof(remaddr);
char buff[BUFLEN];	
int recvlen;		

struct credential_t we,they;


unsigned char conneciton_id_validate(unsigned char *, int);
void connect_to_client();
void *chat_read( void *);
void on_receive(void);
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

int main(void)
{   
	
	printf("\n\r\n\rConnecting ..");
	if ((fd=socket(AF_INET, SOCK_DGRAM, 0))==-1)
		printf("socket created\n");

	memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(stun_client(&we))
	{
		myaddr.sin_port = htons(we.publicPORT);
		if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
			perror("bind failed");
			return 0;
		}       
		on_receive();
	}

	close(fd);
	return 0;
}
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
void on_receive()
{
	 char message[100]={0};
	 printf("Connected\n\r");
	 
	 sprintf(message,"%d-%d-%d-%d-%d",we.publicIP[0],we.publicIP[1],we.publicIP[2],we.publicIP[3],we.publicPORT);
	 printf("========================================================\n\r");
	 printf("\tYour connection ID is %s\n\r",message);	 
	 printf("========================================================\n\r");
	 
	 do {
		 memset(message,0,sizeof(message));
		 printf("Provide connection ID of other peer: ");
		 scanf("%s",(char *) &message);
		 printf("\n\r");
		}while(conneciton_id_validate(message, strlen(message))!=1);
	
	 connect_to_client();
	 
}
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
unsigned char conneciton_id_validate(unsigned char *connectionID, int lenID)
{
	unsigned char *parser = connectionID;
	
	if((parser=strstr(parser,"-")) == NULL)
		return 0;
	if((parser=strstr(parser,"-")) == NULL)
		return 0;
	if((parser=strstr(parser,"-")) == NULL)
		return 0;
	if((parser=strstr(parser,"-")) == NULL)
		return 0;
	
	parser = connectionID;
	
	they.publicIP[0] = atoi(parser);
	parser=strstr(parser,"-");	
	parser+=1;
	
	they.publicIP[1] = atoi(parser);
	parser=strstr(parser,"-");	
	parser+=1;
	
	they.publicIP[2] = atoi(parser);
	parser=strstr(parser,"-");	
	parser+=1;
	
	they.publicIP[3] = atoi(parser);
	parser=strstr(parser,"-");	
	parser+=1;
	
	they.publicPORT = atoi(parser);
	
	return (they.publicIP[0] > 0 &&
			they.publicIP[1] > 0 &&
			they.publicIP[2] > 0 &&
			they.publicIP[3] > 0 &&
			they.publicPORT > 0);
}
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
 void connect_to_client()
 {
	
	unsigned char buf[1024];
	unsigned char r_server[40];
	unsigned char state;
	unsigned int  pingInterval=500, pingCount=0, timeCount=0;
	
	memset((char *) &remaddr, 0, sizeof(remaddr));
	remaddr.sin_family = AF_INET;
	remaddr.sin_port = htons(they.publicPORT);
	
	
	sprintf(r_server,"%d.%d.%d.%d",they.publicIP[0],they.publicIP[1],they.publicIP[2],they.publicIP[3]);
	
	if (inet_aton(r_server, &remaddr.sin_addr)==0) {
		fprintf(stderr, "inet_aton() failed\n");
		exit(1);
	}
	
	
	pthread_t thread_chat;
	pthread_create( &thread_chat, NULL, chat_read, NULL);
	
	
	while(1)
	{
		usleep(100);
		timeCount++;
		
		if(timeCount % pingInterval == 0)
		{
			
			memset(buf,0,sizeof(buf));
			buf[0] = PACKET_PING;
			sendto(fd, buf, 4, 0, (struct sockaddr *)&remaddr, slen);
			
			timeCount = 0;
			connection_timeout++;
			if(connection_timeout > 10)
			{
				printf("Connectin lost !!\n\r");
				exit(0);
			}
		}
				
		struct timeval t_v;
		t_v.tv_sec = 0;  t_v.tv_usec = 1; 
		setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&t_v,sizeof(struct timeval));
		
		recvlen = recvfrom(fd, buf, BUFLEN, 0,NULL,NULL);
        
        if (recvlen > 0) 
        {
			state = peer_parser(buf, recvlen);
			if(state == PACKET_PING)
			{
				memset(buf,0,sizeof(buf));
				buf[0] = PACKET_PONG;
				sendto(fd, buf, 4, 0, (struct sockaddr *)&remaddr, slen);
			}
			else if(state == PACKET_PONG)
			{
				connection_timeout = 0;
			}
			else if(state == PACKET_CHAT)
			{
				printf("Friend: %s\n\r",&buf[4]);
			}
			memset(buf,0,sizeof(buf));
        }
        
        
	}
 }

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
void *chat_read( void *ptr )
{
	unsigned char send_msg[1024]={0};
	unsigned char buf[1024]={0};
	
	printf("Type Something .. Hit  Enter !! Enjoy\n\r");	
	while(1) 
	{
		usleep(100);
		memset(buf,0,sizeof(buf));
		memset(send_msg,0,sizeof(send_msg));

		fgets(send_msg,sizeof(send_msg),stdin);
			
		buf[0] = PACKET_CHAT;
		sprintf(&buf[4],"%s",send_msg);
		if (sendto(fd, buf, 4 + strlen(send_msg), 0, (struct sockaddr *)&remaddr, slen)==-1) {}
		
	}
 
}

