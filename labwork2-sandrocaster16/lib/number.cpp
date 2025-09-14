#include "number.h"


uint239_t FromInt(uint32_t value, uint32_t shift) {
    uint239_t q;

    for(int i=0; i<32; ++i){
        q.data[34-i] |= (((shift>>i)&1)<<7); // запись shift
        q.data[34-((shift+i)%245/7)] |= (((value>>i)&1)<<((shift+i)%7)); // запись value 
    }

    return q;
}

uint239_t FromString(const char* str, uint32_t shift) {
    uint239_t result;

    for(int i=0; i<32; i++){
        result.data[34-i] |= (((shift>>i)&1)<<7);   // запись shift
    }

    //махинации с чарами
    int _len = 0, dec, ii=0;
    uint8_t* str1;
    uint8_t* str2 = new uint8_t[73]{};

    while(*str){
        str2[_len] = (*str-'0');
        str++;
        _len++;
    }

    str1 = new uint8_t[_len];
    for(int i=0; i<_len; ++i){
        str1[_len-i-1] = str2[i];
    }
    delete[] str2;

    while(true){
        dec = 0;

        if(str1[_len-1] == 0){
            _len--;
            if(_len == 0){
                break;
            }
        }

        str2 = new uint8_t[_len]{0};

        for(int i=_len-1; i>=0; --i){
            dec += str1[i];
            str2[i] = dec/2;
            dec = dec%2*10;
        }

        delete[] str1;
        str1 = str2;

        result.data[ 34-((shift+ii)%245/7) ] |= (dec/10)<<((shift+ii)%245%7);
        ++ii;
    }

    delete[] str1;
    return result;
}

uint239_t operator+(const uint239_t& lhs, const uint239_t& rhs) {
    uint64_t lhs_shift = GetShift(lhs);
    uint64_t rhs_shift = GetShift(rhs);
    uint64_t new_shift = (lhs_shift+rhs_shift) % 34359738367;

    uint239_t result;
    uint8_t stack = 0;
    uint8_t lhs_bit, rhs_bit, result_bit;

    for(int i=0; i<34; ++i){
        result.data[34-i] |= (((new_shift>>i)&1)<<7);
        for(int j=0; j<7; ++j){
            lhs_bit = ((lhs.data[ 34-(((lhs_shift+j)%245)/7+i)%35 ] >> ((j+lhs_shift)%7))&1);
            rhs_bit = ((rhs.data[ 34-(((rhs_shift+j)%245)/7+i)%35 ] >> ((j+rhs_shift)%7))&1);
            result_bit = (lhs_bit + rhs_bit + stack)%2;
            
            stack = (lhs_bit + rhs_bit + stack)/2;
            result.data[ 34-(((new_shift+j)%245)/7+i)%35 ] |= (result_bit << ((j+new_shift)%7));
        }
    }
    // last bit
    lhs_bit = ((lhs.data[ 34-(((lhs_shift)%245)/7+34)%35 ] >> ((lhs_shift)%7))&1);
    rhs_bit = ((rhs.data[ 34-(((rhs_shift)%245)/7+34)%35 ] >> ((rhs_shift)%7))&1);
    result_bit = (lhs_bit + rhs_bit + stack)%2;
    
    result.data[ 34-(((new_shift)%245)/7+34)%35 ] |= (result_bit << ((new_shift)%7));
        
    return result;  
}

uint239_t operator-(const uint239_t& lhs, const uint239_t& rhs) {
    uint64_t lhs_shift = GetShift(lhs);
    uint64_t rhs_shift = GetShift(rhs);
    uint64_t new_shift = (lhs_shift-rhs_shift+34359738367) % 34359738367;

    uint239_t result;
    uint8_t stack = 0;
    uint8_t lhs_bit, rhs_bit, result_bit;

    for(int i=0; i<34; ++i){
        result.data[34-i] |= (((new_shift>>i)&1)<<7);

        for(int j=0; j<7; ++j){
            lhs_bit = ((lhs.data[ 34-(((lhs_shift+j)%245)/7+i)%35] >> ((j+lhs_shift)%7))&1);
            rhs_bit = ((rhs.data[ 34-(((rhs_shift+j)%245)/7+i)%35] >> ((j+rhs_shift)%7))&1);
            result_bit = (lhs_bit - rhs_bit - stack + 2)%2;
            
            stack = (lhs_bit < rhs_bit + stack) ? 1 : 0;
            result.data[ 34-(((new_shift+j)%245)/7+i)%35] |= (result_bit << ((j+new_shift)%7));
        }
    }
    // lsat bit
    lhs_bit = ((lhs.data[ 34-(((lhs_shift)%245)/7+34)%35] >> ((lhs_shift)%7))&1);
    rhs_bit = ((rhs.data[ 34-(((rhs_shift)%245)/7+34)%35] >> ((rhs_shift)%7))&1);
    result_bit = (lhs_bit - rhs_bit - stack + 2)%2;
    
    result.data[ 34-(((new_shift)%245)/7+34)%35] |= (result_bit << ((new_shift)%7));
        
    return result;
}

uint239_t operator*(const uint239_t& lhs, const uint239_t& rhs) {
    uint64_t lhs_shift = GetShift(lhs);
    uint64_t rhs_shift = GetShift(rhs);
    uint64_t new_shift = (lhs_shift+rhs_shift) % 34359738367;

    uint239_t rhs_shift_zero = SetShift0(rhs);
    uint239_t result = FromInt(0, new_shift);
    uint8_t lhs_bit;

    for(int i=0; i<34; ++i){
        for(int j=0; j<7; ++j){
            lhs_bit = ((lhs.data[ 34-(((lhs_shift+j)%245)/7+i)%35 ] >> ((j+lhs_shift)%7))&1);
            
            if(lhs_bit == 1){
                result = result + rhs_shift_zero;
            }

            rhs_shift_zero = ShiftUp(rhs_shift_zero);
        }
    }

    return result;
}

