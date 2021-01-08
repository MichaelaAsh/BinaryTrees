struct Node {
    void *data; 
    struct Node *lt;  //less than parent node 
    struct Node *gte; // greater than or equals to parent node 
}; 

struct Performance { // tracks the "performance" of the program
    unsigned int reads; 
    unsigned int writes; 
    unsigned int mallocs; 
    unsigned int frees; 
}; 

///////////////////////////////////////////////////////////////////////
struct Performance *newPerformance (); 

void attachNode (struct Performance *performance, struct Node **node_ptr, void *src, unsigned int width); 

int comparNode (struct Performance *performance, struct Node **node_ptr, int (*compar)(const void *, const void *), void *target); 

int isEmpty (struct Performance *performance, struct Node **node_ptr); 

struct Node **next (struct Performance *performance, struct Node **node_ptr, int direction); 

void readNode (struct Performance *performance, struct Node **node_ptr, void *dest, unsigned int width); 

void detachNode (struct Performance *performance, struct Node **node_ptr); 

void addItem (struct Performance *performance, struct Node **node_ptr, int (*compar)(const void *, const void *), void *src, unsigned int  width); 

void freeTree (struct Performance *performance, struct Node **node_ptr); 

int searchItem (struct Performance *performance, struct Node **node_ptr, int (*compar)(const void *, const void *_), void *target, unsigned int width) ; 