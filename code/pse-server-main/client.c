#include"sys/socket.h"
#include"netinet/in.h"
main()
{
	int sid,status,k,port=5000;
	int ch,i=0,j=0,l=0;
	char key[20];
	char data[100];
	char add[20]="192.168.5.150";
	char dev[2][20];
	struct sockaddr_in s;
	while(1)
	{

	sid=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
		s.sin_family=PF_INET;
		s.sin_port=port;
		s.sin_addr.s_addr=inet_addr(add);
	
	status=connect(sid,&s,sizeof(struct sockaddr_in));
	if(status==-1)
	{
		printf("Connection failure..\n");
		exit(0);
	}
	else
	{
		read(sid,&ch,sizeof(ch));
		if(ch==0)
		{
			while(1)
			{
				printf("Enter the key: ");
				scanf("%s",key);	
				write(sid,key,strlen(key));
				if(strcmp(key,"bye")==0)
					exit(0);
				memset(key,0,sizeof(key));
				while(1)
				{
					memset(data,0,sizeof(data));
					read(sid,data,sizeof(data));
					if(strcmp(data,"stop")==0)
						break;
					printf("%s\n",data);
				}
			}
		}
		else
		{
			memset(add,0,sizeof(add));
			read(sid,add,sizeof(add));
			memset(dev,0,sizeof(dev));
			for(i=0;i<strlen(add);i++)
			{
				if(add[i]!=';')
				{
					dev[j][l]=add[i];
					l++;
				}
				else
				{
					dev[j][l]=0;
					j=1;
					l=0;
				}
			}
			port=atoi(dev[1]);
			strcpy(add,dev[0]);
			printf("%s\n",add);
			printf("%d\n",port);
		}
	
	}
	
	close(sid);
	}

}
