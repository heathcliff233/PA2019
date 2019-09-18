#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
  TK_NOTYPE = 256, /*TK_EQ,*/ TK_PLS = 1,TK_MIN = 2,TK_MUL = 3,TK_DIV = 4,TK_LFP = 5,TK_RTP = 6,TK_NUM = 0

  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */
  {"\/", TK_DIV},         // division
  {"\-", TK_MIN},         // minus
  {"\*", TK_MUL},         // multiply
  {" +", TK_NOTYPE},      // spaces
  {"\\+", TK_PLS},        // plus
  {"(", TK_LFP},          // left parentheses
  {")", TK_RtP},          // right parenttheses
  {"0", TK_NUM},
  {"1", TK_NUM}, 
  {"2", TK_NUM},
  {"3", TK_NUM},
  {"4", TK_NUM},
  {"5", TK_NUM},
  {"6", TK_NUM},
  {"7", TK_NUM},
  {"8", TK_NUM},
  {"9", TK_NUM}, 
//  {"==", TK_EQ}           // equal
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

static Token tokens[32] __attribute__((used)) = {};
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
		nr_token ++;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
			case 1 : tokens[nr_token-1].type = TK_PLS; break;
			case 2 : tokens[nr_token-1].type = TK_MIN; break;
			case 3 : tokens[nr_token-1].type = TK_MUL; break;
			case 4 : tokens[nr_token-1].type = TK_DIV; break;
			case 5 : tokens[nr_token-1].type = TK_LFP; break;
		    case 6 : tokens[nr_token-2].type = TK_RTP; break;
			case 0 : tokens[nr_token-1].type = TK_NUM; tokens[nr_token-1].str = re[i]; break;

            default: TODO();
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

uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  TODO();

  return 0;
}
