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

    }


    inline std::string test_form(const std::string&& input){
        return form_24bit_ansi(ansi_esc, true, std::make_tuple(200,100,200)) + input + get_reset();

    }





}//namespace sansic
#endif 