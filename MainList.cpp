#include <stdio.h>
#include <stdlib.h>
#include "ConstantsList.h"
#include "List.h"

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

    PopList(&list, 1);
    DumpList(&list);
    GraphDump(&list, dump_dot_name, &dump_html, number_of_dumps_txt_name);

    DestroyList(&list, &dump_html);

    return 0;
}
