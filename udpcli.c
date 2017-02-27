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
	socklen_t srvlen, fromlen;
	char lbuf[80], msg[80];
	struct sockaddr_in srvskt, from;

	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}
	printf("server IP address: ");
	fgets(lbuf, sizeof lbuf, stdin);

	memset(&srvskt, 0, sizeof srvskt);
	srvskt.sin_family = AF_INET;
	srvskt.sin_port = htons(SRVPORT);
	if (inet_aton(lbuf, &srvskt.sin_addr) < 0) {
		perror("inet_aton");
		exit(1);
	}

	for (;;) {
		printf("message to be sent: ");
		if (fgets(msg, sizeof msg, stdin) == NULL) {
			break;
		}
		msg[strlen(msg) - 1] = '\0';
		msglen = strlen(msg);

		srvlen = sizeof srvskt;
		if ((cnt = sendto(s, msg, msglen, 0,
						  (struct sockaddr *)&srvskt, srvlen)) < 0) {
			perror("sendto");
			exit(1);
		}
		printf("%d bytes sent\n", cnt);

		fromlen = sizeof from;
		if ((cnt = recvfrom(s, msg, sizeof msg, 0,
							(struct sockaddr *)&from, &fromlen)) < 0) {
			perror("recvfrom");
			exit(1);
		}
		msg[cnt] = '\0';
		printf("%d bytes recved: IP %s, port %d, msg %s\n",
			   cnt, inet_ntoa(from.sin_addr),
			   ntohs(from.sin_port), msg);
	}
	printf("EOF received from keyboard\n");
	close(s);
}
