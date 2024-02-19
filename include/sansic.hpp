#ifndef SANSIC_HPP
#define SANSIC_HPP
#include <string>
#include <tuple>
#include <sstream>
#include <regex>
#include <iostream>

//public 
namespace sansic{



    //private details
    namespace {
        
        enum class TOKEN_TYPE{
            RGB_COLOR,
            FG,
            BG,
            NONE
        };

        auto form_24bit_ansi = [](std::string delim, bool is_foreground,std::tuple<std::string,std::string,std::string> rgb_vals) -> std::string{
            std::stringstream output;

            output << delim << (is_foreground ? 38 : 48) << ";" << "2" <<  ';' << std::get<0>(rgb_vals) << ';' << std::get<1>(rgb_vals)  << ';' << std::get<2>(rgb_vals) << "m";
            
            return output.str();

        };



        inline const std::string ansi_esc {"\033["};

        //regex for csv values
        //example matches: [200,300,200] [300, 200  ,100] [20,10,000]
        //groups divided into 1-3 integers
        inline const std::regex rgb_csv_regex{"\\(\\s*(F|B)(\\d{1,3})\\s*,\\s*(\\d{1,3})\\s*,\\s*(\\d{1,3})\\s*\\)$"};

        inline const char token_start {'('};
        inline const char token_end {')'};
        



        std::string get_reset(){
            return ansi_esc + "0m";
        }
        
        TOKEN_TYPE get_token_type(const std::string& input){ 

            if(std::regex_match(input,rgb_csv_regex)) return TOKEN_TYPE::RGB_COLOR;


            //default, shouldnt happen!
            return TOKEN_TYPE::NONE;

        }



    }


    inline std::string form(std::string input){

        for(size_t i {0}; i < input.size(); ++i){

            if(input.at(i) == token_start){

                auto pos = input.find(token_end,i)+1;
                auto full_token {input.substr(i,pos-i)};

                auto token_type {get_token_type(full_token)};


                if(token_type == TOKEN_TYPE::NONE) continue;

                if(token_type == TOKEN_TYPE::RGB_COLOR){
                    std::smatch components;
                    std::regex_match(full_token,components,rgb_csv_regex);
                    std::string reset {get_reset()};

                    std::string replace {form_24bit_ansi(ansi_esc,components[1] == "F",std::make_tuple(components[2],components[3],components[4]))};

                    input.replace(i,full_token.size(),replace);

                    input += reset;


                }


            }

        }

        return input;

    }





}//namespace sansic
#endif 