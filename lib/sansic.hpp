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
        }

        std::string form_24bit_ansi(const std::string& delim, bool is_foreground,std::tuple<std::uint8_t,std::uint8_t,std::uint8_t>& rgb_vals);
        std::string form_8bit_ansi(const std::string& delim, bool is_foreground,std::uint8_t color_val);

        void do_rgb(std::smatch& components, const std::string& full_token,std::string& input, int& index,bool combined);
        // void do_rgb_combined(std::smatch& components,const std::string& full_token,std::string& input, int& index);

        bool smatch_is_foreground_insensitive(const std::string& smatch);

        void do_8bit_normal(std::smatch& components, const std::string& full_token,std::string& input, int& index);


        void parse_token(const std::string& full_token,std::string& input, int&& index); 

        const std::string get_reset();



    }//end of private details


    //start of public implementation
    std::string form(std::string input);





}//namespace sansic
#endif 