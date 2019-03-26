/*
 * file: lapcount.c
 * Lap Count Server implementation
 */

 /* Note:
  * IPv4-mapped IPv6 addresses will appear in
  * string format with a leading ::ffff:, e.g. ::ffff:203.0.113.47.
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

struct sockaddr_in6 servaddr6;
struct sockaddr_in6 cliaddr6;

void init_socket() {
	int no = 0;		//
	// Creating socket file descriptor
	if ( (sockfd = socket(AF_INET6, SOCK_DGRAM, 0)) < 0 ) {
		perror("ipv6 socket creation failed");
		exit(EXIT_FAILURE);
	}
	if (setsockopt(sockfd, IPPROTO_IPV6, IPV6_V6ONLY, (char *)&no, sizeof(no)) < 0) {
         perror("setsockopt(IPPROTO_IPV6, IPV6_V6ONLY) failed");
         exit(EXIT_FAILURE);
    }

	memset(&servaddr6, 0, sizeof(servaddr6));
	memset(&cliaddr6, 0, sizeof(cliaddr6));

	// Filling server information
	servaddr6.sin6_family = AF_INET6; // IPv4
	servaddr6.sin6_addr = in6addr_any;
	servaddr6.sin6_port = htons(PORT);

	// Bind the socket with the server address
	if ( bind(sockfd, (const struct sockaddr *)&servaddr6,
			sizeof(servaddr6)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
}

/*
 * check address if it is a mapped IPV4 address
 * returns 1 for IPV4, 0 for IPV6
 */
int is_ipv4(struct sockaddr_in6 *s) {
	struct in6_addr *a = &s->sin6_addr;
	for (int i = 0; i < 10; i++) {
		if (a->__in6_u.__u6_addr8[i] != 0) return 0;
	}
	if (a->__in6_u.__u6_addr8[10] != 0xFF) return 0;
	if (a->__in6_u.__u6_addr8[11] != 0xFF) return 0;
	return 1;
}

/*
 * retrieve client IP in char format
 * handles IPV6 and IPV4 mapped addresses
 */
void get_client_ip(struct sockaddr_in6 *sa, char *ip_str, int str_len) {
	char str[128];
	//int len = 128;
	struct in6_addr *a = &sa->sin6_addr;
    if (is_ipv4(&cliaddr6) ) {
		sprintf(str, "%d.%d.%d.%d", a->__in6_u.__u6_addr8[12],a->__in6_u.__u6_addr8[13],
				a->__in6_u.__u6_addr8[14], a->__in6_u.__u6_addr8[15]);
	} else {
		inet_ntop(AF_INET6, &a, ip_str, str_len);
	}

	if (strlen(str) < str_len) {
		strcpy(ip_str, str);
	} else {
		strncpy(ip_str, str, str_len);
	}
}

/*
 * wait for client data packet and append \n
 */
void wait_for_client_packet() {
	unsigned int len, n;
	len = sizeof(cliaddr6);
	n = recvfrom(sockfd, (char *)rx_buffer, RXBUFSIZE,
				MSG_WAITALL, ( struct sockaddr *) &cliaddr6, &len);
	rx_buffer[n] = '\0';
}

int main() {

    //init_socket();
	init_socket();

	wait_for_client_packet();

	// retrieve client IP address
	char client_ip[30];
	get_client_ip( &cliaddr6, &client_ip[0], 30 );

	printf("%s : %s\n", client_ip, rx_buffer);

	/*
	sendto(sockfd, (const char *)hello, strlen(hello),
		MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
			len);
	printf("Hello message sent.\n");
	*/

	return 0;
}
