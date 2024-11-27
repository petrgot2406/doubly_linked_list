#ifndef LIST_H
#define LIST_H

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

#endif
