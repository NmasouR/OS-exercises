#include <iostream>
#include <string>

typedef struct clock_node{
    std::string page;
    bool r;
    clock_node *next;
} clock_node;


clock_node *find(clock_node *start,std::string page);

void update_r(clock_node *ptr,int *error);

clock_node *create_clock_node(std::string page);

void add_clock_node(clock_node **list,clock_node *clock_ptr);

std::string clock_remove(clock_node **list);

void delete_clock(clock_node *list);

void update_clock(clock_node **list,clock_node *clock_ptr);

int clock_lenght(clock_node *list);

void clock_print(clock_node *list);