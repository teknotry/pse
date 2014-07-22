#include"sys/socket.h"
#include"netinet/in.h"
#include"pthread.h"
#include "fcntl.h"
struct xxx
{
	int id;
	char key[20];
};
int flag=0;
int count=0;
	char sip[100];
	char sport[20];

int next(struct xxx *p)					//Connect to next server start...//
{
	char key[20];
	strcpy(key,p->key);
	int ch;
	int sid,status;
	char data[100];
	struct sockaddr_in s;
	sid=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
		s.sin_family=PF_INET;
		s.sin_port=atoi(sport);
		s.sin_addr.s_addr=inet_addr(sip);
	status=connect(sid,&s,sizeof(struct sockaddr_in));
	if(status==-1)
	{
		printf("Connection to next server fail\n");
		flag++;
		pthread_exit(0);
		
	}
	else
	{
		read(sid,&ch,sizeof(ch));
		printf("%d\n",ch);
		if(ch==0)
		{
		write(sid,key,strlen(key));
		while(1)
		{
			memset(data,0,sizeof(data));
			read(sid,data,sizeof(data));
			printf("%s\n",data);
			if(strcmp(data,"stop")==0)
				break;
			write(p->id,data,strlen(data));
		}
		write(sid,"bye",3);
		}
		else
		{
		//Next server(server 3);

		}

	flag++;
	}
	
	close(sid);

}							//Connect to next server end....//

int own(struct xxx *p)					//Serch in own database start...//
{
	char *get;

	get=search(p->key);
	write(p->id,get,strlen(get));
	flag++;
	pthread_exit(0);
}							//search in own database end...//


int client(int *x)					//Main thread start...//
{
	struct xxx p;
	char key[20];
	pthread_t t1,t2;
	p.id=*x;
	while(1)
	{
	memset(key,0,sizeof(key));
	read(p.id,key,sizeof(key));
	printf("In main thread key: %s\n",key);
	if(strcmp(key,"bye")==0)
	{
		count--;
		pthread_exit(0);
	
	}
	strcpy(p.key,key);
	pthread_create(&t1,0,own,&p);
	pthread_create(&t2,0,next,&p);
	while(flag<2)
	{

	}
	write(p.id,"stop",4);
	flag=0;
	}
	pthread_join(t1,0);
	pthread_join(t2,0);
}							//Main thread ends....//
main(int argc, char *argv[],char *env[])
{
	int sid,nid,len,port=5500,i;
	struct sockaddr_in s;
	char add[100];
	pthread_t t;
	if(argc>2)
	{
		printf("Too Many Parameters\n");
		exit(0);
	}
	if(argc<2)
	{
		printf("Parameter Missing\n");
		exit(0);
	}
	if(strcmp(argv[1],"start")!=0 && strcmp(argv[1],"stop")!=0)
	{
		printf("Invalid parameter :%s\n",argv[1]);
		exit(0);
	}

	sid=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
		s.sin_family=PF_INET;
		s.sin_port=5000;
		s.sin_addr.s_addr=INADDR_ANY;
	bind(sid,&s,sizeof(struct sockaddr_in));
	listen(sid,10);

	len=sizeof(struct sockaddr_in);

	if(strcmp(argv[1],"start")==0)
	{
		printf("Enter IP Address of Next Server:");
		scanf("%s",sip);
		printf("Enter Port number of Next Server :");
		scanf("%s",sport); 
		strcpy(add,sip);
		strcat(add,";");
		strcat(add,sport);	
		printf("PSE is started ...\n");
		daemon(1,0);
		int fd=open("/home/lakshya/pid.txt",O_CREAT|O_TRUNC|O_WRONLY);
		int pid=getpid();
		write(fd,&pid,sizeof(pid));
		

	while(1)
	{
		nid=accept(sid,&s,&len);
		if(count <2)
		{
			count++;
			i=0;
			write(nid,&i,sizeof(i));
			pthread_create(&t,0,client,&nid);
		}
		else
		{
			i=1;
			write(nid,&i,sizeof(i));
			write(nid,add,strlen(add));
		}		
	}
	pthread_join(t,0);
	close(sid);
	close(fd);
      }//if
	if(strcmp(argv[1],"stop")==0)
	{
		int fd=open("/home/lakshya/pid.txt",O_RDONLY);
		int pid;
		read(fd,&pid,sizeof(pid));
		
		printf("PSE is stoped \n");
		kill(pid,9);
		exit(0);
	}

}
