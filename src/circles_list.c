/*==================================================================================*/
/* Realization of circles_list methods                                              */
/*==================================================================================*/

/* Standard incudes: */
#include <stdlib.h>
#include <stdio.h>

/* Program includes: */
#include <circles_list.h>
#include <circle.h>
#include <utils.h>
#include <misc.h>

Circles_list_t*
circles_list_create(void)
{
    /* Method for creation of the circles_list object (malloc) */

    Circles_list_t *tmp_list;

    tmp_list = (Circles_list_t *) calloc (1, sizeof(Circles_list_t));
    if (NULL == tmp_list) print_error_and_exit("Error in memory allocation!\n");

    tmp_list->head = NULL;
    tmp_list->size = 0;

    return tmp_list;
}

void
circles_list_delete(Circles_list_t* circles_list, b32 is_del_circle)
{
    /* Method for deleting the list with all it's elements from the memory */

    while (circles_list_is_empty(circles_list) == false) {
        circles_list_pop(circles_list, is_del_circle);
    }
    free(circles_list);
}

void
circles_list_push(Circles_list_t* circles_list, Circle_t *circle)
{
    /* Method for adding a new element to the list */

    Circles_list_elem_t *tmp_elem;

    tmp_elem = (Circles_list_elem_t *) calloc (1, sizeof(Circles_list_elem_t));
    if (NULL == tmp_elem) print_error_and_exit("Error in memory allocation!\n");

    tmp_elem->circle = circle;
    tmp_elem->next = circles_list->head;
    circles_list->head = tmp_elem;
    circles_list->size += 1;    
}

Circle_t
circles_list_pop(Circles_list_t* circles_list, b32 is_del_circle)
{
    /* Method for removing the element from the list. 
       free_content = 0: do not free the circle object
       free_content = 1: delete the circle object as well  */

    Circle_t tmp_circle;
    Circles_list_elem_t* tmp_elem; /* Element to be deleted (1-st element in the list) */

    /* Check if the list is already empty */
    if (circles_list->size == 0) {
        print_error_and_exit("ERROR: trying to pop in empty circles_list stack\n");
    }
    
    tmp_elem = circles_list->head;
    tmp_circle = *(tmp_elem->circle);
    
    /* Delete the element from the list */
    circles_list->head = tmp_elem->next;

    /* If necessary, delete aalso the circle object */
    if (is_del_circle) {
        circle_delete(tmp_elem->circle);
    }   
    free(tmp_elem);

    circles_list->size -= 1;
        
    return tmp_circle;
}

void
circles_list_empty(Circles_list_t* circles_list, b32 is_del_circle)
{
    /* Method to empty the circles list */

    while (!circles_list_is_empty(circles_list)) {
        circles_list_pop(circles_list, is_del_circle);
    }    
}

b32
circles_list_is_empty(Circles_list_t* circles_list)
{
    /* Method for checking if the list is totally empty */

    return (circles_list->size == 0);
}
