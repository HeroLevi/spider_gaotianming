#include <Spider.h>

int main(void)
{
	url_t * u_info;
	int webfd;
	ssl_t * ssl=NULL;
	queue_t  new;
	queue_t  old;
	//const char * tmpurl = "http://img.zcool.cn/community/018a1d57d761f60000012e7ece7fff.png@1280w_1l_2o_100sh.png";
	//char * tmpurl = "https://baike.baidu.com/item/linux/27050?fr=aladdin";
	char* tmpurl = "https://news.sina.com.cn/china/";
	char* tmp;
	Spider_Queue_create(&new,4096);
	Spider_Queue_create(&old,4096);
	Spider_Queue_en(&new,tmpurl);
	Spider_Queue_show(&new);

	while(!Spider_Queue_if_empty(&new))
	{
		tmp = Spider_Queue_de(&new);
		if((u_info = Spider_Analytic_url(tmp))!=NULL){
			printf("[1] Analytical URL Success:\nURL:%s\nDOMAIN:%s\nIP:%s\nPORT:%d\nNAME:%s\nTYPES:%d\n",u_info->url,u_info->domain,u_info->ip,u_info->port,u_info->name,u_info->types);
		}else
			printf("ERROR:Analytical URL Call Failed..\n");
		webfd = Spider_Connect_server(u_info);
		if(u_info->types){
			Spider_Request(u_info,webfd,ssl);
			Spider_Response(webfd,ssl);
		}else{
			ssl = Spider_Openssl_Create(webfd);
			Spider_Request(u_info,webfd,ssl);
			Spider_Response(webfd,ssl);
		}

		char * src = Spider_Create_cache("temp");
		int fd = open("date.html",O_RDWR|O_CREAT,0664);
		Spider_Analytic_html(&new,&old,src,fd);
		Spider_Queue_show(&new);
		Spider_Queue_en(&old,tmp);
	}
	return 0;
}

