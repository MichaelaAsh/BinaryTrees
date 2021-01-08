#include "tree.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

#define TRUE 1
#define FALSE 0


//allocate memory for a performance structure 
struct Performance *newPerformance () {
    struct Performance *new = malloc(sizeof(struct Performance)); //allocate memory  
    if (new == NULL) {
         // if malloc fails print error message to std error and exit 
        fprintf(stderr, "Error: malloc failed\n"); 
        exit(-1); 
    }
    new -> reads = 0; 
    new -> writes = 0; 
    new -> mallocs = 0; 
    new -> frees = 0; 

    return new; 
}// end of *newPerformanc 
///////////////////////////////////////////////////////////////////////////////////////////////////////

void attachNode (struct Performance *performance, struct Node **node_ptr, void *src, unsigned int width) {
    struct Node *new_node = (struct Node *) malloc(sizeof(struct Node)); 
    if (new_node == NULL) {
         // if malloc fails print error message to std error and exit 
        fprintf(stderr, "Error: malloc failed\n"); 
        exit(-1); 
    }

    new_node -> data = malloc (width); 
    memcpy((unsigned char* )(new_node-> data), src, width); //copies src into new_node -> data

    new_node -> lt = NULL;
    new_node -> gte = NULL; 

    (*node_ptr) = new_node; 

    performance -> mallocs += 1; 
    performance -> writes += 1; 

}
///////////////////////////////////////////////////////////////////////////////////////////////////////

/*Return the value returned by the compar function ponter, where applied to target and the data variable 
in the struct at the address node_ptr points (in that order)*/ 
int comparNode (struct Performance *performance, struct Node **node_ptr, int (*compar)(const void *, const void *), void *target) {
     
     int value = compar (target, (*node_ptr)-> data); 
     performance -> reads += 1; 
     return value; 
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//check if the list is empty 
int isEmpty (struct Performance *performance, struct Node **node_ptr) {
    
    if ((*node_ptr) == NULL) {
        return  TRUE;  
    }
    return FALSE; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

//deteremines tthe next node to visit in the tree
struct Node **next (struct Performance *performance, struct Node **node_ptr, int direction) {
    
    if (isEmpty(performance, node_ptr) == TRUE) {
          fprintf(stderr, "Error: List is empty. Cannot point get next node_ptr\n");
         exit (-1); // if the list is empty print to stderr ad exits
    }
    
    performance -> reads += 1; 
    if (direction < 0) {
        return &((*node_ptr) -> lt);
    }
    
    if (direction >= 0) {
        return &((*node_ptr) -> gte);
    }
     return &((*node_ptr) -> gte);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
//Copy data from a node in the tree into dest
void readNode (struct Performance *performance, struct Node **node_ptr, void *dest, unsigned int width) {

    if (isEmpty(performance, node_ptr) == TRUE) {
          fprintf(stderr, "Error: List is empty. Cannot read node\n");
         exit (-1); // if the list is empty print to stderr ad exits
    }

      //copy data from the head into dest
     memcpy(  dest, (*node_ptr) -> data, width);

     performance -> reads += 1; //increment reads by 1

}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//Copy data from a node in the tree into dest
void detachNode (struct Performance *performance, struct Node **node_ptr) {

    if (isEmpty(performance, node_ptr) == TRUE) {
          fprintf(stderr, "Error: List is empty. Cannot detach node\n");
         exit (-1); // if the list is empty print to stderr ad exits
    }
    
    
    free( (*node_ptr) -> data); 
    free(*node_ptr);  // remove node
    (*node_ptr) = NULL; 

    performance -> frees +=1; 

}
///////////////////////////////////////////////////////////////////////////////////////////////////////
// add item to appropriate spot
void addItem (struct Performance *performance, struct Node **node_ptr, int (*compar)(const void *, const void *), void *src, unsigned int  width) {
    
    int direction; 
    if (isEmpty(performance, node_ptr) == TRUE) {
        attachNode (performance,node_ptr, src, width); 
    } else { 
        while (isEmpty(performance, node_ptr) == FALSE) {
            direction = comparNode(performance, node_ptr, compar, src);
            node_ptr = next(performance,node_ptr, direction);

        }
        attachNode (performance,node_ptr, src, width); 
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
//frees the entire Tree recursively
void freeTree(struct Performance *performance, struct Node **node_ptr) {

   if (isEmpty(performance, node_ptr) == FALSE) { //while list is not empty
      
            
        freeTree (performance,next(performance,node_ptr,-1));  //free left child     
        freeTree (performance, next(performance,node_ptr,1));   //free right child
   
        detachNode (performance,node_ptr);    //free node
     }     
} 
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//search for target in tree
int searchItem (struct Performance *performance, struct Node **node_ptr, int (*compar)(const void *, const void *_), void *target, unsigned int width) {
  
    while (isEmpty(performance, node_ptr) == FALSE) { //while list is not empty
        
        int direction = comparNode(performance, node_ptr, compar, target);
        if(0 < direction ||  0 > direction ){
             node_ptr = next(performance,node_ptr,direction); //if not a match then go to next child
        } 

        if(0 == direction){
            readNode (performance, node_ptr, target, width); //copy to target 
            return TRUE; //found
        } 
           
    }
    
    return FALSE; //not found
  
}//end of search