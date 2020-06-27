/*!************************************************************************
\file           cachelist.c
\author         Adrian T.P. Kacmarcik
\par DP email   a.kacmarcik\@digipen.edu
\par course     CS180
\par section    A
\par assignment 1
\date           2020/01/23

\brief Description:
  Linked lists operstions with cacheing.
  
  cl_add_end:
    Add a node to the end of the list contained with the inputted list pointer
  
  cl_add_front:
    Add a node to the front of the list that is contained in the pointer
      passed in (this will change the head pointer to point to the new 
      beginning of the list)
  
  cl_remove:
    Search the list contained with the head pointer for the inputted search
      value and removes the forst one that it finds from the list
  
  cl_insert_before:
    Inserts a new node with the inputted value and label before the matching 
      value. If no matching value is found, then nothing is done.
  
  cl_insert_after:
    Inserts a new node with the inputted value and label after the matching
      value. If no matching value is found, the nothings is done.
  
  cl_find:
    Searches through the list looking for the first matching value, counting
      the number of comparisons it took to find the matching node. If
      cacheing is true then the found node will be moved to the beginning of
      the list to simmulate cacheing.
  
  cl_destroy:
    Frees the memory in the linked list that the head pointer is pointing to.
  
  cl_dump:
    Prints out the list to the console.
*************************************************************************/

#include <stdio.h>     /* printf                    */
#include <stdlib.h>    /* malloc, exit              */
#include <string.h>    /* strncpy                   */
#include "cachelist.h" /* LABEL_SIZE, cl_node, bool */

/*!************************************************************************
\brief  Make a new node based on inputs.
\param  value Number to be stored in the newNodes's |value|.
\param  label String to be stored in |label|.
\return The pointer to the new node.
**************************************************************************/
static cl_node *make_node(int value, const char *label)
{
  // malloc the space for a new node
  cl_node *node = (cl_node *)malloc(sizeof(cl_node));
  
  // check if malloc succeeded
  if (!node)
  {
    printf("Can't allocate new node.\n"); // print that it failed
    exit(1);                              // exit the code early
  }
  
  // set the values of |node|
  node->value = value;
  node->next = NULL;
  
  /* Be sure not to overwrite memory */
  strncpy(node->label, label, LABEL_SIZE - 1); // copy the label in |node|
  node->label[LABEL_SIZE - 1] = 0;             // set end |NULL|
  
  return node; // return the pointer to the new node
}

/*!************************************************************************
\brief Add a node to the end of |list|.
\param  list  Pointer to the start of the linked list.
\param  value Value to store in the new node.
\param  label Label for the new node.
\return The pointer to the head of the list.
**************************************************************************/
cl_node *cl_add_end(cl_node *list, int value, const char *label)
{
  // copy of the start of the list
  cl_node *listLocation = list; 
  cl_node *newNode = make_node(value, label); // node to add to |list| 
  
  // check if the list exists
  if (list)
  {
    // find the end of the list
    while (listLocation->next) {
      listLocation = listLocation->next;
    }
    listLocation->next = newNode; // add |newNode| to end of |list|
    
    return list; // return pointer to start of list
  }
  else
  {
    list = newNode; // set the head of the list to be the new node
    return newNode; // return the head of the new list
  }
}

/*!************************************************************************
\brief Add a new node to the front of |list|.
\param list  Head pointer.
\param value Value for new node.
\param label String for the new node.
\return The pointer to the head of |list|.
**************************************************************************/
cl_node *cl_add_front(cl_node *list, int value, const char *label)
{
  cl_node *newNode = make_node(value, label); // node to add to |list|
  
  newNode->next = list; // point next to be the old start
  list = newNode;       // update the head pointer
  
  return newNode; // return new head pointer
}

/*!************************************************************************
\brief Search |list| for a match and delete it.
\param list         Head pointer of the list.
\param search_value Value to search |list| for.
\return The head pointer.
**************************************************************************/
cl_node *cl_remove(cl_node *list, int search_value)
{
  cl_node *listLocation = list;     // current location in |list|
  cl_node *prevListLocation = list; // previous location in |list|
  
  // loop through |list|
  while (listLocation)
  {
    // check for match
    if (listLocation->value == search_value)
    {
      // check for begginging of |list|
      if (prevListLocation == listLocation)
      {
        // move head pointer to next node
        cl_node *temp = listLocation->next;
        free(listLocation); // free the head pointer
        list = temp;        // change the head pointer to new one
        return temp;        // return new head pointer
      }
      else
      {
        // move the pointers to skip over |listLocation|
        prevListLocation->next = listLocation->next;
        free(listLocation); // free |listLocaiton|
        return list;        // return head pointer
      }
    }
    // move to next node
    prevListLocation = listLocation;
    listLocation = listLocation->next;
  }
  // if nothing found return head pointer
  return list;
}

