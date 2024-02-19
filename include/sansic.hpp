#ifndef SANSIC_HPP
#define SANSIC_HPP
#include <string>
#include <tuple>
#include <iostream>
#include <format>


//public 
namespace sansic{



    //private details
    namespace {
        
        enum class TOKEN_TYPE{
            RGB_COLOR,
            FG,
            BG
        };

        inline auto form_24bit_ansi = [](std::string delim, bool is_foreground,std::tuple<int,int,int> rgb_vals) -> std::string{
            std::string output {

                std::format("{0}{1};2;{2};{3};{4}m",
                delim,
                is_foreground ? 38 : 48,
                std::get<0>(rgb_vals),
                std::get<1>(rgb_vals),
                std::get<2>(rgb_vals)
                )

            };
            return output;

        };



        inline std::string ansi_esc {"\033["};


        std::string get_reset(){
            return ansi_esc + "0m";
        }
        
        TOKEN_TYPE get_token_type(const std::string& input){
            std::cout << input;


            return TOKEN_TYPE::RGB_COLOR;




        }



    }


    inline std::string form(const std::string&& input){
        bool is_next_fg {false};

        for(size_t i {0}; i < input.size(); ++i){
            if(input.at(i) == '['){
                auto token_type {get_token_type(input.substr(i,input.find(']')+1))};
            }

        }
        //return form_24bit_ansi(ansi_esc, true, std::make_tuple(200,100,200)) + input + get_reset();

        return " ";

    }





}//namespace sansic
#endif 