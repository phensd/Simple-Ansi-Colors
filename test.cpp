#include <iostream>
#include "lib/sansic.hpp"

int main(){

    std::cout << sansic::form("[Basic 24 bit colour syntax] (F255,0,0) Very red foreground. (B0,0,0)(F0,255,0)Very dark background, very green foreground. \n");

    std::cout << sansic::form("[Combined 24 bit colour syntax] (F255,0,0,B0,0,255) Very red foreground, background very blue (B0,255,0,F255,0,0) Very green background, very red foreground \n");
    std::cout << sansic::form("[Combined 24 bit colour syntax] [Different delimiters will work] (B255_255|255-F255-119|253) White background - pink foreground \n");
    std::cout << sansic::form("[Basic 24 bit colour syntax] [Each RGB val should be > 0 and < 256 regardless](F255,255,255)Foreground 255,255,255 \n");
    std::cout << sansic::form("[Basic 24 bit colour syntax] [Each RGB val should be > 0 and < 256 regardless](F300,300,300)Should be same as above. 900,900,900 \n");
    std::cout << sansic::form("[Basic 24 bit colour syntax] [Each RGB val should be > 0 and < 256 regardless](F0,0,0)Foreground 0,0,0. \n");
    std::cout << sansic::form("[Basic 24 bit colour syntax] [Each RGB val should be > 0 and < 256 regardless](F-321,-321,-321)(Shouldnt be tokenized, doesnt fit regex.). \n");

    return 0;
}
