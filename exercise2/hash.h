#include <string>


typedef struct page_to_frame{
    std::string page;
    int frame_num;
    int read;
    page_to_frame* next;
}page_to_frame;

void find_page(page_to_frame* start,std::string page,int * exist,int * error);

page_to_frame* create_new_hash(std::string page,int place,int read);

void add_hash(page_to_frame** hash,page_to_frame* new_hash);

int find_frame(page_to_frame* start,std::string page);

void remove_hash(page_to_frame** start,std::string page);

void delete_hash(page_to_frame** start);

int emptry_hash(page_to_frame* hash);

int modified(page_to_frame* start,std::string page);

int get_read(page_to_frame* start,std::string page);

void set_read(page_to_frame* start,std::string page,int read);