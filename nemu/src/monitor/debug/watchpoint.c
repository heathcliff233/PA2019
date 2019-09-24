#include "monitor/watchpoint.h"
#include "monitor/expr.h"
#include <stdlib.h>

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;
WP *scan_wp();

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
	wp_pool[i].active = false;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp(){
	assert(free_ != NULL);
	WP* wp = free_;
	free_ = free_->next;
	wp->active = true;
	return wp;

}

void free_wp(WP* wp){
	assert(wp->active == true);
	wp->active = false;
	free(wp->expr);
	wp->expr = NULL;
	wp->next = free_;
	free_ = wp; 
}

void list_watchpoints(){
	if(head == NULL){
		printf("no breakpoints set");
		return;
	}
	printf("%8s\t%8s\t%8s","NO","Address","Value");
	WP *wp;
	for(wp=head; wp!=NULL; wp=wp->next){
		printf("%8d\t%#08x\t%8d\n",wp->NO,wp->addr,wp->old_value);
	}
}

bool del_watchpoint(int NO){
	WP *wp, *pwp = NULL;
	for(wp=head; wp!=NULL; pwp=wp, wp=wp->next){
		if(wp->NO == NO) break;
	}
	if(wp==NULL){
		printf("no match");
		return false;
	}
	if(pwp==NULL){
		head = wp->next;
		free_wp(wp);
		return true;
	}
	pwp->next = wp->next;
	free_wp(pwp);
	return true;
}

int set_watchpoint(char *e){
	uint32_t val = 0;
	bool success;
	printf("exp: %s\n",e);
	val = expr(e,&success);
	printf("%d\n",val);
	if(!success){
		printf("wrong expression fuck you");
		return -1;
	}

	WP *wp = new_wp();
//	wp->expr = (char *)malloc(sizeof(e));
	wp->expr = strdup(e);
	wp->old_value = val;
	wp->active = true;

	wp->next = head;
	head = wp;

	return wp->NO;
}

WP *scan_wp(){
	WP *wp;
	for(wp=head; wp!=NULL; wp=wp->next){
		bool success;
		wp->new_value = expr(wp->expr,&success);
		if(wp->old_value != wp->new_value) return wp;

	}
	return NULL;
}
