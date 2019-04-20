/*
 * mylist.h
 *
 *  Created on: 7 Kas 2018
 *      Author: aytac.macit
 */

#ifndef MYLIST_H_
#define MYLIST_H_

#define minMemorySize 0
#define maxMemorySize	900

typedef struct  node {
		int startAddr;
		int finishAddr;
		struct node *next;
}node;

node *root;

void  print_n(node *r);
node* addsort_n(node *r, int start,int offset);
node* delete_n(node *r, int start);
node* add_undirected(node *r, int offset);
node* freefunc(node *r , int start, int offset);
int cmp(const void *a, const void *b);

#endif /* MYLIST_H_ */
