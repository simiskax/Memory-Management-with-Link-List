# Memory-Management-with-Link-List
Memory management using the connected list. Nodes are created, merged and released.

1. All nodes print
void  print_n(node *r);

2. Add nodes by sorting with start adress and offset
node* addsort_n(node *r, int start,int offset);

3. Delete a node with start adress
node* delete_n(node *r, int start);

4. Add nodes to the most appropriate location 
node* add_undirected(node *r, int offset);

5. Free and delete node fields
node* freefunc(node *r , int start, int offset);

6. Compare two nodes address values
int cmp(const void *a, const void *b);
