#include <string>

typedef struct LRU{
    std::string page;
    LRU* next;
}LRU;

typedef struct LRU_ptr{
    LRU* start;
    LRU* end;
}LRU_ptr;

LRU* find(LRU* LRU,std::string page);

int put_in_end(LRU_ptr* list,LRU* found);

LRU* create_LRU_node(std::string page);

void add_node(LRU_ptr* list,LRU* node);

void remove_from_start(LRU_ptr* list);

void delete_all(LRU_ptr* list);

int list_length(LRU_ptr *list);

void print_list(LRU_ptr *list);