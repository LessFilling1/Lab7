#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <getopt.h>

#define SADDR struct sockaddr
#define SLEN sizeof(struct sockaddr_in)

int main(int argc, char **argv) 
{
  int sockfd, n;
  int bufSize = 0;
  int port =0;
  while (1)
	{
		int current_optind = optind ? optind : 1;

		static struct option options[] = {
		{ "port", required_argument, 0, 0 },
		{ "buf", required_argument, 0, 0 },
		{ 0, 0, 0, 0 } };

		int option_index = 0;
		int c = getopt_long(argc, argv, "", options, &option_index);

		if (c == -1)
			break;

		switch (c)
		{
		case 0:
		{
			switch (option_index)
			{
			case 0:
			    port = atoi(optarg);
				printf("port = %d\n", port);
				break;
			case 1:
			    bufSize = atoi(optarg);
				printf("bufSize = %d\n", bufSize);
				break;
			default:
				printf("Index %d is out of options\n", option_index);
			}
		} break;

		case '?':
			printf("Arguments error\n");
			break;
		default:
			fprintf(stderr, "getopt returned character code 0%o?\n", c);
		}
	}
  char mesg[bufSize], ipadr[16];
  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr;
  
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket problem");
    exit(1);
  }

  memset(&servaddr, 0, SLEN);
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port);
  printf("binding...\n");
  if (bind(sockfd, (SADDR *)&servaddr, SLEN) < 0) {
    perror("bind problem");
    exit(1);
  }
  printf("SERVER starts...\n");
  while (1) {
    unsigned int len = SLEN;

    if ((n = recvfrom(sockfd, mesg, bufSize, 0, (SADDR *)&cliaddr, &len)) < 0) {
      perror("recvfrom");
      exit(1);
    }
    mesg[n] = 0;

    printf("REQUEST %s      FROM %s : %d\n", mesg,
           inet_ntop(AF_INET, (void *)&cliaddr.sin_addr.s_addr, ipadr, 16),
           ntohs(cliaddr.sin_port));

    if (sendto(sockfd, mesg, n, 0, (SADDR *)&cliaddr, len) < 0) {
      perror("sendto");
      exit(1);
    }
  }
}