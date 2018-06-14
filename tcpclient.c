#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <errno.h>
#include <getopt.h>

#define SADDR struct sockaddr
#define SIZE sizeof(struct sockaddr_in)

int main(int argc, char **argv) {
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
  int fd;
  int nread;
  char buf[bufSize];
  struct sockaddr_in servaddr;
  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket creating");
    exit(1);
  }

  memset(&servaddr, 0, SIZE);
  servaddr.sin_family = AF_INET;
  printf("ip parsing...\n");
  if (inet_pton(AF_INET, ip, &servaddr.sin_addr) <= 0) {
    perror("bad address");
    exit(1);
  }

  servaddr.sin_port = htons(port);
  printf("connecting to server...\n");
  if (connect(fd, (SADDR *)&servaddr, SIZE) < 0) 
  {
    perror("connect");
    exit(1);
  }
  printf("writing to server...\n");
  write(1, "Input message to send\n", 22);
  while ((nread = read(0, buf, bufSize)) > 0) {
    if (write(fd, buf, nread) < 0) {
      perror("write");
      exit(1);
    }
  }

  close(fd);
  exit(0);
}