#include "clock.h"


clock_node *find(clock_node *start,std::string page)
{
    if(start == NULL)
        return start;
    while(start->next != NULL)
    {
        if(start->page.compare(page) == 0)
        {
            return start;
        }
        start = start->next;
    }

    if(start->page.compare(page) == 0)
    {
        return start;
    }
    return NULL;

}

void update_r(clock_node *ptr,int *error)
{
    *error = 0;
    if(ptr == NULL)
    {
        *error = 1;
        return;
    }
    if(ptr->r == 0)
        ptr->r == 1;
    return;
}

clock_node *create_clock_node(std::string page)
{
    clock_node *new_clock = new clock_node;
    new_clock->page = page;
    new_clock->r = 0;
    new_clock->next = NULL;
    return new_clock;
}

void add_clock_node(clock_node **list,clock_node *clock_ptr)
{
    if((*list) == NULL)
    {
        *list = clock_ptr;
        return;
    }
    clock_node *temp = *list;
    while(temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = clock_ptr;
}

std::string clock_remove(clock_node **list)
{
    clock_node *temp,*prev;
    std::string rmv;
    if(*list == NULL)
    {
        return "error";
    }
    temp = *list;
    if((*list)->r == 0)
    {
        *list = (*list)->next;
        rmv = temp->page;
        delete temp;
        return rmv;
    }
    prev = *list;
    while(temp != NULL)
    {
        if(temp->r == 0)
        {
            while(prev->next != temp)
            {   
                prev = prev->next;
            }   
            prev->next = temp->next;
            rmv = temp->page;
            delete temp;
            return rmv;
        }
        if(temp->r == 1)
        {
            temp->r = 0;
            temp = temp->next;
        }
    }
    temp = *list;
    rmv = temp->page;
    *list = (*list)->next;
    delete temp;
    return rmv;
}

void delete_clock(clock_node *start)
{
    clock_node* temp = start;
    if(start == NULL)
    {
        return;
    }
    while(start != NULL)
    {
        start = start->next;
        delete temp;
        temp = start;
    }
    return;
}

void update_clock(clock_node **list,clock_node *clock_ptr)
{
    clock_node *prev = *list;
    clock_node *temp = *list;
    if(*list == NULL)
    {
        return;
    }
    while(temp->next != NULL)
    {
        temp = temp->next;
    }

    if((*list) == clock_ptr)
    {
        temp->next = clock_ptr;
        (*list) = (*list)->next;
        clock_ptr->next = NULL;
        return;
    }

    while(prev->next != clock_ptr)
    {
        prev = prev->next;
    }
    temp->next = clock_ptr;
    prev->next = clock_ptr->next;
    clock_ptr->next = NULL;
}

int clock_lenght(clock_node *list)
{
    int length = 0;
    if(list == NULL)
        return 0;
    while(list != NULL)
    {
        length++;
        list = list->next;
    }
    return length;
}

void clock_print(clock_node *list)
{
    clock_node* temp = list;
    while(temp != NULL)
    {
        std::cout << temp->page << std::endl;
        temp = temp->next;
    }
}