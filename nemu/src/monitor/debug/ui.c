#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"
#include "cpu/exec.h"
#include "memory.h"
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
void cpu_exec(uint64_t);
void isa_reg_display(void);
void list_watchpoint();
int set_watchpoint(char *e);
bool delete_watchpoint(int NO);
WP *check_watchpoint();
uint32_t vaddr_read(vaddr_t addr,int len);
void register_pmem(paddr_t base);
uint32_t expr(char *e,bool *success);
/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);
static int cmd_info(char *args);
static int cmd_si(char *args);
static int cmd_x(char *args);
static int cmd_p(char *args);
static int cmd_w(char *args);
static int cmd_d(char *args);
static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si","Execute once",cmd_si},
  { "info","printf registers",cmd_info},
  { "x","print memory",cmd_x},
  { "p","evaluate expression",cmd_p},
  { "w", "set watchpoint",cmd_w},
  { "d", "delete watchpoint", cmd_d},
  /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}
static int cmd_si(char *args)
{
 char *arg=strtok(NULL," ");
 if(arg!=NULL){
 int sum;
 sscanf(arg,"%d",&sum);
 cpu_exec(sum);
 }
 else {cpu_exec(1);}
 return 0;
}
static int cmd_info(char *args)
{
	char *arg=strtok(NULL," ");
    if(strcmp(arg,"r")==0){
	isa_reg_display();
	}
	else if(strcmp(arg,"w")==0){
		list_watchpoint();
	}
	return 0;
}
static int cmd_x(char *args)
{
   char *arg=strtok(NULL," ");
   int n;
   sscanf(arg,"%d",&n);
   char *brg=strtok(NULL," ");
   int addr;
   sscanf(brg,"%x",&addr);
   register_pmem(addr);
   while(n>4){
	 printf("0x%08x\n",vaddr_read(addr,4));
     n-=4;
     addr-=4;
     register_pmem(addr);	 
   }
   if(n>0)
   {
      printf("0x%x\n",vaddr_read(addr,n));
   }
   return 0;
}
static int cmd_p(char *args){
	bool success;
	uint32_t val=expr(args,&success);
	if(success){
		printf("%d\n",val);
	}
	else{
		printf("error");
	}
	return 0;
}
static int cmd_d(char *args){
	char *p=strtok(NULL, " ");
	int NO;
	sscanf(p,"%d", &NO);
	delete_watchpoint(NO);
	return 0;
}
static int cmd_w(char *args){
	char *e=strtok(NULL, " ");
	int NO=set_watchpoint(e);
	printf("watchpoint %d has been set\n",NO);
	return 0;
}
void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd);}
  }
}
