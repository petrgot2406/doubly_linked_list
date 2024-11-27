#include <stdio.h>
#include <stdlib.h>
#include "ConstantsList.h"

struct List_t
{
    int* data;
    int* previous;
    int* next;
    int free;
    int size;
    int capacity;
};

Error_list_t InitList(List_t* list, const char* dump_html_name, FILE** dump_html);
Error_list_t DestroyList(List_t* list, FILE** dump_html);
Error_list_t CheckList(List_t* list, int anchor);
Error_list_t DumpList(List_t* list);
Error_list_t PushToList(List_t* list, int anchor, int new_element);
Error_list_t PopList(List_t* list, int anchor);
Error_list_t PoisonArray(int* array, int anchor);
void GraphDump(List_t* list,     const char* dump_dot_name,
               FILE** dump_html, const char* number_of_dumps_txt_name);
void DeleteOldDumps(const char* number_of_dumps_txt_name);

int main()
{
    struct List_t list = {};

    const char* dump_dot_name            = "Dump.dot";
    const char* dump_html_name           = "Dump.html";
    const char* number_of_dumps_txt_name = "NumberOfDumps.txt";

    FILE* dump_html = NULL;

    DeleteOldDumps(number_of_dumps_txt_name);

    InitList(&list, dump_html_name, &dump_html);
    DumpList(&list);
    GraphDump(&list, dump_dot_name, &dump_html, number_of_dumps_txt_name);

    PushToList(&list, 0, 10);
    DumpList(&list);
    GraphDump(&list, dump_dot_name, &dump_html, number_of_dumps_txt_name);

    PushToList(&list, 0, 20);
    DumpList(&list);
    GraphDump(&list, dump_dot_name, &dump_html, number_of_dumps_txt_name);

    PushToList(&list, 0, 30);
    DumpList(&list);
    GraphDump(&list, dump_dot_name, &dump_html, number_of_dumps_txt_name);

    PopList(&list, 1);
    DumpList(&list);
    GraphDump(&list, dump_dot_name, &dump_html, number_of_dumps_txt_name);

    DestroyList(&list, &dump_html);

    return 0;
}

