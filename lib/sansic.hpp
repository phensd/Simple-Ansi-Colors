#ifndef SANSIC_HPP
#define SANSIC_HPP
#include <cstdint>
#include <string>
#include <tuple>
#include <sstream>
#include <regex>

namespace sansic{



    //private details
    namespace internal {


        namespace util{
            std::tuple<int,int,int> conform_rgb_vals(std::tuple<int,int,int>&& rgb_vals);
        }

        std::string form_24bit_ansi(const std::string& delim, bool is_foreground,std::tuple<std::uint8_t,std::uint8_t,std::uint8_t>& rgb_vals);
        std::string form_8bit_ansi(const std::string& delim, bool is_foreground,std::uint8_t color_val);

        void do_rgb_normal(std::smatch& components, const std::string& full_token,std::string& input, int& index);
        void do_rgb_combined(std::smatch& components,const std::string& full_token,std::string& input, int& index);

        void do_8bit_normal(std::smatch& components, const std::string& full_token,std::string& input, int& index);


        void parse_token(const std::string& full_token,std::string& input, int&& index); 

        const std::string get_reset();



    }//end of private details


    //start of public implementation
    std::string form(std::string input);





}//namespace sansic
#endif 