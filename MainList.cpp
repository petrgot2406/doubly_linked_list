#include <stdio.h>
#include <stdlib.h>
#include "Constants.h"

struct List_t
{
    int* data;
    int* previous;
    int* next;
    int free;
    int size;
    int capacity;
};

Error_list_t InitList(List_t* list);
Error_list_t DestroyList(List_t* list);
Error_list_t DumpList(List_t list);
//Error_list_t PushToList(List_t* list, int anchor, int new_element);
//Error_list_t PopList(List_t* list, int anchor);
Error_list_t PoisonArray(int* array, int anchor);

int main()
{
    struct List_t list = {};

    InitList(&list);
    DumpList(list);

    //PushToList(&list, 1, 2);
    //DumpList(list);

    DestroyList(&list);

    return 0;
}

Error_list_t InitList(List_t* list)
{
    if (list == NULL)
    {
        printf("Error in address!\n");
        return ERROR_ADDRESS;
    }

    list->data     = (int*)calloc(DATA_SIZE, sizeof(int));
    list->previous = (int*)calloc(DATA_SIZE, sizeof(int));
    list->next     = (int*)calloc(DATA_SIZE, sizeof(int));

    list->free     = 1;
    list->size     = 1;
    list->capacity = DATA_SIZE;

    PoisonArray(list->data,     0);
    PoisonArray(list->previous, 1);

    list->next[0]     = 0;
    list->previous[0] = 0;

    for (int i = 1; i < list->capacity - 1; i++)
    {
        list->next[i] = i + 1;
    }

    list->next[list->capacity - 1] = 0;

    return LIST_OK;
}

Error_list_t DestroyList(List_t* list)
{
    if (list == NULL)
    {
        printf("Error in address!\n");
        return ERROR_ADDRESS;
    }

    free(list->data);
    free(list->previous);
    free(list->next);

    return LIST_OK;
}

Error_list_t DumpList(List_t list) {
    printf("\n---------------------------DUMP----------------------------\n");
    printf("data:       ");
    for(int i = 0; i < list.capacity; i++) {
        printf("%d ", list.data[i]);
    }
    printf("\nnext:     ");
    for(int i = 0; i < list.capacity; i++) {
        printf("%d ", list.next[i]);
    }
    printf("\nprevious: ");
    for(int i = 0; i < list.capacity; i++) {
        printf("%d ", list.previous[i]);
    }
    printf("\nfree: %d", list.free);
    printf("\nsize: %d", list.size);
    printf("\ncapacity: %d", list.capacity);
    printf("\n-----------------------------------------------------------\n");
    return LIST_OK;
}

/*
Error_list_t PushToList(List_t* list, int anchor, int new_element)
{
    list->data[list->free] = new_element;
    int old_free = list->free;

    list->free = list->next[list->free];

    int index = 0;

    for(int i = 0; i < anchor; i++)
    {
        index = list->next[index];
    }

    list->next[old_free] = list->next[index];
    list->previous[list->next[index]] = old_free;
    list->next[index] = old_free;
    list->previous[old_free] = index;

    list->size++;

    return LIST_OK;
}
*/
/*
Error_list_t PopList(List_t* list, int number)
{
    return LIST_OK;
}
*/

Error_list_t PoisonArray(int* array, int anchor)
{
    if (array == NULL)
    {
        return ERROR_ADDRESS;
    }

    for (int i = anchor; i < DATA_SIZE; i++)
    {
        array[i] = POISON;
    }

    return LIST_OK;
}
