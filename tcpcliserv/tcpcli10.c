#include	"unp.h"
//ipv6 client
#define IPV6_ADDR_M "2001:0db8:0:f101::1"
#define LENGTH 	256  
int main(int argc, char **argv)
{
	int					sockfd;
	struct linger		ling;
	struct sockaddr_in6	servaddr;
	char revbuf[LENGTH]; 
//	if (argc != 2)
//		err_quit("usage: tcpcli <IPaddress>");

	sockfd = Socket(AF_INET6, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin6_family = AF_INET6;
	servaddr.sin6_port = htons(5000);
//	Inet_pton(AF_INET6, IPV6_ADDR_M, &servaddr.sin6_addr);
	servaddr.sin6_addr = in6addr_any;
	printf("IPV6_ADDR_M = %s\n",IPV6_ADDR_M);

	Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

	
    	/* Try to connect the server */
    	while (strcmp(revbuf,"exit") != 0)     // Check remoter command
    	{      
        	memset (revbuf,0,LENGTH);
        int 	num = recv(sockfd, revbuf, LENGTH, 0);

        	switch(num)
        	{
            	case -1:
      			printf("ERROR: Receive string error!\n");
                	close(sockfd);
                	return (0);
                
            	case  0:
                	close(sockfd);
                	return(0);
              
            	default:
                	printf ("OK: Receviced numbytes = %d\n", num);
                	break;
        	}
        
        	revbuf[num] = '\0';
        	printf ("OK: Receviced string is: %s\n", revbuf);
    	}

    	close (sockfd);
	exit(0);
}
