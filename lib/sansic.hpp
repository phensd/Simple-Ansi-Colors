#ifndef SANSIC_HPP
#define SANSIC_HPP
#include <cstdint>
#include <string>
#include <tuple>
#include <sstream>
#include <regex>
#include <optional>

namespace sansic{



    //private details
    namespace internal {


        namespace util{
            using otu8t = std::optional<std::tuple<std::uint8_t,std::uint8_t,std::uint8_t>>;

        }

        std::string form_24bit_ansi(const std::string& delim, bool is_foreground,std::tuple<std::uint8_t,std::uint8_t,std::uint8_t>& rgb_vals);
        std::string form_8bit_ansi(const std::string& delim, bool is_foreground,std::uint8_t color_val);

        bool smatch_is_foreground_insensitive(const std::string& smatch);

        void do_rgb(
        const std::string& full_token,
        std::string& input,
        int& index,
        bool is_foreground,
        std::tuple<std::uint8_t,std::uint8_t,std::uint8_t> components_lhs,
        std::optional<std::tuple<std::uint8_t,std::uint8_t,std::uint8_t>> components_rhs = std::nullopt);
        
        void do_8bit(
        const std::string& full_token,
        std::string& input, 
        int& index,
        bool is_foreground,
        std::uint8_t color_value_lhs, 
        std::optional<std::uint8_t> color_value_rhs = std::nullopt);


        void parse_token(const std::string& full_token,std::string& input, int&& index); 

        const std::string get_reset();



    }//end of private details


    //start of public implementation
    std::string form(std::string input);





}//namespace sansic
#endif 