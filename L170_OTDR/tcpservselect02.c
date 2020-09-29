#include	"unp.h"
//这个主要用来连接上位机和本地Linux的一个转接
//用9878端口
//思路就是先接本地Linux发过来的连接，然后等到下一个连接来的
//时候直接用给Linux的套接字发一个指令，指令暂时为momomo
//Linux本地的套接字接到这个指令之后马上建立和otdr 5000端口
//的连接，然后再中转，中间不做任何解析
#define  SERV_PORT1 5000
#define 	NOTDEF 1
 int
main(int argc, char **argv)
{
	int					i, maxi, maxfd, listenfd, connfd, sockfd;
	int					nready, client[FD_SETSIZE];
	ssize_t				n;
	fd_set				rset, allset;
	char				buf[MAXLINE];
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr;
	int reuse = 1;
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
	{
		perror("setsockopet error\n");
		return -1;
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT1);

	printf("port = %d \n",SERV_PORT1);
	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);
	int os_po = -1,cli_po = -1; 
	maxfd = listenfd;			/* initialize */
	maxi = -1;					/* index into client[] array */
	for (i = 0; i < FD_SETSIZE; i++)
		client[i] = -1;			/* -1 indicates available entry */
	FD_ZERO(&allset);
	int flag = 1;
	FD_SET(listenfd, &allset);
	char ip_cli[16] = "";
	for ( ; ; ) {
		rset = allset;		/* structure assignment */
		nready = Select(maxfd+1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(listenfd, &rset)) {	/* new client connection */
			clilen = sizeof(cliaddr);
			connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);
			if(flag ==1 )
			{
				os_po = connfd;
				flag = 0;
			}
			/* //之前想的设定的指令
			   else if (os_po != connfd)
			   {
			   send(connfd,"momomo",strlen("momomo"),0);	
			   }
			   */
#ifdef	NOTDEF
			inet_ntop(AF_INET, &cliaddr.sin_addr, ip_cli, ((socklen_t)sizeof(ip_cli))),
				printf("new client: %s, port %d\n",ip_cli,
						ntohs(cliaddr.sin_port));
#endif

			for (i = 0; i < FD_SETSIZE; i++)
				if (client[i] < 0) {
					client[i] = connfd;	/* save descriptor */
					break;
				}
			if (i == FD_SETSIZE)
				err_quit("too many clients");

			FD_SET(connfd, &allset);	/* add new descriptor to set */
			if (connfd > maxfd)
				maxfd = connfd;			/* for select */
			if (i > maxi)
				maxi = i;				/* max index in client[] array */

			if (--nready <= 0)
				continue;				/* no more readable descriptors */
		}

		for (i = 0; i <= maxi; i++) 	/* check all clients for data */
		{
			if(client[i] > 0 && client[i] != os_po)
			{
				cli_po = client[i];
			}
		}
		for (i = 0; i <= maxi; i++) {	/* check all clients for data */
			if ( (sockfd = client[i]) < 0 )
				continue;
			if(cli_po < 0)
				continue;
			if (FD_ISSET(sockfd, &rset)) {
				if ( (n = Read(sockfd, buf, MAXLINE)) == 0) {
					/*4connection closed by client */
					Close(sockfd);
					printf("close sockfd = %d\n",sockfd);
					FD_CLR(sockfd, &allset);
					client[i] = -1;
				} else
				{
					//2019.7.22 添加 打印接收到的信息
					//这里假设只有两个tcp客户端连接，分辨出客户端
					//分别进行通信
					printf("\n");
					printf("\n");
					buf[n] = 0;
					printf("maxi = %d\n",maxi);
					if(sockfd == os_po )
					{
						//	来自第一个连接
						printf("11111111\n");
						printf("往 %d 写\n",cli_po);
						printf("buf:%s \n",buf);
						Writen(cli_po, buf, n);
					}
					else if(maxi > 0) 
					{

						printf("2222222\n");
						printf("往 %d 写\n",os_po);
						printf("buf:%s \n",buf);
						Writen(os_po, buf, n);
					}
					//Writen(sockfd, buf, n);
				}
				if (--nready <= 0)
					break;				/* no more readable descriptors */
			}
		}
	}
}
/* end fig02 */
