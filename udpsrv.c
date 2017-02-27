#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>

#define SRVPORT 49152

int main()
{
	int s, msglen, cnt;
	socklen_t fromlen;
	in_port_t myport;
	char msg[80];
	struct sockaddr_in myskt, from;

	myport = SRVPORT;

	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	memset(&myskt, 0, sizeof myskt);
	myskt.sin_family = AF_INET;
	myskt.sin_port = htons(myport);
	myskt.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(s, (struct sockaddr *)&myskt, sizeof myskt) < 0){
		perror("bind");
		exit(1);
	}

	for (;;) {
		fromlen = sizeof from;
		if ((cnt = recvfrom(s, msg, sizeof msg, 0,
							(struct sockaddr *)&from, &fromlen)) < 0) {
			perror("recvfrom");
			exit(1);
		}
		msg[cnt] = '\0';
		msglen = strlen(msg);
		if (!strcmp(msg, "exit")) {
			break;
		}
		printf("%d bytes recved: IP %s, port %d, msg %s\n",
			   cnt, inet_ntoa(from.sin_addr),
			   ntohs(from.sin_port), msg);

		if ((cnt = sendto(s, msg, msglen, 0,
						  (struct sockaddr *)&from, fromlen)) < 0) {
			perror("sendto");
			exit(1);
		}
		printf("%d bytes sent\n", cnt);
	}
	printf("EOF received from keyboard\n");
	close(s);
}