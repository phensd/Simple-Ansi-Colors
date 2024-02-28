#include "sansic.hpp"
#include <iostream>
#include <optional>
#include <regex>

    //anon namespace for some things that dont need to be seen elsewhere
    namespace {

        //beginning and end of tokens we parse
        const char token_start {'('};
        const char token_end {')'};


        //the beginning of an ANSI escape code
        //https://en.wikipedia.org/wiki/ANSI_escape_code
        const std::string ansi_esc {"\033["};

        //regex for csv values
        //example matches: [200,300,200] [300, 200  ,100] [20,10,000]
        //groups divided into 1-3 integers
        const std::regex rgb_normal_regex{"\\(\\s*(F|B|f|b)\\s*(\\d{1,3})\\s*\\D\\s*(\\d{1,3})\\s*\\D\\s*(\\d{1,3})\\s*\\)$"};


        //same as above, but allows setting both background and foreground at once
        //ex. (F200,100,200,B200,100,200)
        const std::regex rgb_combined_regex{"\\(\\s*(F|B|f|b)\\s*(\\d{1,3})\\s*\\D\\s*(\\d{1,3})\\s*\\D\\s*(\\d{1,3})\\s*\\D\\s*(F|B|f|b)\\s*(\\d{1,3})\\s*\\D\\s*(\\d{1,3})\\s*\\D\\s*(\\d{1,3})\\s*\\)$"};


        //regex for 8 bit colors ex. (F28)
        const std::regex r8bit_normal_regex {"\\(\\s*(F|B|f|b)\\s*(\\d{0,3})\\s*\\)$"};

        //same as above, but with B aswell. ex. (F100,b200)
        const std::regex r8bit_combined_regex {"\\(\\s*(F|B|f|b)\\s*(\\d{0,3})\\s*\\D\\s*(F|B|f|b)\\s*(\\d{0,3})\\s*\\)$"};

        
    }//end of anon namespace

//internal





//forms a 24 bit ansi escape code
//https://en.wikipedia.org/wiki/ANSI_escape_code#24-bit
std::string sansic::internal::form_24bit_ansi(const std::string& delim, bool is_foreground,std::tuple<std::uint8_t,std::uint8_t,std::uint8_t>& rgb_vals){

    std::stringstream output;

    output << delim << (is_foreground ? 38 : 48) << ";" << "2" <<  ';' << static_cast<int>(std::get<0>(rgb_vals)) << ';' << static_cast<int>(std::get<1>(rgb_vals))  << ';' << static_cast<int>(std::get<2>(rgb_vals)) << "m";
    
    return output.str();

}

//https://en.wikipedia.org/wiki/ANSI_escape_code#8-bit
std::string sansic::internal::form_8bit_ansi(const std::string& delim, bool is_foreground,std::uint8_t color_val){

    std::stringstream output;

    output << delim << (is_foreground ? 38 : 48) << ";" << "5" <<  ';' << static_cast<int>(color_val) << "m";
    
    return output.str();

}





bool sansic::internal::smatch_is_foreground_insensitive(const std::string& smatch){
    return (smatch == "F" or smatch == "f");
}


//takes syntax such as (F200,300,100) and creates a 24 bit ansi code out of it
void sansic::internal::do_rgb(
    const std::string& full_token,
    std::string& input, 
    int& index,
    bool is_foreground,
    std::tuple<std::uint8_t,std::uint8_t,std::uint8_t> components_lhs,
    std::optional<std::tuple<std::uint8_t,std::uint8_t,std::uint8_t>> components_rhs)
    {

    //ansi code formed from lhs rgb vals
    std::string replace_lhs {form_24bit_ansi(ansi_esc,is_foreground,components_lhs)};

    //second half of the ansi code. only present if components_rhs != nullopt
    std::optional<std::string> replace_rhs = components_rhs ? std::optional<std::string>(form_24bit_ansi(ansi_esc,!is_foreground,components_rhs.value())) : std::nullopt;

    //we use both lhs and rhs if rhs has a value, otherwise just lhs
    if(replace_rhs){
        input.replace(index,full_token.size(),replace_lhs + replace_rhs.value());
    }else {
        input.replace(index,full_token.size(),replace_lhs);
    }

    //add the reset string onto the end, so any further text is not affected
    input += get_reset();

}


//same as above, but only one number per f/b
void sansic::internal::do_8bit(
    const std::string& full_token,
    std::string& input, 
    int& index,
    bool is_foreground,
    std::uint8_t color_value_lhs, 
    std::optional<std::uint8_t> color_value_rhs)
    {

    std::string replace_lhs {form_8bit_ansi(ansi_esc,is_foreground,color_value_lhs)};

    std::optional<std::string> replace_rhs = color_value_rhs ? std::optional<std::string>(form_8bit_ansi(ansi_esc,!is_foreground,color_value_rhs.value())) : std::nullopt;

    if(replace_rhs){
        input.replace(index,full_token.size(),replace_lhs + replace_rhs.value());
    }else {
        input.replace(index,full_token.size(),replace_lhs);
    }

    input += get_reset();

   
}


void sansic::internal::parse_token(const std::string& full_token,std::string& input, int&& index){ 

    //regex group output
    std::smatch components{};

    //parse the token passed, if it matches a regex, do the function associated with that regex with the token
    //both "do_rgb" and "do_8bit" contain an optional parameter. If that parameter is present, both foreground and background are set in one go.
    if(std::regex_match(full_token,components,rgb_normal_regex)){
        do_rgb(
            full_token,
            input,
            index,
            sansic::internal::smatch_is_foreground_insensitive(components[1]),
            std::make_tuple(std::stoi(components[2]),std::stoi(components[3]),std::stoi(components[4])) 
        );
    };

    if(std::regex_match(full_token,components,rgb_combined_regex)){
        do_rgb(
            full_token,
            input,
            index,
            sansic::internal::smatch_is_foreground_insensitive(components[1]),
            std::make_tuple(std::stoi(components[2]),std::stoi(components[3]),std::stoi(components[4])),
            otu8t(std::make_tuple(std::stoi(components[6]),std::stoi(components[7]),std::stoi(components[8])))
        );
    };


    if(std::regex_match(full_token,components,r8bit_normal_regex)) { 
        do_8bit(
            full_token,
            input,
            index,
            sansic::internal::smatch_is_foreground_insensitive(components[1]),
            std::stoi(components[2])
        ); 
    }

    if(std::regex_match(full_token,components,r8bit_combined_regex)) { 
        do_8bit(
            full_token,
            input,
            index,
            sansic::internal::smatch_is_foreground_insensitive(components[1]),
            std::stoi(components[2]),
            std::stoi(components[4])
        ); 
    }



}


//creates a string that resets all colors
const std::string sansic::internal::get_reset() {
    return ansi_esc + "0m";
}


//end of internal


//public 

std::string sansic::form (std::string input){

    for(size_t i {0}; i < input.size(); ++i){

        //if the input is the beginning of a token, grab it and parse it
        if(input.at(i) == token_start){

            //the position of the next end-of-token character ')'
            auto pos = input.find(token_end,i)+1;

            //the token extracted from the input
            //a token starts with '(' and ends in ')'
            auto full_token {input.substr(i,pos-i)};

            //parse the token
            sansic::internal::parse_token(full_token, input,i);


        }

    }

    return input;

}

