#include <iostream>
#include "hash.h"

void find_page(page_to_frame* start,std::string page,int * exist,int * error)
{
    if(start == NULL)
    {
        *error = 1;
        return;
    }
    while(start != NULL)
    {
        if(start->page.compare(page) == 0)
        {
            *exist = 1;
            break;
        }
        start = start->next;
    }
}

page_to_frame* create_new_hash(std::string page,int place,int read)
{
    page_to_frame* hash = new page_to_frame;
    hash->page = page;
    hash->frame_num = place;
    hash->read = read;
    hash->next = NULL;
    return hash;
}

void add_hash(page_to_frame** start,page_to_frame* new_hash)
{
    if((*start) == NULL)
    {
        (*start) = new_hash;
        return;
    }
    page_to_frame* temp = *start;
    while(temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = new_hash;
}


int find_frame(page_to_frame* next,std::string page)
{
    int frame;
    while(next != NULL)
    {
        if(next->page.compare(page) == 0)
        {
            frame = next->frame_num;
            return frame;
        }
        next = next->next;
    }
    return -1;
}

void remove_hash(page_to_frame** start,std::string page)
{
    page_to_frame* temp;
    page_to_frame* prev;
    if((*start) == NULL)
    {
        return;
    }
    if((*start)->page.compare(page) == 0)
    {
        temp = *start;
        (*start) = (* start)->next;
        delete temp;
        return;
    }
    temp = (*start)->next;
    prev = *start;
    while(temp != NULL)
    {
        if(temp->page.compare(page) == 0)
        {
            prev->next = temp->next;
            delete temp;
            return;
        }
        prev = prev->next;
        temp = temp->next;
    }
}

void delete_hash(page_to_frame** start)
{
    if(start == NULL)
    {
        return;
    }
    if((*start) == NULL)
    {
        return;
    }
    page_to_frame* temp = *start; 
    while(temp != NULL)
    {
        *start = (*start)->next;
        delete temp;
        temp = *start;
    }
}


int emptry_hash(page_to_frame* hash)
{
    return(hash != NULL);
}

int modified(page_to_frame* start,std::string page)
{
    int read;
    while(start != NULL)
    {
        if(start->page.compare(page) == 0)
        {
            read = start->read;
            return read;
        }
        start = start->next;
    }
    return -1;
}

int get_read(page_to_frame* start,std::string page)
{
    if(start == NULL)
    {
        return -1;
    }
    while(start != NULL)
    {
        if(start->page.compare(page) == 0)
        {
            return start->read;
        }
        start = start->next;
    }
    return -1;
}

void set_read(page_to_frame* start,std::string page,int read)
{
    while(start != NULL)
    {
        if(start->page.compare(page) == 0)
        {
            start->read = read;
            return;
        }
        start = start->next;
    }
}