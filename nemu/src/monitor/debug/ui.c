#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"
#include "cpu/exec.h"
#include "nemu.h"
#include "memory/memory.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);

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

static int cmd_si(char *args);

static int cmd_info(char *args);

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
  { "si", "Exec once, pass an integer to circle, default to 1", cmd_si },
  { "info", "Print out the desired information", cmd_info },
  { "x", "Scan the memory", cmd_x },
  { "p", "process the expression", cmd_p},
  { "w", "insert a watchpoint", cmd_w},
  { "d", "delete a watchpoint", cmd_d},
  /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

/* start of cmd_si */

vaddr_t exec_once(void);

static int cmd_si(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int circle ;
	if (arg == NULL) {
		/* no argument given */
        cpu_exec(1);
	}else{
		circle = atoi(arg);
		if (circle <= 0){
			printf("fuck you test bitch");
			return 0;
		}
		cpu_exec(circle);

	}
	return 0;
}

/* end of cmd_si */

/* start of cmd_info */

void isa_reg_display();

static int cmd_info(char *args){
	/* extract argument */
	char *arg = strtok(NULL, " ");
	if(arg == NULL){
		printf("Pass an arg you little bitch");
	}else if(strcmp(arg , "r")==0){
		isa_reg_display();
	}else if(strcmp(arg , "w")==0){
		list_watchpoints();
	}else{
	    printf("Invalid arg, try -r or -w instead");
	}
	return 0;
}

/*  end of cmd_info */

/*	start of cmd_p */

static int cmd_p(char *args){
	if(args == NULL){
		printf("no args");
		return 0 ;
	}

	bool success=true ;
	uint32_t val = expr(args, &success);
	if(!success){
		printf("invalid expression");
	}else{
		printf("%d\n",val);
	}
	return 0 ;
}

/*	end of cmd_p */

/*  start of cmd_x */

uint32_t vaddr_read(vaddr_t addr, int len);

static int cmd_x(char *args){
    char *num = strtok(NULL, " ");
	int nu = atoi(num);
	args = strtok(NULL, " ");
	int addre = 0;
//	sscanf(args,"%x",&addre);
	bool success = true;
	addre = expr(args,&success);

    register_pmem(addre);
	while(nu > 4){
	    printf("0x%08x\n",vaddr_read(addre,4));
		nu-=4;
	}
	if(nu>0){
	    printf("0x%x\n",vaddr_read(addre,nu));
	}
	return 0;    
}

/*  end of cmd_x */

/*  start of cmd_w */

static int cmd_w(char *args){
	args = strtok(NULL," ");
	if(args == NULL){
		printf("please enter an expression\n");
		return 0;
	}

	int NO = set_watchpoint(args);
	if(NO == -1){
		printf("invalid expression\n");
		return 0;
	}
	printf("set watchpoint %d\n", NO);
	return 0;

}

/*  end of cmd_w  */

/*	start of cmd_d  */

static int cmd_d(char *args){
	args = strtok(NULL, " ");
	if(args==NULL){
		printf("give the id");
	}
	int NO = 0;
	sscanf(args, "%d", &NO);
	bool res = del_watchpoint(NO);
	if(res){
		printf("successfully delete watchpoint NO.%d\n",NO);
		return 0;
	}else{
		printf("invalid expression");
		return 0;
	}
}

/*	end of cmd_d  */

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

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
