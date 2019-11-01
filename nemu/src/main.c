#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"monitor/expr.h"
int init_monitor(int, char *[]);
void ui_mainloop(int);
/*uint32_t expr(char *e, bool *success);
bool success=true;
char buf[65536];
char *ex;*/
int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  int is_batch_mode = init_monitor(argc, argv);
/*    int len=0;
	int count=0;
	int result=0;
	int com=0;
    FILE *fp=fopen("/home/charlie/ics2019/nemu/src/input","r");
	if(fp==NULL){
		printf("error\n");
	}
    char *p;
	printf("before entering while\n");
    while(1){
	printf("before if\n");
	printf("before memset\n");
	memset(buf,'\0',sizeof(buf));
	printf("before fgets\n");
    p=fgets(buf,6553,fp);
	len=strlen(buf);
	buf[len-1]='\0';
	printf("before strtok\n");
	ex=strtok(buf," ");
	sscanf(ex,"%d",&result);
	ex=strtok(NULL," ");
	printf("before return\n");
	uint32_t ret=expr(ex,&success);
	com=(ret==result);
	printf("%d\n",com);
	++count;
	if(count==100){
		break;
	}
  }
  fclose(fp);*/
  /* Receive commands from user. */
  ui_mainloop(is_batch_mode);

  return 0;
}