/*!************************************************************************
\brief Add a new node before the matching value.
\param list         Pointer to the start of the list.
\param search_value Value to search |list| for.
\param value        Value for new node.
\param label        String for the new node.
\return The pointer to the head of the list.
**************************************************************************/
cl_node *cl_insert_before(cl_node *list, int search_value, int value, 
                          const char *label)
{
  cl_node *listLocation = list;     // current location in |list|
  cl_node *prevListLocation = list; // previous location in |list|
  
  // loop through |list|
  while (listLocation)
  {
    // check if ew have found a match
    if (listLocation->value == search_value)
    {
      cl_node *newNode = make_node(value, label); // create new node 
      newNode->next = listLocation;               // point next to curr
      
      //check if we are at the start of the list
      if (prevListLocation == listLocation)
      {
        // new node is at the beggining change the head pointer
        list = newNode;
        return newNode; // return new head pointer
      }
      else
      {
        // set the previous node to be pointing at the new one
        prevListLocation->next = newNode;
        
        return list; // return the head pointer
      }
    }
    // move to the next node
    prevListLocation = listLocation;
    listLocation = listLocation->next;
  }
  // if none are found return the head pointer
  return list;
}

/*!************************************************************************
\brief Add new node after matching value
\param list         Pointer to the start of the list.
\param search_value Value to match
\param value        Value of the new node
\param label        String for the new node
**************************************************************************/
void cl_insert_after(cl_node *list, int search_value, int value, 
                     const char *label)
{
  cl_node *listLocation = list; // current location in |list|
  
  // loop through |list|
  while (listLocation)
  {
    // check if we founnd a match
    if (listLocation->value == search_value)
    {
      cl_node *newNode = make_node(value, label); // node to add to |list|
      
      // insert node in |list|
      newNode->next = listLocation->next;
      listLocation->next = newNode;
      
      return; // escape with first match found
    }
    listLocation = listLocation->next; // next node
  }
}

/*!************************************************************************
\brief Find a node in |list|, and if |cache| then move to the beginning if found
\param list         Pointer to the head of the list
\param search_value Value to search for in |list|
\param cache        Weather we should be cacheing
\param compares     Pointer to store the number of comparasons
\return The pointer to the head of the list.
**************************************************************************/
cl_node *cl_find(cl_node *list, int search_value, bool cache, int *compares)
{
  cl_node *listLocation = list;     // current location in |list|
  cl_node *prevListLocation = list; // previous location in |lsit|
  *compares = 0;                    // set the number of coimpares to |0|
  
  // loop through |list|
  while (listLocation)
  {
    (*compares)++; // incrament the number of compares
    
    // check if we have found a matching value
    if (listLocation->value == search_value)
    {
      // check if we are cacheing
      if (cache)
      {
        // check if we have moved form the start
        if (prevListLocation != listLocation)
        {
          prevListLocation->next = listLocation->next; // skip over curr loc
          listLocation->next = list;                   // add beggining
          list = listLocation;                         // point head to curr
        }
      }
      // return pointer to the start of the list
      return list;
    }
    // if we havent found anything then go to next node
    prevListLocation = listLocation;
    listLocation = listLocation->next;
  }
  // if there was nothing to match in the list then return the head
  return list;
}

/*!************************************************************************
\brief Deletes and frees all of |list|
\param list Pointer to the head of the list to free.
**************************************************************************/
void cl_destroy(cl_node *list)
{
  cl_node *listLocation = list; // temp storeage of location in |list|
  
  // loop through |list| freeing all of the nodes
  while (list)
  {
    listLocation = list; // store the current location in |listLocation|
    list = list->next;   // move |list| to be the next location
    free(listLocation);  // free |listLocation|
  }
}

/*!************************************************************************
\brief print out the list
\param list Pointer to the head of the list to walkk through.
**************************************************************************/
void cl_dump(const cl_node *list)
{
  printf("==================\n"); // print seperator
  
  // loop through |list|
  while (list)
  {
    printf("%4i: %s\n", list->value, list->label); // print formatted
    list = list->next;                             // next node in |list|
  }
}
