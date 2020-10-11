/*================================================================================*/
/* Header file for the circles_list class.                                        */
/*================================================================================*/
#ifndef CIRCLES_LIST_H_
#define CIRCLES_LIST_H_

#include <utils.h>
#include <circle.h>

/* Structure of the circles_list single element */
struct Circles_list_elem {
    Circle_t *circle; /* Data field of the list */
    struct Circles_list_elem *next; /* Pointer to the next element */
};
typedef struct Circles_list_elem Circles_list_elem_t;

/* Structure of the circles_list */
struct Circles_list {
    Circles_list_elem_t *head; /* Pointer to the head element */
    u32 size; /* Total number of elements in the list */
};
typedef struct Circles_list Circles_list_t;

/* Methods of circles_list class */
/* Method for creation of the circles_list object (malloc) */
Circles_list_t *circles_list_create(void);

/* Method for deleting the list with all it's elements from the memory */
void circles_list_delete(Circles_list_t *circles_list, b32 is_del_circle);

/* Method for adding a new element to the list */
void circles_list_push(Circles_list_t *circles_list, Circle_t *circle);

/* Method for removing the element from the list */
Circle_t circles_list_pop(Circles_list_t *circles_list, b32 is_del_circle);

/* Method to empty the circles list */
void circles_list_empty(Circles_list_t *circles_list, b32 is_del_circle);

/* Method for checking if the list is totally empty */
b32 circles_list_is_empty(Circles_list_t *circles_list);

#endif //CIRCLES_LIST_H_
