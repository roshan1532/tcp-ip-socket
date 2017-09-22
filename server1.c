
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<errno.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>


#define ERROR -1
#define AVAIL 4
#define MAX_CLIENTS 4
#define MAX_DATA 1024
struct prod_avail{
   int PC;
   char pname[26];
   int price;
};

struct prod_avail product[AVAIL];
int total;
int grand_total=0;

void fun(char data[],char ret[])
{
		  //printf("request=%s",data);
		  int p,q,r;
		  char a[26];
		  int i=0;
		  int k=0;
		  int c=0;
		 // printf(" length=%d\n",strlen(data));
		  while(i<strlen(data)-1)
		  {
			    c++;
			    k=0;
			    while(i<strlen(data)-1&&data[i]!=' ')
			    {
			      a[k]=data[i];
			      i++;
			      k++;
		           }
			    a[k]='\0';
			    if(c==1)
			    p=atoi(a);
			    else if(c==2)
			    q=atoi(a);
			    else
			    r=atoi(a);
			    i++;
			//printf("%d %d %d \n",p,q,r);   
		  }
		// printf("type=%d pc=%d number=%d\n",p,q,r);
		  //strcat(ret,"roshan");
		   if(p==1)
		     {
			sprintf(a, "%d", total);
			strcat(ret,"<OK><total=");
		        strcat(ret,a);
		        strcat(ret,">");
		        //printf("return=%s\n",ret);
		        return;
		     }
		   else if(p==0)
		   for(i=0;i<AVAIL;i++)
		     {
		       if(product[i].PC==q)
		       {
			 total=total+product[i].price*r;
			 grand_total=grand_total+product[i].price*r;
			 sprintf(a, "%d", product[i].price);
			 strcat(ret,"<OK><PRICE:"); strcat(ret,a); strcat(ret,">");  strcat(ret,"<NAME:");  strcat(ret,product[i].pname);  strcat(ret,">");
			 return;
		       }
		     }
		   if(i==AVAIL)
		      {
		       strcat(ret,"<error><PC NOT IN DATABASE>");
		       return ;
		      
		      }
		    else
		    {
		     strcat(ret,"<error><protocol error>");
		     return;
		    
		    }
}
///////////////////////////////////////////////////////////////////////
int main(int argc ,char *argv[])
{

  
   int i;
   FILE *fp=fopen("product.txt","r");
   for(i=0;i<4;i++)
   {
     fscanf(fp,"%d %s %d",&product[i].PC,product[i].pname,&product[i].price);
   
   }
   fclose(fp);
  printf("products which are available in database\n");
  for(i=0;i<4;i++)
    printf("%d %s %d\n",product[i].PC,product[i].pname,product[i].price);
    printf("...............................................................\n");
    
   struct sockaddr_in server;
   struct sockaddr_in client;
   int sock;
   int new;
   int sockaddr_len=sizeof(struct sockaddr_in);
   int data_len;
   char data[MAX_DATA];
   char ret[MAX_DATA];
  
   
   if((sock=socket(AF_INET ,SOCK_STREAM,0))==ERROR)
      {
        perror("socket: ");
        exit(-1);
        
      }
      server.sin_family=AF_INET;
      server.sin_port=htons(atoi(argv[1]));
      server.sin_addr.s_addr=INADDR_ANY;  //LISTEN ON ALL INTERFACES ON LOCAL MACHINE
      bzero(&server.sin_zero,8);
      
      
  if((bind(sock ,(struct sockaddr *)&server,sockaddr_len)==ERROR))
     {
       perror("bind: ");
       exit(-1);
     }    
   
   if(listen(sock,MAX_CLIENTS)==ERROR)
   { 
      perror("LISTEN: ");
       exit(-1);
   }
  while(1)
  {
     if((new=accept(sock ,(struct sockaddr*)&client,&sockaddr_len))==ERROR)
      {
        printf("ACCEPT:");
        exit(-1);
      }
      total=0;
     // printf("new client connected to port no . %d and IP %s\n",ntohs(client.sin_port),inet_ntoa(client.sin_addr));
      printf("new customer send a request :\n");
      data_len=1;
      while(data_len)
      {
       ret[0]='\0';
       data_len=recv(new ,data,MAX_DATA ,0);
      // printf("data_len=%d\n",data_len);
       if(data_len)
       {
        data[data_len]='\0';
        printf("request:%s",data);
        fun(data,ret);

         send(new ,ret,strlen(ret),0);
      
         printf("total purchase by this custoer so far %d\n",total);
         printf("grand_total so far=%d\n",grand_total);
       }
      
      }
     printf("total purchasing by this customer=%d\n",total);
     printf("customer left\n");
     close(new);
  
  } 
  close(sock);
   return 0;
}
