#pragma once
#include <fstream>

class files_worker{
private:
    std::ifstream file_read;

    #pragma pack(push, 1)

    struct Color {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
        uint8_t reserved;
    };

    struct BMPHeader {
        uint16_t bfType;
        uint32_t bfSize;
        uint16_t bfReserved1;
        uint16_t bfReserved2;
        uint32_t bfOffBits;
    };

    struct DIBHeader {
        uint32_t biSize;
        int32_t biWidth;
        int32_t biHeight;
        uint16_t biPlanes;
        uint16_t biBitCount;
        uint32_t biCompression;
        uint32_t biSizeImage;
        int32_t biXPelsPerMeter;
        int32_t biYPelsPerMeter;
        uint32_t biClrUsed;
        uint32_t biClrImportant;
    };

    #pragma pack(pop)

    Color palette[5] = {
        {255, 255, 255, 0}, // 0  - Белый
        {0,   255, 0,   0}, // 1  - Зеленый
        {0,   255, 255, 0}, // 2  - Фиолетовый
        {255, 0,   255, 0}, // 3  - Желтый
        {0,   0,   0,   0}, // 4+ - Черный
    };

public:
    files_worker() : file_read(nullptr) {}

    void files_open(const char* _input=nullptr, const char* _output=nullptr){
        if(_input != nullptr){
            file_read.open(_input);
            if(!file_read.is_open()){
                std::cerr<<"Error: Could not open input file "<<_input<<'\n';
                throw;
            }
        }
    }

    void files_close(){
        if(file_read.is_open()){
            file_read.close();
        }
    }

    char* get_line(){
        char q;
        char* qwe = new char[516]{'\0'};
        int _len=0;

        while( (q = file_read.get()) ){
            if(q == EOF && _len == 0){
                // std::cout<<"\nTHE END\n";
                delete[] qwe;
                return nullptr;
            }

            if(q == '\n' || q == EOF){
                break;
            }

            qwe[_len] = q;
            _len++;
        }
        qwe[_len] = '\0';

        return qwe;
    }

    void write_bmp(int name, const char* dir, Table_Node& field){
        // name bmp
        char* name_bmp = new char[64];
        int ii=0;

        while(*dir != '\0'){
            name_bmp[ii++] = *dir;
            dir++;
        }
        name_bmp[ii++] = '/';

        char* name_reverse = new char[64];
        int j=0;
        while(name != 0){
            name_reverse[j++] = static_cast<char>((name%10)+'0');
            name/=10;
        }
        name_reverse[j] = '\0';
        int _len = len_char(name_reverse);
        for(int i=0; i<_len; ++i){
            name_bmp[ii++] = name_reverse[_len-i-1];
        }

        name_bmp[ii++] = '.';
        name_bmp[ii++] = 'b';
        name_bmp[ii++] = 'm';
        name_bmp[ii++] = 'p';
        name_bmp[ii++] = '\0';
        

        // write bmp
        int16_t width = field.get_width();
        int16_t height = field.get_height();

        unsigned int rowSize = width / 2 + width % 2;
        unsigned int paddingPerRow = (4 - rowSize % 4) % 4;
        unsigned int totalDataSize = (rowSize + paddingPerRow) * height;

        BMPHeader fileHeader = {0};
        DIBHeader infoHeader = {0};

        fileHeader.bfType = 0x4D42;
        fileHeader.bfSize = totalDataSize + sizeof(palette) + sizeof(infoHeader) + sizeof(fileHeader);
        fileHeader.bfOffBits = sizeof(palette) + sizeof(infoHeader) + sizeof(fileHeader);

        infoHeader.biSize = sizeof(infoHeader);
        infoHeader.biWidth = width;
        infoHeader.biHeight = height;
        infoHeader.biPlanes = 1;
        infoHeader.biBitCount = 4;
        infoHeader.biCompression = 0;
        infoHeader.biSizeImage = totalDataSize;
        infoHeader.biXPelsPerMeter = 0;
        infoHeader.biYPelsPerMeter = 0;
        infoHeader.biClrUsed = 5;
        infoHeader.biClrImportant = 5;

        std::ofstream file(name_bmp, std::ios::binary);

        file.write((char*) &fileHeader, sizeof(fileHeader));
        file.write((char*) &infoHeader, sizeof(infoHeader));
        file.write((char*) palette, sizeof(palette));

        uint8_t* pixelData = new uint8_t[totalDataSize];
        uint8_t* currentPixel = pixelData;

        // field.print_table();

        for (int16_t i = height - 1; i >= 0; --i) {
            for (int16_t j = 0; j < width; j += 2) {
                // std::cout<<height<<' '<<width<<'\n';
                uint64_t first_pixel = field.get_data(j, i);
                uint64_t second_pixel = 4;
                if (j + 1 < width) {
                    second_pixel = std::min((long long) field.get_data(j+1, i), 4ll);
                }
                *currentPixel = static_cast<uint8_t>(first_pixel << 4 | second_pixel);
                ++currentPixel;
            }
            currentPixel += paddingPerRow;
        }

        file.write((char*) (pixelData), totalDataSize);
        file.close();

        delete[] pixelData;
        delete[] name_bmp;
        delete[] name_reverse;
    }
};
