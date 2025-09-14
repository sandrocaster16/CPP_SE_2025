#pragma once
#include <iostream>

int len_char(const char* q);
int char_to_int(char* q, int start = 0, int end = -1);
bool found_int(char* q, int start = 0, int end = -1);
bool string_identity(const char* a, const char* b);

// таблица на списках
class Table_Node{
private:
    struct Node{
        uint64_t data;
        uint64_t stack;

        Node* top;
        Node* right;
        Node* bottom;
        Node* left;

        Node() : data(0), stack(0), top(nullptr),right(nullptr), bottom(nullptr), left(nullptr) {}
    };


    Node* start_table;
    uint16_t size_x;
    uint16_t size_y;

public:
    Table_Node() : start_table(new Node()), size_x(1), size_y(1) {}
    ~Table_Node(){
        Node* x_table = start_table;
        Node* y_table;
        Node* t;
        
        for(uint16_t x=0; x<size_x; ++x){
            y_table = x_table;
            x_table = x_table->right;
            
            for(uint16_t y=0; y<size_y; ++y){
                t = y_table;
                y_table = y_table->bottom;

                t->top = nullptr;
                t->right = nullptr;
                t->bottom = nullptr;
                t->left = nullptr;

                delete t;
            }
        }

        start_table = nullptr;
    }

    inline bool empty(){
        return (start_table == nullptr);
    }

    const uint16_t& get_width(){
        return size_x;
    }

    const uint16_t& get_height(){
        return size_y;
    }

    void add_top_border(){
        Node* t_bottom = start_table;
        start_table = new Node();
        start_table->bottom = t_bottom;
        t_bottom->top = start_table;

        Node* t = start_table;
        Node* t_old;

        for(uint16_t x=1; x<size_x; ++x){
            t->right = new Node();
            
            t_old = t;
            t = t->right;
            t->left = t_old;
            
            t_bottom = t_bottom->right;
            t->bottom = t_bottom;
            t_bottom->top = t;
        }

        ++size_y;
    }

    void add_right_border(){
        Node* t = start_table;
        Node* t_top;
        Node* t_right;

        for(uint16_t x=0; x<size_x-1; ++x){
            t = t->right;
        }

        t->right = new Node();
        t_right = t->right;
        t_right->left = t;

        for(uint16_t y=0; y<size_y-1; ++y){
            t_top = t->right;
            t = t->bottom;

            t->right = new Node();
            t_right = t->right;
            t_right->left = t;
            t_right->top = t_top;
            t_top->bottom = t_right;
        }

        ++size_x;
    }

    void add_bottom_border(){
        Node* t = start_table;
        Node* t_left;
        Node* t_bottom;

        for(uint16_t y=0; y<size_y-1; ++y){
            t = t->bottom;
        }

        t->bottom = new Node();
        t_bottom = t->bottom;
        t_bottom->top = t;

        for(uint16_t x=0; x<size_x-1; ++x){
            t_left = t->bottom;
            t = t->right;

            t->bottom = new Node();
            t_bottom = t->bottom;
            t_bottom->top = t;
            t_bottom->left = t_left;
            t_left->right = t_bottom;
        }

        ++size_y;
    }

    void add_left_border(){
        Node* t_right = start_table;
        start_table = new Node();
        start_table->right = t_right;
        t_right->left = start_table;

        Node* t = start_table;
        Node* t_old;

        for(uint16_t y=1; y<size_y; ++y){
            t->bottom = new Node();

            t_old = t;
            t = t->bottom;
            t->top = t_old;
            
            t_right = t_right->bottom;
            t->right = t_right;
            t_right->left = t;
        }

        ++size_x;
    }

    void print_table(){
        Node* t_old = start_table;
        Node* t;

        std::cout<<"size_x: "<<size_x<<" size_y: "<<size_y<<'\n';

        for(uint16_t y=0; y<size_y; ++y){
            t = t_old;
            for(uint16_t x=0; x<size_x; ++x){
                std::cout<<'['<<x<<','<<y<<"] "<<t->data<<" : "<<t->top<<' '<<t->right<<' '<<t->bottom<<' '<<t->left<<'\n';
                t = t->right;
            }
            t_old = t_old->bottom;
        }

        // for(int y=0; y<size_x; ++y){
        //     t = t_old;
        //     for(int x=0; x<size_y; ++x){
        //         std::cout<<'['<<x<<','<<y<<"] "<<t->data<<" : "<<t->top<<' '<<t->right<<' '<<t->bottom<<' '<<t->left<<'\n';
        //         t = t->bottom;
        //     }
        //     t_old = t_old->right;
        // }
    }

    void set_data(int16_t x, int16_t y, uint64_t data, bool qwe=false){
        Node* t = start_table;

        for(uint16_t _x=0; _x<x; ++_x){
            t = t->right;
        }

        for(uint16_t _y=0; _y<y; ++_y){
            t = t->bottom;
        }

        // std::cout<<x<<' '<<y<<' '<<data<<'\n';
        if(qwe) ++t->data;
        else t->data = data;
    }

    const uint64_t& get_data(int16_t x, int16_t y){
        Node* t = start_table;

        for(uint16_t _x=0; _x<x; ++_x){
            t = t->right;
        }

        for(uint16_t _y=0; _y<y; ++_y){
            t = t->bottom;
        }

        return t->data;
    }

    void crumbling(bool& editing){
        const int16_t _size_x = size_x;
        const int16_t _size_y = size_y;
        Node* t_old = start_table;
        Node* t;

        for(uint16_t _y=0; _y<_size_y; ++_y){
            t = t_old;
            for(uint16_t _x=0; _x<_size_x; ++_x){
                if(t->data > 3){
                    editing = true;
                    t->data -= 4;

                    if(!t->top){
                        add_top_border();
                    }
                    ++t->top->stack;

                    if(!t->right){
                        add_right_border();
                    }
                    ++t->right->stack;

                    if(!t->bottom){
                        add_bottom_border();
                    }
                    ++t->bottom->stack;

                    if(!t->left){
                        add_left_border();
                    }
                    ++t->left->stack;
                }

                t = t->right;
            }
            t_old = t_old->bottom;
        }

        //update;
        t_old = start_table;
        
        for(uint16_t _y=0; _y<size_y; ++_y){
            t = t_old;
            for(uint16_t _x=0; _x<size_x; ++_x){
                t->data += t->stack;
                t->stack = 0;
                t = t->right;
            }
            t_old = t_old->bottom;
        }
    }
};