uint239_t operator/(const uint239_t& lhs, const uint239_t& rhs) {
    if(SetShift0(rhs) == FromInt(0, 0)){
        throw std::invalid_argument("Division by zero");
    }

    uint64_t lhs_shift = GetShift(lhs);
    uint64_t rhs_shift = GetShift(rhs);
    uint64_t new_shift = (lhs_shift-rhs_shift+34359738367) % 34359738367;

    uint239_t rhs_shift_zero = SetShift0(rhs);
    uint239_t result;
    uint239_t remainder = lhs;

    if(SetShift0(rhs) == FromInt(1, 0)){
        return FromInt(0, new_shift) + SetShift0(lhs);
    }

    if(remainder >= rhs_shift_zero){
        result = FromInt(1, new_shift);
        remainder = remainder - rhs_shift_zero;

        while(remainder >= rhs_shift_zero){
            result = result + FromInt(1, 0);
            remainder = remainder - rhs_shift_zero;
        }
    }

    return result;
}

bool operator==(const uint239_t& lhs, const uint239_t& rhs) {
    uint64_t lhs_shift = GetShift(lhs);
    uint64_t rhs_shift = GetShift(rhs);

    uint8_t lhs_bit, rhs_bit;

    for(int i=0; i<35; ++i){
        for(int j=0; j<7; ++j){
            lhs_bit = ((lhs.data[ 34-(((lhs_shift+j)%245)/7+i)%35 ] >> ((j+lhs_shift)%7))&1);
            rhs_bit = ((rhs.data[ 34-(((rhs_shift+j)%245)/7+i)%35 ] >> ((j+rhs_shift)%7))&1);
        
            if(lhs_bit != rhs_bit){
                return false;
            }
        }
    }

    return true;
}

bool operator!=(const uint239_t& lhs, const uint239_t& rhs) {
    return !(lhs==rhs);
}

std::ostream& operator<<(std::ostream& stream, const uint239_t& value){    
    // десятичка
    uint64_t shift = GetShift(value);

    uint8_t  result[72]{};
    uint8_t res_bin[72]{};
    int dec;
    res_bin[0] = 1;

    for(int i=0; i<35; ++i){
        for(int j=0; j<7; ++j){
            // если в двоичке 1, то плюсуем степень двойки к ответу
            if(((value.data[34-(((shift+j)%245)/7+i)%35]>>(j+shift)%7)&1) == 1){
                dec = 0;
                for(int q=0; q<72; ++q){
                    int y = result[q] + res_bin[q] + dec;
                    int yy = y%10;
                    result[q] = yy; 
                    dec = (y-yy)/10;
                }
            }

            // делаем тоже самое но на каждой итерации с степенью двойки
            dec = 0;
            for(int q=0; q<72; ++q){
                int y = res_bin[q]*2 + dec;
                int yy = y%10;
                res_bin[q] = yy;
                dec = (y-yy)/10;
            }
        }
    }

    bool e = false;
    for(int i=71; i>=0; --i){
        if(result[i] != 0){
            e = true;
        }

        if(e){
            stream<<+result[i];
        }
    }
    if(!e){
        stream<<0;
    }

    return stream;
}

uint64_t GetShift(const uint239_t& value) {
    uint64_t q = 0;
    for(int i=34; i>=0; --i){
        q |= ((value.data[i]>>7)<<(34-i));
    }

    return q;
}

uint239_t ShiftUp(const uint239_t& value){
    uint64_t shift = GetShift(value);
    uint239_t result;
    uint8_t last_bit = ((value.data[0]>>6)&1);

    for(int i=0; i<35; ++i){
        result.data[34-i] |= (((shift>>i)&1)<<7);

        for(int j=5; j>=0; --j){
            result.data[i] |= ((value.data[i]>>j)&1)<<(j+1);
        }

        if(i == 34){
            result.data[34] |= last_bit;
        } 
        else{
            result.data[i] |= ((value.data[i+1]>>6)&1);
        }
        
    }

    return result;
}

uint239_t SetShift0(const uint239_t& value){
    uint239_t result;
    uint64_t shift = GetShift(value);

    for(int i=0; i<34; ++i){
        for(int j=0; j<7; ++j){
            result.data[34-i] |= (((value.data[ 34-(((shift+j)%245)/7+i)%35 ] >> ((j+shift)%7))&1) << j);
        }
    }

    return result;
}

bool operator>(const uint239_t& lhs, const uint239_t& rhs) {
    uint64_t lhs_shift = GetShift(lhs);
    uint64_t rhs_shift = GetShift(rhs);

    uint8_t lhs_bit, rhs_bit;

    for(int i=34; i>=0; --i){
        for(int j=6; j>=0; --j){
            lhs_bit = ((lhs.data[ 34-(((lhs_shift+j)%245)/7+i)%35] >> ((j+lhs_shift)%7))&1);
            rhs_bit = ((rhs.data[ 34-(((rhs_shift+j)%245)/7+i)%35] >> ((j+rhs_shift)%7))&1);

            if(lhs_bit > rhs_bit){
                return true;
            }
            if(lhs_bit < rhs_bit){
                return false;
            }
        }
    }

    return false;
}

bool operator>=(const uint239_t& lhs, const uint239_t& rhs) {
    return (lhs > rhs || lhs == rhs);
}

//cmake --build . --target number_tests && ctest -V