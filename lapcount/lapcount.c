/*
 * file: lapcount.c
 * Lap Count Server implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT	 2006
#define RXBUFSIZE 1024

char rx_buffer[RXBUFSIZE];
int sockfd;

struct sockaddr_in servaddr;
struct sockaddr_in cliaddr;

/*
void get_client_ip () {
	char *s = NULL;
switch(res->sa_family) {
    case AF_INET: {
        struct sockaddr_in *addr_in = (struct sockaddr_in *)res;
        s = malloc(INET_ADDRSTRLEN);
        inet_ntop(AF_INET, &(addr_in->sin_addr), s, INET_ADDRSTRLEN);
        break;
    }
    case AF_INET6: {
        struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *)res;
        s = malloc(INET6_ADDRSTRLEN);
        inet_ntop(AF_INET6, &(addr_in6->sin6_addr), s, INET6_ADDRSTRLEN);
        break;
    }
    default:
        break;
}
printf("IP address: %s\n", s);
free(s);
}
*/

void init_socket() {
	// Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));

	// Filling server information
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);

	// Bind the socket with the server address
	if ( bind(sockfd, (const struct sockaddr *)&servaddr,
			sizeof(servaddr)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

}

void wait_for_client_packet() {
	int len, n;
	n = recvfrom(sockfd, (char *)rx_buffer, RXBUFSIZE,
				MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
	rx_buffer[n] = '\0';
}

int main() {

    init_socket();

	wait_for_client_packet();

	// retrieve client IP address
	char *client_ip = inet_ntoa(cliaddr.sin_addr);
	printf("%s : %s\n", client_ip, rx_buffer);
	/*
	sendto(sockfd, (const char *)hello, strlen(hello),
		MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
			len);
	printf("Hello message sent.\n");
	*/

	return 0;
}
