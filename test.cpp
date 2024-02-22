#include <iostream>
#include "lib/sansic.hpp"

int main(){

    std::cout << sansic::form("[Basic 24 bit colour syntax] (F255,0,0) Very red foreground. (B0,0,0)(F0,255,0)Very dark background, very green foreground. \n");
    std::cout << sansic::form("[Basic 24 bit colour syntax] [Different delimiters will work] (F255|0_0) Very red foreground. (B0-0-0)(F0_255|0)Very dark background, very green foreground. \n");

    std::cout << "\n";

    std::cout << sansic::form("[Combined 24 bit colour syntax] (F255,0,0,B0,0,255) Very red foreground, background very blue (B0,255,0,F255,0,0) Very green background, very red foreground \n");
    std::cout << sansic::form("[Combined 24 bit colour syntax] [Different delimiters will work] (B255_255|255-F255-119|253) White background - pink foreground \n");
    std::cout << sansic::form("[Combined 24 bit colour syntax] [Different delimiters will work] (B255!255_255~F255,119.253) Should be same as above \n");
    std::cout << sansic::form("[Combined 24 bit colour syntax] [Different delimiters will work] (B255]255[255|F255?119!253) Any non-digit char should work as a delimiter (\\D) \n");
    std::cout << sansic::form("[Combined 24 bit colour syntax] [Different delimiters will work] (B255%255#255@F255@119@253) Any non-digit char should work as a delimiter (\\D) \n");
    std::cout << sansic::form("[Basic 24 bit colour syntax] [Different delimiters will work] (B255%255#255@F255@119@253) Any non-digit char should work as a delimiter (\\D) \n");
    std::cout << sansic::form("[Basic 24 bit colour syntax] [Different delimiters will work] (B255/255~255_F255\119-253) Any non-digit char should work as a delimiter (\\D) \n");

    std::cout << "\n";

    std::cout << sansic::form("[Basic 24 bit colour syntax] [Each RGB val should be > 0 and < 256 regardless](F255,255,255)Foreground 255,255,255 \n");
    std::cout << sansic::form("[Basic 24 bit colour syntax] [Each RGB val should be > 0 and < 256 regardless](F300,300,300)Should be same as above. set as 900,900,900 \n");
    std::cout << sansic::form("[Basic 24 bit colour syntax] [Each RGB val should be > 0 and < 256 regardless](F0,0,0)Foreground 0,0,0. \n");
    std::cout << sansic::form("[Basic 24 bit colour syntax] [Each RGB val should be > 0 and < 256 regardless](F-321,-321,-321)(Shouldnt be tokenized, doesnt fit regex.). \n");

    std::cout << "\n";

    std::cout << sansic::form("[Combined 24 bit colour syntax] [Each RGB val should be > 0 and < 256 regardless](F255,255,255,B255,255,255)Foreground & Background 255,255,255 \n");
    std::cout << sansic::form("[Combined 24 bit colour syntax] [Each RGB val should be > 0 and < 256 regardless](F900,900,900,B900,900,900)Should be same as above. set as F&B set to 900,900,900 \n");
    std::cout << sansic::form("[Combined 24 bit colour syntax] [Each RGB val should be > 0 and < 256 regardless](F0,0,0,B0,0,0)Foreground & Background 0,0,0. \n");
    std::cout << sansic::form("[Combined 24 bit colour syntax] [Each RGB val should be > 0 and < 256 regardless](F,-321,-321,-321,B-321,-321,-321)(Shouldnt be tokenized, doesnt fit regex.). \n");

    std::cout << "\n";


    return 0;
}
