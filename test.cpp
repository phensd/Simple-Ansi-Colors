#include <iostream>
#include "include/sansic.hpp"

int main(){

    std::cout << sansic::form("[Basic 24 bit coloursyntax] (F255,0,0) Very red foreround. (B0,0,0)(F0,255,0)Very dark background, very green foreground. \n");
    std::cout << sansic::form("[Combined 24 bit colour syntax] (F255,0,0,B0,0,255) Very red foreround, background very blue (B0,255,0,F255,0,0) Very green background, very red foreground \n");
    std::cout << sansic::form("[Combined 24 bit colour syntax] [Different delimiters] (B255_255|255-F255-119|253) White background - pink foreground \n");

    return 0;
}
