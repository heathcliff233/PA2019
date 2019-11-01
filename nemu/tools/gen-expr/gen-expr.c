#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
// this should be enough
static char buf[65536];
static int idx;
uint32_t choose(uint32_t n) {	
   	uint32_t ran=(rand()%n);
	return ran;
 }

static inline void gen_num(){
    extern int idx;
   	int n=choose(9);
	char c=n + '1';
	buf[idx++]=c;
}
static inline void gen(char c){
	extern int idx;
	buf[idx++]=c;
}

static inline void gen_rand_op(){
   extern int idx;
   	char c;
   	switch(choose(4)){
	   case 0: c='+'; buf[idx++]=c; break;
	   case 1: c='-'; buf[idx++]=c; break;
	   case 2: c='*'; buf[idx++]=c; break;
	   default: c='/'; buf[idx++]=c; break;
   }
}
static inline void gen_rand_expr() {
	extern int idx;
	switch(choose(3)){
		case 0: gen('('); gen_rand_expr(); gen(')'); break;
		case 1: gen_num(); break;
		default:{
				   	gen_rand_expr();
				   	gen_rand_op();
				    if(buf[idx-1]=='/'){
						gen_num(); break;
					}
					else{
                        gen_rand_expr();break;
					}
	}
  }
}
static char code_buf[65536];
static char *code_format =
"#include <stdio.h>\n"
" int main() {"
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
    idx=0;
	memset(buf,0,sizeof(buf));
    memset(code_buf,0,sizeof(code_buf));
	gen_rand_expr();
    buf[idx]='\0';	
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
