#include <iostream>
#include "lib/sansic.hpp"

int main(){

    std::cout << sansic::form("[Basic 24 bit colour syntax] (F255,0,0) Very red foreground. (B0,0,0)(F0,255,0)Very dark background, very green foreground. \n");
    std::cout << sansic::form("[Basic 24 bit colour syntax] [Various spacings should work] ( F 255 , 0  , 0) Very red foreground. (B0 ,0 ,0)(F 0,255, 0)Very dark background, very green foreground. \n");
    std::cout << sansic::form("[Combined 24 bit colour syntax] (F255,0,0,B0,0,255) Very red foreground, background very blue (B0,255,0,F255,0,0) Very green background, very red foreground \n");
    std::cout << sansic::form("[Combined 24 bit colour syntax] [Various spacings should work] (F 255, 0,0, B 0,0,255) Very red foreground, background very blue ( B 0,255,0,F255, 0,0 ) Very green background, very red foreground \n");


    std::cout << sansic::form("[Basic 24 bit colour syntax] [Lowercase F and B work as well]] (f255,0,0) Very red foreground. (b0-0-0)(f0_255|0)Very dark background, very green foreground. \n");
    std::cout << sansic::form("[Combined 24 bit colour syntax] [Lowercase F and B work as well]] (f255,100,200,b200,100,200) Combined syntax test with lower-case test \n");

    std::cout << "\n";

    std::cout << sansic::form("[Combined 24 bit colour syntax] [Different delimiters will work] (B255_255|255-F255-119|253) White background - pink foreground \n");
    std::cout << sansic::form("[Combined 24 bit colour syntax] [Different delimiters will work] (B255!255_255~F255,119.253) Should be same as above \n");
    std::cout << sansic::form("[Combined 24 bit colour syntax] [Different delimiters will work] (B255]255[255|F255?119!253) Any non-digit char should work as a delimiter (\\D) \n");
    std::cout << sansic::form("[Combined 24 bit colour syntax] [Different delimiters will work] (B255%255#255@F255@119@253) Any non-digit char should work as a delimiter (\\D) \n");

    std::cout << "\n";

    std::cout << sansic::form("[Basic 24 bit colour syntax] [Different delimiters will work] (B255%255#255@F255@119@253) Any non-digit char should work as a delimiter (\\D) \n");
    std::cout << sansic::form("[Basic 24 bit colour syntax] [Different delimiters will work] (B255/255~255_F255\\119-253) Any non-digit char should work as a delimiter (\\D) \n");
    std::cout << sansic::form("[Basic 24 bit colour syntax] [Different delimiters will work] (B25522551255_F25531192253) (Should not be tokenized) (\\D) \n");
    std::cout << sansic::form("[Basic 24 bit colour syntax] [Different delimiters will work] (F255|0_0) Very red foreground. (B0-0-0)(F0_255|0)Very dark background, very green foreground. \n");


    std::cout << "\n";

    std::cout << sansic::form("[Basic 24 bit colour syntax] [Values should naturally wrap around](F255,255,255)Foreground 255,255,255 \n");
    std::cout << sansic::form("[Basic 24 bit colour syntax] [Values should naturally wrap around](F300,300,300) 300,300,300 = 44,44,44 \n");
    std::cout << sansic::form("[Basic 24 bit colour syntax] [Values should naturally wrap around](F0,0,0)Foreground 0,0,0. \n");
    std::cout << sansic::form("[Basic 24 bit colour syntax] [Values should naturally wrap around](F-321,-321,-321)(Shouldnt be tokenized, doesnt fit regex.). \n");

    std::cout << "\n";

    std::cout << sansic::form("[Combined 24 bit colour syntax] [Values should naturally wrap around](F255,255,255,B255,255,255)Foreground & Background 255,255,255 \n");
    std::cout << sansic::form("[Combined 24 bit colour syntax] [Values should naturally wrap around](F900,900,900,B900,900,900) 900,900,900 should be 132,132,132 \n");
    std::cout << sansic::form("[Combined 24 bit colour syntax] [Values should naturally wrap around](F0,0,0,B0,0,0)Foreground & Background 0,0,0. \n");
    std::cout << sansic::form("[Combined 24 bit colour syntax] [Values should naturally wrap around](F,-321,-321,-321,B-321,-321,-321)(Shouldnt be tokenized, doesnt fit regex.). \n");

    std::cout << "\n";

    std::cout << sansic::form("[8 bit basic syntax] (F25) Some Text Here");
    std::cout << sansic::form("[8 bit basic syntax] [Various spacings should work]( F 255 ) Some Text Here");

    std::cout << sansic::form("[Combined 8 bit basic syntax] (F100,B200) Some Text Here");
    std::cout << sansic::form("[Combined 8 bit basic syntax][Various spacings should work] (F 100 ,B 200) Some Text Here");
    std::cout << sansic::form("[Combined 8 bit basic syntax][Different delimiters will work] (F100|B200) Some Text Here");
    std::cout << sansic::form("[Combined 8 bit basic syntax][Different delimiters will work] (F100_B200) Some Text Here");


    return 0;
}
