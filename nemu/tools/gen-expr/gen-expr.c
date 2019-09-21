#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536];

static int idx;

uint32_t choose(uint32_t n){
	uint32_t ran = rand()%n;
	return ran;
}

static inline void gen_num(){
	int nu = rand()%10;
	char pu = nu + '0';
	buf[idx] = pu;
	idx++;
}

static inline void gen_rand_op(){
	int nu = random()%4;
	char lst[] = {'+','-','*','/'};
	char pu = lst[nu];
	buf[idx] = pu;
	idx++;
}

static inline void gen(char sig){
	buf[idx] = sig;
	idx++;
}

static inline void gen_rand_expr() {
//  buf[0] = '\0';
  idx = 0;
  switch(choose(3)){
	case 0 : gen_num; break;
	case 1 : gen('('); gen_rand_expr(); gen(')'); break;
	
	default : gen_rand_expr(); gen_rand_op(); gen_rand_expr(); break;
  }
}

static char code_buf[65536];
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    gen_rand_expr();
/*  add an end here  */	
    buf[idx] = '\0';
    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
