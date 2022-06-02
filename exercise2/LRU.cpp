#include <iostream>
#include "LRU.h"

LRU* find(LRU* LRU,std::string page)
{
    if(LRU == NULL)
    {
        return NULL;
    }
    while(LRU->next != NULL)
    {
        if(LRU->page.compare(page) == 0)
        {
            return LRU;
        }
        LRU = LRU->next;
    }

    if(LRU->page.compare(page) == 0)
    {
        return LRU;
    }
    return NULL;
}

int put_in_end(LRU_ptr* list,LRU* found)
{
    LRU* prev;
    LRU* temp;
    temp = list->start;
    prev = temp;
    if(list->end == found)
    {
        return 1;
    }
    else
    {
        if(list->start == found)
        {
            list->start = list->start->next;
            list->end->next = found;
            list->end = found;
            found->next = NULL;
        }
        else
        {
            while(temp != NULL)
            {
                if(temp->next == found)
                {
                    prev = temp;
                    break;
                }                    
                temp = temp->next;
            }

            if(found->next->next == NULL)
            {
                found->next->next = found;
                prev->next = list->end;
                list->end = found;
                found->next = NULL;
            }
            else
            {
                list->end->next = found;
                prev->next = found->next;
                list->end = found;
                found->next = NULL;
            }
            
        }
        
    }
    return 1;
}

LRU* create_LRU_node(std::string page)
{
    LRU* node = new LRU;
    node->page = page;
    node->next = NULL;
    return node;
}

void add_node(LRU_ptr* list,LRU* node)
{
    if(list->end == NULL)
    {
        list->end = node;
        list->start = node;
    }
    else
    {
        list->end->next = node;
        list->end = node;
    }
}

void remove_from_start(LRU_ptr* list)
{
    LRU* temp;
    if(list->start == list->end)
    {
        delete list->start;
        list->start = NULL;
        list->end = NULL;
    }
    else
    {
        temp = list->start;
        list->start = temp->next;
        delete temp;
    }
}

void delete_all(LRU_ptr* list)
{
    LRU* temp = list->start;
    if(list->start == NULL)
    {
        return;
    }
    while(list->end != list->start)
    {
        list->start = list->start->next;
        delete temp;
        temp = list->start;
    }
    delete list->start;
    return;
}

int list_length(LRU_ptr *list)
{
    LRU* temp = list->start;
    int length = 0;
    if(temp == NULL)
        return 0;
    while(temp != NULL)
    {
        length++;
        temp = temp->next;
    }
    return length;
}

void print_list(LRU_ptr *list)
{
    LRU* temp = list->start;
    while(temp != NULL)
    {
        std::cout << temp->page << std::endl;
        temp = temp->next;
    }
}