#ifndef SANSIC_HPP
#define SANSIC_HPP
#include <string>
#include <tuple>
#include <sstream>
#include <regex>

namespace sansic{



    //private details
    namespace internal {
        

        std::string form_24bit_ansi(std::string delim, bool is_foreground,std::tuple<std::string,std::string,std::string> rgb_vals);

        void do_rgb_normal(std::smatch components, const std::string& full_token,std::string& input, int& index);

        void do_rgb_combined(std::smatch components,const std::string& full_token,std::string& input, int& index);

        void parse_token(const std::string& full_token,std::string& input, int&& index); 

        const std::string get_reset();



    }//end of private details


    //start of public implementation
    std::string form(std::string input);





}//namespace sansic
#endif 