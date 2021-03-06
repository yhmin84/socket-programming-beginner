#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <errno.h>
#include <arpa/inet.h>


main (int argc, char* argv[]) {
	int sd;
	struct sockaddr_in s_addr, c_addr;
	char rcvBuffer[BUFSIZ];
	int n_rev;
	int addr_len;

	if (argc != 2) {
		fprintf(stderr, "usage: UDPreader {port}\n");
		exit(-1);
	}

	sd = socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&s_addr, sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr.sin_port = htons(atoi(argv[1]));

	if (bind(sd, (struct sockaddr *) &s_addr, sizeof(s_addr)) != 0) {
		fprintf(stderr, "bind error\n");
		exit(-2);
	}

	while(1) {
		addr_len = sizeof(c_addr);
		if ((n_rev = recvfrom(sd, rcvBuffer, sizeof(rcvBuffer), 0, (struct sockaddr *) &c_addr, &addr_len)) < 0) {
			fprintf(stderr, "recvfrom() error\n");
			exit(-3);
		}

		rcvBuffer[n_rev] = '\0';

		if (!strncmp(rcvBuffer, "quit", 4)) break;

		printf("Broadcasted from %s [%s]\n", inet_ntoa(c_addr.sin_addr), rcvBuffer);

	}
	close(sd);
}
