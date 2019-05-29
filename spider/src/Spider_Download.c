#include <Spider.h>



int Spider_Download(int filefd,char* url,char * title,char* description)
{
	char t[1024];
	char p[4096];
	char u[1024];
	bzero(t,sizeof(t));
	bzero(p,sizeof(p));
	sprintf(u,"url:%s\r\n",url);
	sprintf(t,"title:%s\r\n",title);
	sprintf(p,"description:%s\r\n",description);
	write(filefd,u,strlen(u));
	write(filefd,t,strlen(t));
	write(filefd,p,strlen(p));
	return 0;
}

