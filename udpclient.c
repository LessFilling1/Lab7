#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <getopt.h>

#define SADDR struct sockaddr
#define SLEN sizeof(struct sockaddr_in)

int main(int argc, char **argv) {
  int sockfd, n;
  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr;
  int bufSize = 0;
  int port = 0;
  char ip[255] = {'\0'};
  while (1)
	{
		int current_optind = optind ? optind : 1;

		static struct option options[] = { { "ip", required_argument, 0, 0 },
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
				memcpy(ip, optarg, strlen(optarg) + 1);
				printf("ip = %s\n", ip);
				break;
			case 1:
			    port = atoi(optarg);
				printf("port = %d\n", port);
				break;
			case 2:
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

  
  char sendline[bufSize], recvline[bufSize + 1];

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);
  printf("parsing ip...\n");
  if (inet_pton(AF_INET, ip, &servaddr.sin_addr) < 0) {
    perror("inet_pton problem");
    exit(1);
  }
  printf("creating socket...\n");
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket problem");
    exit(1);
  }
  printf("writing to server...\n");
  write(1, "Enter string\n", 13);

  while ((n = read(0, sendline, bufSize)) > 0) {
    if (sendto(sockfd, sendline, n, 0, (SADDR *)&servaddr, SLEN) == -1) {
      perror("sendto problem");
      exit(1);
    }

    if (recvfrom(sockfd, recvline, bufSize, 0, NULL, NULL) == -1) {
      perror("recvfrom problem");
      exit(1);
    }

    printf("REPLY FROM SERVER= %s\n", recvline);
  }
  close(sockfd);
}