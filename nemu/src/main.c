#include <stdio.h>
#include <stdlib.h>
#include "monitor/expr.h"

int init_monitor(int, char *[]);
void ui_mainloop(int);
uint32_t expr(char *e, bool *success);

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  int is_batch_mode = init_monitor(argc, argv);

  /* Receive commands from user. */
//  ui_mainloop(is_batch_mode);
  
  /* Compare the expression and the value. */
  char line[65536];
  char exp[65536];
  char val[10];
  int i;
  int p;
  int base;
  int tr_val;
  bool sig;

  FILE *cp = NULL;
  cp = fopen("../tools/gen-expr/input", "r");
  for(i=0; i<100; i++){
	printf("%d\n",i);
	p = 0;
	base = 0;
	tr_val = 0;
	sig = true;
	memset(line, 0, sizeof(line));
	memset(exp, 0, sizeof(exp));
	memset(val, 0, sizeof(val));
	printf("shit\n");
//	fgets(line, 65536, cp);
	printf("%d",sig);
	printf("ass");
	printf("%s\n",line);
	while(line[p]!=' '){
		val[p] = line[p];
		p++;
	}
	line[p]='\0';
	p++;
	base = p;
	while(line[p]!='\0'){
		exp[p-base] = line[p];
		p++;
	}
	exp[p-base] = '\0';
	sscanf(val, "%d", &tr_val);
	if(tr_val == expr(exp, &sig)){
		printf("true\n");
	}else{
		printf("false\n");
	}
  }
  fclose(cp);
  
    /* Receive commands from user. */
  ui_mainloop(is_batch_mode);
 

  return 0;
}
