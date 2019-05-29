#include <Spider.h>


int Spider_Analytic_html(queue_t * new,queue_t * old,char* src,int fd)
{
	char title[1024];
	char descrip[4096];
	char * url = NULL;
	char * jsrc = src;
	regex_t treg,areg,dreg;
	regmatch_t tmatch[2];
	regmatch_t amatch[2];
	regmatch_t dmatch[2];
	regcomp(&treg,"<h1 >\\([^<]\\+\\?\\)</h1>",0);
	//regcomp(&dreg,"<meta name=\"description\" content=\"\\([^\"]\\+\\?\"[^>]\\+\\?\\)>",0);
	regcomp(&dreg,">\\([^\"]\\+</a>\\)",0);
	regcomp(&areg,"<a href=\"\\([^\"]\\+\\?.shtml\\)",0);
	while((regexec(&dreg,src,2,dmatch,0))==0)
	{
		bzero(descrip,sizeof(descrip));
		snprintf(descrip,dmatch[1].rm_eo - dmatch[1].rm_so + 1,"%s",src+dmatch[1].rm_so);
		printf("Description: %s\n",descrip);
		src += dmatch[0].rm_eo;
		Spider_Download(fd,url,title,descrip);
	}
	src = jsrc;
	
	while((regexec(&treg,src,2,tmatch,0))==0)
	{
		bzero(title,sizeof(title));
		snprintf(title,tmatch[1].rm_eo - tmatch[1].rm_so + 1,"%s",src+tmatch[1].rm_so);
		printf("Title : %s\n",title);
		src += tmatch[0].rm_eo;
		Spider_Download(fd,url,title,descrip);
	}
	src = jsrc;
	while((regexec(&areg,src,2,amatch,0))==0)
	{
		url = (char *)malloc(sizeof(char)*1024);
		bzero(url,sizeof(char)*1024);
		strncpy(url,src+amatch[1].rm_so,amatch[1].rm_eo - amatch[1].rm_so);
		printf("\n\n\nurl:%s\n",url);
		if((Spider_Queue_notin(new,url))==FALSE && (Spider_Queue_notin(old,url))==FALSE)
			Spider_Queue_en(new,url);
		else
			free(url);
		src += amatch[0].rm_eo;
		Spider_Download(fd,url,title,descrip);
	}
	
	regfree(&areg);
	regfree(&treg);
	regfree(&dreg);
	//unlink("temp");
	//munmap();
	return 0;
}

