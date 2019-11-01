#include "nemu.h"
#include "memory/memory.h"
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ, nbr, neq, hex, reg, deref, and

  /* TODO: Add more token types */

};
uint32_t isa_reg_str2val(const char *s, bool *success);
static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */
  {"\\-", '-'},           // minus
  {"\\*",'*'},           //multiply
  {"/",'/'},           //divide
  {"\\(",'('},
  {"\\)",')'},           //bracket
  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"==", TK_EQ},         // equal
  {"0[x,X][0-9a-fA-F]+", hex},   //hex numbers
  {"[0-9]+",nbr},       //numbers
  {"[\\$,a,g,r,s,t][0-9]+", reg},
  {"[a,g,r,s,t][a,p]",reg},       //registers
  {"!=", neq},         //not equal
  {"&&", and}         //logic operator and
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;
static Token tokens[65536] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        int j=0;
        switch (rules[i].token_type) {
            case TK_NOTYPE: break;
			case reg:
			case hex:
			case nbr:   
					  for(j=0;j<substr_len;j++) {
						  tokens[nr_token].str[j]=*(substr_start+j);
		                 }
					  tokens[nr_token].str[j]='\0';
			default: tokens[nr_token].type=rules[i].token_type;
					 ++nr_token;
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}
int check_parentheses(int p, int q){
	int i,count=0;
	for(i=p;i<q;++i){
		if(tokens[i].type=='('){
			++count;
           }
		else if(tokens[i].type==')'){
			--count;
		}
		if(count==0) return 0;
	}
	return (count==1)&&(tokens[p].type=='(')&&(tokens[q].type==')');
}
int iscertaintype(int i){
	return tokens[i].type!=TK_NOTYPE && tokens[i].type!=nbr &&tokens[i].type!=reg && tokens[i].type!=hex && tokens[i].type!='(' && tokens[i].type!=')';
}
static struct pri{
	int op;
	int order;
}table[]={
	{and,5},
	{TK_EQ, 4},
	{neq, 4},
	{'+',3},
	{'-',3},
	{'*',2},
	{'/',2},
	{deref,1}
};
int nr_pri=sizeof(table)/sizeof(table[0]);
uint32_t eval(int p, int q, bool *success){
	if(p>q){
		*success=false;
		return 0;
	}
	else if(p==q){
		if(tokens[p].type==nbr){
	      int ret;
	      sscanf(tokens[p].str,"%d",&ret);
		  uint32_t retu=(uint32_t)ret;
		  return retu;
		}
		else if(tokens[p].type==hex){
			int ret=0;
			sscanf(tokens[p].str,"%x",&ret);
			uint32_t retu=(uint32_t)ret;
			return retu;
		}
        else if(tokens[p].type==reg){
			uint32_t ret=isa_reg_str2val(tokens[p].str, success);
			return ret;
		}
	}
	else if(check_parentheses(p,q)==1){
		return eval(p+1,q-1,success);
	}
	else{
		int ope,op_pri;
		op_pri=0; ope=-1;
		int i=0;
		for(i=p;i<=q;i++){
			if(tokens[i].type=='('){
				int c=1;
				while(c>0){
					++i;
					if(tokens[i].type==')'){
						--c;
					}
					else if(tokens[i].type=='('){
						++c;
					}
				}
			}
			else if(iscertaintype(i)){
				int j=0;
				for(j=0;j<nr_pri;j++){
					if(table[j].op==tokens[i].type){
						break;
					}
				}
				if(table[j].order>=op_pri){
					op_pri=table[j].order;
					ope=i;
				}
			}
		}
			uint32_t val1,val2;
			val1=0; val2=0;
			if(tokens[ope].type!=deref){
			  val1=eval(p,ope-1,success);
			}
		   	val2=eval(ope+1,q,success);
			switch(tokens[ope].type){
				case '+': return val1 + val2; break;
				case '-': return val1 - val2; break;
				case '*': return val1 * val2; break;
				case '/': return val1 / val2; break;
				case TK_EQ: return val1==val2; break;
				case neq: return val1!=val2; break;
				case and: return val1 && val2; break;
				case deref: return *(pmem+val2); break;
				default: assert(0);
			}
	}
	return 0;
}
uint32_t expr(char *e, bool *success) {
    *success=true;
   	if (!make_token(e)) {
    *success = false;
    return 0;
  }
 
  /* TODO: Insert codes to evaluate the expression. */
  int i;
  for(i=0;i<nr_token;i++){
	  if(tokens[i].type=='*' && (i==0 || iscertaintype(i-1))){
		  tokens[i].type=deref;
	  }
  }
  uint32_t ret=eval(0,nr_token-1,success);
  return ret;
}