Error_list_t InitList(List_t* list, const char* dump_html_name, FILE** dump_html)
{
    if (list == NULL)
    {
        printf("Error in address!\n");
        return ERROR_ADDRESS;
    }

    *dump_html = fopen(dump_html_name, "w+");

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

Error_list_t DestroyList(List_t* list, FILE** dump_html)
{
    if (list == NULL)
    {
        printf("Error in address!\n");
        return ERROR_ADDRESS;
    }

    fclose(*dump_html);

    free(list->data);
    free(list->previous);
    free(list->next);

    list->data     = NULL;
    list->next     = NULL;
    list->previous = NULL;

    return LIST_OK;
}

Error_list_t CheckList(List_t* list, int anchor)
{
    if (list == NULL)
    {
        printf("Error: address is not exist!\n");
        return ERROR_ADDRESS;
    }
    else if (list->size <= 0)
    {
        printf("Error: size is lower than NULL!\n");
        return ERROR_SIZE;
    }
    else if (list->capacity < list->size)
    {
        printf("Error: capacity is lower than size!\n");
        return ERROR_CAPACITY;
    }
    else if (anchor < 0)
    {
        printf("Error: anchor is lower than NULL!\n");
        return ERROR_ANCHOR;
    }
    else if (anchor >= list->size)
    {
        printf("Error: anchor is bigger than size!\n");
        return ERROR_ANCHOR;
    }
    else if (list->free <= 0)
    {
        printf("Error: index of free element is lower than NULL!\n");
        return ERROR_FREE;
    }
    else if (list->free >= list->capacity)
    {
        printf("Error: index of free element is bigger than capacity!\n");
        return ERROR_FREE;
    }
    return LIST_OK;
}

Error_list_t DumpList(List_t* list)
{
    printf("\n--------------------------------------DUMP--------------------------------------\n");
    printf("data:     ");

    for (int i = 0; i < list->capacity; i++)
    {
        printf("%6d ", list->data[i]);
    }

    printf("\nnext:     ");

    for (int i = 0; i < list->capacity; i++)
    {
        printf("%6d ", list->next[i]);
    }

    printf("\nprevious: ");

    for (int i = 0; i < list->capacity; i++)
    {
        printf("%6d ", list->previous[i]);
    }

    printf("\n\nfree: %d", list->free);
    printf("\nsize: %d", list->size);
    printf("\ncapacity: %d", list->capacity);
    printf("\n--------------------------------------------------------------------------------\n");
    return LIST_OK;
}

Error_list_t PushToList(List_t* list, int anchor, int new_element)
{
    Error_list_t err_list = CheckList(list, anchor);

    if (err_list != LIST_OK)
    {
        return err_list;
    }

    list->data[list->free] = new_element;
    int old_free           = list->free;
    list->free             = list->next[list->free];

    int index = 0;

    for (int i = 0; i < anchor; i++)
    {
        index = list->next[index];
    }

    list->next[old_free]              = list->next[index];
    list->previous[list->next[index]] = old_free;
    list->next[index]                 = old_free;
    list->previous[old_free]          = index;

    list->size++;

    return LIST_OK;
}

Error_list_t PopList(List_t* list, int anchor)
{
    Error_list_t err_list = CheckList(list, anchor);

    if (err_list != LIST_OK)
    {
        return err_list;
    }

    int index = 0;

    for (int i = 0; i < anchor; i++)
    {
        index = list->next[index];
    }

    list->data[index] = POISON;

    int ind_next     = list->next[index];
    int ind_previous = list->previous[index];

    list->next[ind_previous] = ind_next;
    list->previous[ind_next] = ind_previous;

    list->previous[index] = POISON;
    list->next[index]     = list->free;
    list->free            = index;

    list->size--;

    return LIST_OK;
}

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

void GraphDump(List_t* list,     const char* dump_dot_name,
               FILE** dump_html, const char* number_of_dumps_txt_name)
{
    FILE* dump_dot = fopen(dump_dot_name, "w");

    fprintf(dump_dot, "digraph list {\n");
    fprintf(dump_dot, "    rankdir = LR\n");

    fprintf(dump_dot, "    node%d [shape = Mrecord, style = \"filled\", label = \"%d | data: %d"
                      "| next: %d | previous: %d\", fillcolor = \"#3e35de\"]\n",
                      0, 0, list->data[0],
                      list->next[0], list->previous[0]);

    for (int i = 1; i < list -> capacity; i++)
    {
        fprintf(dump_dot, "    node%d [shape = Mrecord, style = \"filled\", label = \"%d | data: %d "
                          "| next: %d | previous: %d\", fillcolor = \"#edd085\"]\n",
                          i, i, list->data[i],
                          list->next[i], list->previous[i]);
    }

    fprintf(dump_dot, "    free [fillcolor = \"black\", color = \"black\"]\n");

    fprintf(dump_dot, "    node%d -> node%d[color = \"black\"]\n",
                      0, list->next[0]);
    int i = list->next[0];

    while (i != 0)
    {
        fprintf(dump_dot, "    node%d -> node%d[color = \"black\"]\n",
                          i, list->next[i]);
        i = list->next[i];
    }

    fprintf(dump_dot, "    free -> node%d[color = \"black\"]\n",
                      list -> free);
    i = list->free;

    while (list -> next[i] != 0)
    {
        fprintf(dump_dot, "    node%d -> node%d[color = \"black\"]\n",
                          i, list->next[i]);
        i = list->next[i];
    }

    fprintf(dump_dot, "}");

    fclose(dump_dot);

    static int number_dump = 0;

    char* command = (char*)calloc(100, sizeof(char));

    sprintf(command, "dot Dump.dot -Tpng -o Graphs/Dump%02d.png",
                     number_dump);

    system(command);

    free(command);

    if(number_dump == 0)
    {
        fprintf(*dump_html, "<pre>\n");
    }

    fprintf(*dump_html, "\n<img src = \"Graphs/Dump%02d.png\"/>\n",
                        number_dump);

    number_dump++;

    FILE* number_file = fopen(number_of_dumps_txt_name, "w");

    fprintf(number_file, "%d", number_dump);

    fclose(number_file);
}

void DeleteOldDumps(const char* number_of_dumps_txt_name)
{
    int number;
    FILE* number_file = fopen(number_of_dumps_txt_name, "r");

    fscanf(number_file, "%d", &number);

    for (int i = 0; i < number; i++)
    {
        char file_name[100] = "";
        sprintf(file_name, "Graphs/Dump%02d.png", i);
        remove(file_name);
    }

    fclose(number_file);
}
