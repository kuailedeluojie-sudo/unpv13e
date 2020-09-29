#include	"unp.h"
#undef SERV_PORT
#define SERV_PORT 161	
typedef   signed           int int_fast8_t;
typedef   signed           int int_fast16_t;
typedef   signed           int int_fast32_t;
typedef uint8_t   u8_t;
typedef int8_t    s8_t;
typedef uint16_t  u16_t;
typedef int16_t   s16_t;
typedef uint32_t  u32_t;
typedef int32_t   s32_t;
struct snmp_asn1_tlv
{
  u8_t  type;       /* only U8 because extended types are not specified by SNMP */
  u8_t  type_len;   /* encoded length of 'type' field (normally 1) */
  u8_t  length_len; /* indicates how many bytes are required to encode the 'value_len' field */
  u16_t value_len;  /* encoded length of the value */
};
int
main(int argc, char **argv)
{
	int					sockfd, n,i;
	socklen_t len;
	char mesg[1024];
	struct sockaddr_in	servaddr, cliaddr;

	sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	Bind(sockfd, (SA *) &servaddr, sizeof(servaddr));

while(1)
{
	len = sizeof(cliaddr);
	n = Recvfrom(sockfd,mesg,1024,0,(SA *)&cliaddr,&len);
	for(i = 0;i<n;i++)
	{
		printf("0x%x ",mesg[i]);
	}
printf("\n\n");
	//mesg[n] = 0;
	//printf("mesg = %s n = %d\n",mesg,n);
	
}

//	dg_echo(sockfd, (SA *) &cliaddr, sizeof(cliaddr));
}
