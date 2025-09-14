#pragma once

class game{
private:
    Table_Node field;

    /*
      x_left|x_right
    y_top   |   y_top
    ________|________
    y_bottom|y_bottom
            |     
      x_left|x_right
    */

    int16_t x_left;
    int16_t x_right;
    int16_t y_top;
    int16_t y_bottom;

public:
    game() : x_left(0), x_right(0), y_top(0), y_bottom(0) {}

    void add_sand(int16_t x, int16_t y, uint64_t data){
        while(x < x_left){
            field.add_left_border();
            --x_left;
        }
        while(x > x_right){
            field.add_right_border();
            ++x_right;
        }
        while(y < y_bottom){
            field.add_bottom_border();
            --y_bottom;
        }
        while(y > y_top){
            field.add_top_border();
            ++y_top;
        }

        field.set_data(x-x_left, (y_top-y_bottom)-(y-y_bottom), data);
    }

    void cut_line(char* qwe, int16_t& x, int16_t& y, uint64_t& data){
        int i1=0;
        while(qwe[i1] != '\t'){
            ++i1;
        }
        x = char_to_int(qwe, 0, i1);

        int i2=i1+1;
        while(qwe[i2] != '\t'){
            ++i2;
        }
        y = char_to_int(qwe, i1+1, i2);

        while(qwe[i1] != '\0'){
            ++i1;
        }
        data = char_to_int(qwe, i2+1, i1);
    }

    void play(parser& my_parser, files_worker& my_files){
        my_files.files_open(my_parser._input);

        char* qwe;
        int16_t x, y;
        uint64_t data;

        // разброс по полю
        while(qwe = my_files.get_line()){
            if(*qwe == '\0'){
                break;
            }
            cut_line(qwe, x, y, data);
            // std::cout<<x<<' '<<y<<'\n';
            add_sand(x, y, data);
        }

        my_files.files_close();

        // начало клеточного автомата
        bool editing = false;
        for(int i=0; i<my_parser._max_iter; ++i){
            // steping(editing);
            field.crumbling(editing);

            if(my_parser._freq != 0){
                if(i%my_parser._freq==0){
                    my_files.write_bmp(i+1 , my_parser._output, field);
                }
            }

            if(!editing){
                if(my_parser._freq == 0){
                    my_files.write_bmp(i+1 , my_parser._output, field);
                }
                break;
            }
            
            editing = false;
        }
    }
};
