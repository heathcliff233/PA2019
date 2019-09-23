#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
  NOTYPE = 256, NUM, HEX, REG, EQ, NEQ, AND, OR, DEREF, NEG 

  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */
  {"\\/", '/'},         // division
  {"\\-", '-'},         // minus
  {"\\*", '*'},         // multiply
  {" +", NOTYPE},      // spaces
  {"\\+", '+'},       // plus
  {"\\(", '('},          // left parentheses
  {"\\)", ')'},          // right parenttheses
  {"==", EQ},           // equal
  {"!=", NEQ},          // not equal
  {"0[xX][0-9a-fA-F]+", HEX},  
  {"&&", AND},
  {"[0-9]+", NUM},
  {"\\|\\|", OR},
  {"[\\$,a,g,r,s,t][a-z0-9]+", REG}  //register
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

static struct Node {
  int oprat ;
  int prior ;
}op_table[] = {
	{DEREF, 2},
	{NEG, 2},
	{'*', 3},
	{'/', 3},
	{'+', 4},
	{'-', 4},
	{EQ, 7},
	{NEQ, 7},
	{AND, 9},
	{OR, 9},
};

int len_optb = sizeof(op_table) / sizeof(op_table[0]);

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
		
		int j ;
        switch (rules[i].token_type) {
			case NOTYPE : break ;
			case REG :
			case HEX :
			case NUM :
						  for(j = 0 ; j < substr_len ; j++){
						      tokens[nr_token].str[j] = *(substr_start+j);
						  }
						  tokens[nr_token].str[j]='\0';

            default:
						  tokens[nr_token].type = rules[i].token_type;
						  nr_token++ ;
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

uint32_t eval(int p, int q, bool *success);

bool is_op(int i);

uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  for(int i=0; i<nr_token; i++){
	if(tokens[i].type=='*' && (i==0 || is_op(i-1))){
		tokens[i].type = DEREF;
	}

	else if(tokens[i].type=='-' && (i==0 || is_op(i-1))){
		tokens[i].type = NEG;
	}
  }

  return eval(0, nr_token-1, success);
}

bool check_parentheses(int p, int q);
bool is_op(int i){
	return (tokens[i].type!=NOTYPE)&&(tokens[i].type!=NUM)&&(tokens[i].type!=HEX)&&(tokens[i].type!='(')&&(tokens[i].type!='(');
}

uint32_t isa_reg_str2val(const char *s, bool *success);

uint32_t eval(int p, int q, bool *success){
	if (p > q){
		*success = false ;
		printf("p:%d q%d\n",p,q);
		printf("false expression fuck you\n");
		return 0 ;
	}
	else if(p == q){
		uint32_t value=0 ;
		if(tokens[p].type==NUM){
			sscanf(tokens[p].str , "%d" , &value);
			return value ;
		}else if(tokens[p].type==HEX){
			sscanf(tokens[p].str , "%x" , &value);
			return value ;
		}else if(tokens[p].type==REG){
			return isa_reg_str2val(tokens[p].str, success);
		}else{
			return 0 ;
		}
	}
	else if(check_parentheses(p,q)==true){
		printf("p:%d q%d\n",p,q);
		return eval(p+1, q-1, success);
	}
	else{
		int op = -1 ;
		int op_priority = 0 ;
		int i ;
		for(i=p; i<=q; i++){
			if(tokens[i].type=='('){
				int k=1 ;
				while(k!=0){
					i++ ;
					if(tokens[i].type=='('){
						k++ ;
					}else if(tokens[i].type==')'){
						k-- ;
					}
				}
			}else if(is_op(i)){
				int j ;
				for(j=0; j<len_optb; j++){
					if(tokens[i].type == op_table[j].oprat){
						break ;
					}
				}

				if(op_table[j].prior >= op_priority){
					op_priority = op_table[j].prior ;
					op = i ;
				}
			}

		}
		uint32_t val1=0;
		if(tokens[op].type!=DEREF && tokens[op].type!=NEG){
			val1 = eval(p, op-1, success);
		}
		uint32_t val2 = eval(op+1, q, success);

		switch(tokens[op].type){
			case '+' : return val1 + val2 ; break;
			case '-' : return val1 - val2 ; break;
			case '*' : return val1 * val2 ; break;
			case '/' : return val1 / val2 ; break;
			case EQ  : return val1 == val2 ; break;
			case NEQ : return val1 != val2 ; break;
			case AND : return val1 && val2 ; break;
			case OR  : return val1 || val2 ; break;
            case DEREF : return *(pmem+val2) ; break;

			default :assert(0); 
		}
	}
	return 0 ;
}

bool check_parentheses(int p , int q){
	int score = 0 ;
	int i ;
	for(i=p; i<q; i++){
		
		if(tokens[i].type == '('){
			score++ ;
		}else if(tokens[i].type == ')'){
			score-- ;
		}

		if(score == 0){
			return false ;
		}
		i++ ;
	}
	if(tokens[q].type==')'){
		score-- ;
	}
	
	if(score==0 && tokens[p].type == '(' && tokens[q].type == ')'){
		return true ;
	}else{
		return false ;
	}
}
