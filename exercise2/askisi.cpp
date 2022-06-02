#include <iostream>
#include <fstream>
#include <cstring>

#include "LRU.h"
#include "hash.h"
#include "clock.h"

unsigned int Hash(std::string page);



int main(int argc,char *argv[])
{
    short int algorithm;
    int frame_num,exist = 0,free_space = 0,q,read = 0,frame_rmv,write_in_disk = 0,read_from_disk = 0,page_fault = 0;
    int trace = 0,trace_num1 = 0,error = 0,max_trace,free_frames = 0,trace_num = 0;
    unsigned int hash_num,hash_num1;
    page_to_frame* hash_table[4096],*hash_table2[4096],*new_hash;
    LRU* new_node,*found = NULL;
    clock_node *clock_ptr = NULL,*clock_proccess = NULL,*clock_proccess1 = NULL,*new_clock;
    LRU_ptr LRU_pointer,LRU_pointer2;
    std::string line,page,offset,remove;
    std::fstream file1,file2;
    LRU_pointer.start = NULL;
    LRU_pointer.end = NULL;
    LRU_pointer2.start = NULL;
    LRU_pointer2.end = NULL;
    for(int i = 0;i<4096;i++) //initialization
    {
        hash_table[i] = NULL;
    }
    for(int i = 0;i<4096;i++)
    {
        hash_table2[i] = NULL;
    }
    max_trace = atoi(argv[4]);
    frame_num = atoi(argv[2]);
    int frame[frame_num];
    for(int i = 0;i < frame_num;i++)
    {
        frame[i] = 0;
    }
    file1.open("gcc.trace");
    if(!file1.is_open())
    {
        std::cout << "error open file" << std::endl;
    }
    file2.open("bzip.trace");
    if(!file2.is_open())
    {
        std::cout << "error open file" << std::endl;
    }

    if(strstr(argv[1],"LRU") != NULL)
    {   
        algorithm = 1;
    }
    else
    {
        algorithm = 0;
    }
    q = atoi(argv[3]);

    if(algorithm == 1)
    {

        while(trace < 2*max_trace)
        {
            for(int i = 0;i < q;i++)
            {
                trace_num++;
                read = 0;
                exist = 0;
                std::getline(file1,line);
                page = line.substr(0,5); //20 bits
                offset = line.substr(5,3); //12 bits log2(4096)
                if(line.back() == 'R') 
                {
                    read = 1;
                }
                hash_num = Hash(page); //hash function
                if(emptry_hash(hash_table[hash_num]) != 0)
                {
                    find_page(hash_table[hash_num],page,&exist,&error);
                    if(error == 1)
                    {
                        std::cout << "error!" << std::endl;
                    }
                }

                if(exist == 1) //page exists in the memory already
                {
                    if(get_read(hash_table[hash_num],page) != read && read == 0)//if page can now be modified
                        set_read(hash_table[hash_num],page,read);
                    found = find(LRU_pointer.start,page);
                    if(found != NULL)
                    {
                        put_in_end(&LRU_pointer,found);
                    }
                    else
                    {
                        std::cout <<"not found in LRU" << std::endl;
                    }                
                }
                else if(exist == 0) //page not in mem
                {
                    page_fault++;
                    read_from_disk = read_from_disk + 1;
                    int place;
                    for(int i = 0;i < frame_num;i++)
                    {
                        if(frame[i] == 0)
                        {
                            free_space = 1;
                            place = i;
                            break;
                        }
                    }
                    if(free_space == 1 && list_length(&LRU_pointer) <= (frame_num/2))
                    {
                        new_hash = create_new_hash(page,place,read);
                        add_hash(&hash_table[hash_num],new_hash);
                        frame[place] = 1;
                        new_node = create_LRU_node(page);
                        add_node(&LRU_pointer,new_node);
                        free_space = 0;
                    }
                    else //there is not free space in mem
                    {
                        frame_rmv = -1;
                        if(LRU_pointer.start != NULL)
                        {
                            remove = LRU_pointer.start->page;
                            hash_num1 = Hash(remove);
                            frame_rmv = find_frame(hash_table[hash_num1],remove);
                            if(frame_rmv == -1)
                                std::cout << "error!" << std::endl;

                        }
                        if(frame_rmv == -1)
                        {
                            remove = LRU_pointer2.start->page;
                            hash_num1 = Hash(remove);
                            frame_rmv = find_frame(hash_table2[hash_num1],remove);
                            if(frame_rmv == -1)
                                std::cout << "error!" << std::endl;
                            if(modified(hash_table2[hash_num1],remove) == 0)
                                write_in_disk+=1;
                            else if(modified(hash_table2[hash_num1],remove) == -1)
                                std::cout << "cant find read or write" << std::endl;
                            remove_from_start(&LRU_pointer2);
                            remove_hash(&hash_table2[hash_num1],remove);
                            new_hash = create_new_hash(page,frame_rmv,read);
                            add_hash(&hash_table[hash_num],new_hash);
                            new_node = create_LRU_node(page);
                            add_node(&LRU_pointer,new_node);
                            continue;
                        }
                        if(modified(hash_table[hash_num1],remove) == 0)
                            write_in_disk+=1;
                        else if(modified(hash_table[hash_num1],remove) == -1)
                            std::cout << "cant find read or write" << std::endl;
                        remove_from_start(&LRU_pointer);
                        remove_hash(&hash_table[hash_num1],remove);
                        new_hash = create_new_hash(page,frame_rmv,read);
                        add_hash(&hash_table[hash_num],new_hash);
                        new_node = create_LRU_node(page);
                        add_node(&LRU_pointer,new_node);
                    }
                }
            }
            for(int i = 0;i <q;i++) //2nd proccess
            {
                trace_num1++;
                read = 0;
                exist = 0;
                std::getline(file2,line);
                page = line.substr(0,5); //20 bits
                offset = line.substr(5,3); //12 bits log2(4096)
                if(line.back() == 'R')
                {
                    read = 1;
                }
                hash_num = Hash(page);
                if(emptry_hash(hash_table2[hash_num]) != 0)
                {
                    find_page(hash_table2[hash_num],page,&exist,&error);
                    if(error == 1)
                    {
                        std::cout << "error!" << std::endl;
                    }
                }
                if(exist == 1) //page exists in the memory already
                {
                    if(get_read(hash_table2[hash_num],page) != read && read == 0)
                        set_read(hash_table2[hash_num],page,read);
                    found = find(LRU_pointer2.start,page);
                    if(found != NULL)
                    {
                        put_in_end(&LRU_pointer2,found);
                    }
                }
                else if(exist == 0) //page not in mem
                {
                    page_fault++;
                    read_from_disk = read_from_disk + 1;
                    int place;
                    for(int i = 0;i < frame_num;i++)
                    {
                        if(frame[i] == 0)
                        {
                            free_space = 1;
                            place = i;
                            break;
                        }
                    }
                    if(free_space == 1)
                    {
                        new_hash = create_new_hash(page,place,read);
                        add_hash(&hash_table2[hash_num],new_hash);
                        frame[place] = 1;
                        new_node = create_LRU_node(page);
                        add_node(&LRU_pointer2,new_node);
                        free_space = 0;
                    }
                    else
                    {
                        frame_rmv = -1;
                        if(LRU_pointer2.start != 0)
                        {
                            remove = LRU_pointer2.start->page;
                            hash_num1 = Hash(remove);
                            frame_rmv = find_frame(hash_table2[hash_num1],remove);
                            if(frame_rmv == -1)
                                std::cout << "error!" << std::endl;
                        }
                        if(frame_rmv == -1)
                        {
                            remove = LRU_pointer.start->page;
                            hash_num1 = Hash(remove);
                            frame_rmv = find_frame(hash_table[hash_num1],remove);
                            if(frame_rmv == -1)
                                std::cout << "error!" << std::endl;
                            if(modified(hash_table[hash_num1],remove) == 0)
                                write_in_disk+=1;
                            else if(modified(hash_table[hash_num1],remove) == -1)
                                std::cout << "cant find read or write" << std::endl;
                            remove_from_start(&LRU_pointer);
                            remove_hash(&hash_table[hash_num1],remove);
                            new_hash = create_new_hash(page,frame_rmv,read);
                            add_hash(&hash_table2[hash_num],new_hash);
                            new_node = create_LRU_node(page);
                            add_node(&LRU_pointer2,new_node);
                            continue;
                        }
                        if(modified(hash_table2[hash_num1],remove) == 0)
                            write_in_disk+=1;
                        else if(modified(hash_table2[hash_num1],remove) == -1)
                            std::cout << "cant find read or write" << std::endl;
                        remove_from_start(&LRU_pointer2);
                        remove_hash(&hash_table2[hash_num1],remove);
                        new_hash = create_new_hash(page,frame_rmv,read);
                        add_hash(&hash_table2[hash_num],new_hash);
                        new_node = create_LRU_node(page);
                        add_node(&LRU_pointer2,new_node);
                    }
                }
            }
            trace = trace + 2*q;
        }
        delete_all(&LRU_pointer);
        delete_all(&LRU_pointer2);
    }
    else if(algorithm == 0)
    {
        while(trace < 2*max_trace)
        {
            for(int i = 0;i < q;i++)
            {
                trace_num++;
                read = 0;
                exist = 0;
                std::getline(file1,line);
                page = line.substr(0,5); //20 bits
                offset = line.substr(5,3); //12 bits log2(4096)
                if(line.back() == 'R')
                {
                    read = 1;
                }
                hash_num = Hash(page);
                if(emptry_hash(hash_table[hash_num]) != 0)
                {
                    find_page(hash_table[hash_num],page,&exist,&error);
                    if(error == 1)
                    {
                        std::cout << "error!" << std::endl;
                    }
                }
                if(exist == 1)
                {
                    if(get_read(hash_table[hash_num],page) != read && read == 0)
                        set_read(hash_table[hash_num],page,read);
                    clock_ptr = find(clock_proccess,page);
                    if(clock_ptr != NULL)
                    {
                        update_r(clock_ptr,&error);
                        if(error == 1)
                        {
                            std::cout << "an error has occur" << std::endl;
                        }
                        update_clock(&clock_proccess,clock_ptr);

                    }
                    else
                    {
                        std::cout <<"not found in clock" << std::endl;
                    }                

                }
                else if(exist == 0)
                {
                    page_fault++;
                    read_from_disk = read_from_disk + 1;
                    int place;
                    for(int i = 0;i < frame_num;i++)
                    {
                        if(frame[i] == 0)
                        {
                            free_space = 1;
                            place = i;
                            break;
                        }
                    }
                    if(free_space == 1 && clock_lenght(clock_proccess) <= (frame_num/2))
                    {
                        new_hash = create_new_hash(page,place,read);
                        add_hash(&hash_table[hash_num],new_hash);
                        frame[place] = 1;
                        new_clock = create_clock_node(page);
                        add_clock_node(&clock_proccess,new_clock);
                        free_space = 0;
                    }
                    else if(free_space == 0)
                    {
                        frame_rmv = -1;
                        if(clock_proccess != NULL)
                        {
                            remove = clock_remove(&clock_proccess);
                            hash_num1 = Hash(remove);
                            frame_rmv = find_frame(hash_table[hash_num1],remove);
                            if(frame_rmv == -1)
                                std::cout << "error" << std::endl;

                        }
                        if(frame_rmv == -1)
                        {
                            remove = clock_remove(&clock_proccess1);
                            hash_num1 = Hash(remove);
                            frame_rmv = find_frame(hash_table2[hash_num1],remove);
                            if(frame_rmv == -1)
                                std::cout << "error" << std::endl;
                            if(modified(hash_table2[hash_num1],remove) == 0)
                                write_in_disk++;
                            else if(modified(hash_table2[hash_num1],remove) == -1)
                                std::cout << "cant find read or write" << std::endl;
                            remove_hash(&hash_table2[hash_num1],remove);
                            new_hash = create_new_hash(page,frame_rmv,read);
                            add_hash(&hash_table[hash_num],new_hash);
                            new_clock = create_clock_node(page);
                            add_clock_node(&clock_proccess,new_clock);
                            continue; 
                        }
                        if(modified(hash_table[hash_num1],remove) == 0)
                            write_in_disk++;
                        else if(modified(hash_table[hash_num1],remove) == -1)
                            std::cout << "cant find read or write" << std::endl;
                        remove_hash(&hash_table[hash_num1],remove);
                        new_hash = create_new_hash(page,frame_rmv,read);
                        add_hash(&hash_table[hash_num],new_hash);
                        new_clock = create_clock_node(page);
                        add_clock_node(&clock_proccess,new_clock);
                    }
                }
            }
            for(int i = 0;i < q;i++)
            {
                trace_num1++;
                exist = 0;
                std::getline(file2,line);
                page = line.substr(0,5); //20 bits
                offset = line.substr(5,3); //12 bits log2(4096)
                if(line.back() == 'R')
                {
                    read = 1;
                }
                hash_num = Hash(page);
                if(emptry_hash(hash_table2[hash_num]) != 0)
                {
                    find_page(hash_table2[hash_num],page,&exist,&error);
                    if(error == 1)
                    {
                        std::cout << "error!" << std::endl;
                    }
                }
                if(exist == 1)
                {
                    if(get_read(hash_table2[hash_num],page) != read && read == 0)
                        set_read(hash_table2[hash_num],page,read);
                    clock_ptr = find(clock_proccess1,page);
                    if(clock_ptr != NULL)
                    {
                        update_r(clock_ptr,&error);
                        if(error == 1)
                        {
                            std::cout << "an error has occur" << std::endl;
                        }
                        update_clock(&clock_proccess1,clock_ptr);

                    }
                    else
                    {
                        std::cout <<"not found in clock1" << std::endl;
                    }                

                }
                else if(exist == 0)
                {
                    page_fault++;
                    read_from_disk = read_from_disk + 1;
                    int place;
                    for(int i = 0;i < frame_num;i++)
                    {
                        if(frame[i] == 0)
                        {
                            free_space = 1;
                            place = i;
                            break;
                        }
                    }
                    if(free_space == 1)
                    {
                        new_hash = create_new_hash(page,place,read);
                        add_hash(&hash_table2[hash_num],new_hash);
                        frame[place] = 1;
                        new_clock = create_clock_node(page);
                        add_clock_node(&clock_proccess1,new_clock);
                        free_space = 0;
                    }
                    else if(free_space == 0)
                    {
                        frame_rmv = -1;
                        if(clock_proccess1 != NULL )
                        {
                            remove = clock_remove(&clock_proccess1);
                            hash_num1 = Hash(remove);
                            frame_rmv = find_frame(hash_table2[hash_num1],remove);
                            if(frame_rmv == -1)
                                std::cout << "error" << std::endl;
                        }

                        if(frame_rmv == -1)
                        {
                            remove = clock_remove(&clock_proccess);
                            hash_num1 = Hash(remove);
                            frame_rmv = find_frame(hash_table[hash_num1],remove);
                            if(frame_rmv == -1)
                                std::cout << "error" << std::endl;
                            if(modified(hash_table[hash_num1],remove) == 0)
                                write_in_disk++;
                            else if(modified(hash_table[hash_num1],remove) == -1)
                                std::cout << "cant find read or write" << std::endl;
                            remove_hash(&hash_table[hash_num1],remove);
                            new_hash = create_new_hash(page,frame_rmv,read);
                            add_hash(&hash_table2[hash_num],new_hash);
                            new_clock = create_clock_node(page);
                            add_clock_node(&clock_proccess1,new_clock);
                            continue;
                        }
                        if(modified(hash_table2[hash_num1],remove) == 0)
                            write_in_disk++;
                        else if(modified(hash_table2[hash_num1],remove) == -1)
                            std::cout << "cant find read or write" << std::endl;
                        remove_hash(&hash_table2[hash_num1],remove);
                        new_hash = create_new_hash(page,frame_rmv,read);
                        add_hash(&hash_table2[hash_num],new_hash);
                        new_clock = create_clock_node(page);
                        add_clock_node(&clock_proccess1,new_clock);
                    }
                }
            }
            trace = trace + 2*q;
        }
        delete_clock(clock_proccess);
        delete_clock(clock_proccess1);
    }

    for(int i = 0;i< 4096;i++)
    {
        delete_hash(&hash_table[i]);
        delete_hash(&hash_table2[i]);
    }

    for(int i = 0;i < frame_num;i++)
    {
        if(frame[i] == 0)
            free_frames++;
    }

    
    std::cout << "number of writes to disk:"<< write_in_disk << std::endl; 
    std::cout << "number of reads from disk:" << read_from_disk << std::endl; 
    std::cout << "number of page faults:" << page_fault << std::endl;
    std::cout << "input number from file 1:" << trace_num << std::endl;
    std::cout << "input number from file 2:" << trace_num1 << std::endl;
    std::cout << "total frames:" << frame_num << std::endl;
    return 0;
}

unsigned int Hash(std::string page)
{
    int max = 4096;

    unsigned int hash = 5381;
    for (size_t i = 0; i < page.size(); ++i)
        hash = 33 * hash + (unsigned char)page[i];
    hash = hash % max;
    return hash;

}