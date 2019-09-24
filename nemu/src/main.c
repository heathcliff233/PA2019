#include <stdio.h>
#include <stdlib.h>
#include "monitor/expr.h"

int init_monitor(int, char *[]);
void ui_mainloop(int);
uint32_t expr(char *e, bool *success);
void init_wp_pool();

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  int is_batch_mode = init_monitor(argc, argv);
  init_wp_pool();
/*  
  char line[65536];
  char exp[65536];
  char val[10];
  int i;
  int p;
  int base;
  int tr_val;
  bool sig;
  
  FILE *fp = NULL;
	printf("NO.%d\n ",i);
	p = 0;
	base = 0;
	tr_val = 0;
	sig = true;
	memset(line, 0, sizeof(line));
	memset(exp, 0, sizeof(exp));
	memset(val, 0, sizeof(val));
	if(fp==NULL){
		perror("invalid\n");
		return (-1);
	}else{
		fgets(line, 65536, fp);
	}
	printf("%s\n",line);
	while(line[p]!=' '){
		val[p] = line[p];
		p++;
	}
	val[p]='\0';
	p++;
	base = p;
	while(line[p]!='\n'){
		exp[p-base] = line[p];
		p++;
	}
	exp[p-base] = '\0';
	sscanf(val, "%d", &tr_val);
	printf("%u  %u\n",tr_val,expr(exp,&sig));
	if(tr_val == expr(exp, &sig)){
		printf("true\n\n");
	}else{
		printf("false\n");
	}
  }
  fclose(fp);

*/

  /* Receive commands from user. */
  ui_mainloop(is_batch_mode);
 

  return 0;
}
