#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/netlink.h>

#define BUFSIZE 1024
#define NETLINK_USER 31

void error(char *msg) {
  perror(msg);
  exit(1);
}

  struct sockaddr_nl src_addr, dest_addr;
  struct nlmsghdr *nlh = NULL;
  struct iovec iov;
  struct msghdr msg;
  int sockfd2;

int main(int argc, char **argv) {
  int sockfd; /* socket */
  int portno; /* port to listen on */
  int clientlen; /* byte size of client's address */
  struct sockaddr_in serveraddr; /* server's addr */
  struct sockaddr_in clientaddr; /* client addr */
  struct hostent *hostp; /* client host info */
  char buf[BUFSIZE]; /* message buf */
  char *hostaddrp; /* dotted decimal host addr string */
  int optval; /* flag value for setsockopt */
  int n; /* message byte size */
  int slen=sizeof(clientaddr);

  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }
  portno = atoi(argv[1]);

  

  /* server socket */
  sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (sockfd < 0)
    error("ERROR opening server socket");
  
  optval = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, 
         (const void *)&optval , sizeof(int));

  bzero((char *) &serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons((unsigned short)portno);

  if (bind(sockfd, (struct sockaddr *) &serveraddr, 
       sizeof(serveraddr)) < 0) 
    error("ERROR on binding");

  clientlen = sizeof(clientaddr);
  //while (1) {
/* netlink socket */
  sockfd2 = socket(PF_NETLINK, SOCK_RAW, NETLINK_USER);
  if (sockfd2 < 0)
    error("ERROR opening netlink socket");

  memset(&src_addr, 0, sizeof(src_addr));
  src_addr.nl_family = AF_NETLINK;
  src_addr.nl_pid = getpid(); /* self pid */

  bind(sockfd2, (struct sockaddr*)&src_addr, sizeof(src_addr));

  memset(&dest_addr, 0, sizeof(dest_addr));
  dest_addr.nl_family = AF_NETLINK;
  dest_addr.nl_pid = 0; /* For Linux Kernel */
  dest_addr.nl_groups = 0; /* unicast */

  nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(BUFSIZE));
  memset(nlh, 0, NLMSG_SPACE(BUFSIZE));
  nlh->nlmsg_len = NLMSG_SPACE(BUFSIZE);
  nlh->nlmsg_pid = getpid();
  nlh->nlmsg_flags = 0;

//---------------------------------------

   if (recvfrom(sockfd, buf, BUFSIZE, 0, &clientaddr, &slen)==-1)
            error("recvfrom()");
          printf("Received packet from %s:%d\nData: %s\n\n", 
                 inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);
    /* send to kernel */
    strcpy(NLMSG_DATA(nlh), buf);
    iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;

    msg.msg_name = (void *)&dest_addr;
    msg.msg_namelen = sizeof(dest_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    printf("Sending message to kernel\n");
    sendmsg(sockfd2,&msg,0);
    printf("Waiting for message from kernel\n");

    /* Read message from kernel */
    recvmsg(sockfd2, &msg, 0);
    printf("Received message payload: %s\n", (char *)NLMSG_DATA(nlh));
    printf("cat the file:\n");
    system("cat /dev/chardev");
    close(sockfd2); 
    close(sockfd);
    return 0;
}
