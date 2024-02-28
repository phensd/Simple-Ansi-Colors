#include "sansic.hpp"
#include <iostream>
#include <optional>
#include <regex>
#include <unordered_map>
#include <functional>

    //anon namespace for some things that dont need to be seen elsewhere
    namespace {

        //beginning and end of tokens we parse
        const char token_start {'('};
        const char token_end {')'};


        //the beginning of an ANSI escape code
        //https://en.wikipedia.org/wiki/ANSI_escape_code
        const std::string ansi_esc {"\033["};

        //the regexes we will use for matches
        const std::array<const std::regex,4> regexes {
            std::regex{"\\(\\s*(F|B|f|b)\\s*(\\d{1,3})\\s*\\D\\s*(\\d{1,3})\\s*\\D\\s*(\\d{1,3})\\s*\\)$"}, //RGB_NORMAL
            std::regex{"\\(\\s*(F|B|f|b)\\s*(\\d{1,3})\\s*\\D\\s*(\\d{1,3})\\s*\\D\\s*(\\d{1,3})\\s*\\D\\s*(F|B|f|b)\\s*(\\d{1,3})\\s*\\D\\s*(\\d{1,3})\\s*\\D\\s*(\\d{1,3})\\s*\\)$"}, //RGB_COMBINED
            std::regex{"\\(\\s*(F|B|f|b)\\s*(\\d{0,3})\\s*\\)$"}, //C8BIT_NORMAL
            std::regex{"\\(\\s*(F|B|f|b)\\s*(\\d{0,3})\\s*\\D\\s*(F|B|f|b)\\s*(\\d{0,3})\\s*\\)$"} //C8BIT_COMBINED
        };


        //this is used to map the indexes of the array to their companion function in parse_token()
        enum class REGEX_VALUES{
            RGB_NORMAL,
            RGB_COMBINED,
            C8BIT_NORMAL,
            C8BIT_COMBINED  

        };



    }//end of anon namespace

//internal





//forms a 24 bit ansi escape code
//https://en.wikipedia.org/wiki/ANSI_escape_code#24-bit
std::string sansic::internal::form_24bit_ansi(const std::string& delim, bool is_foreground,std::tuple<std::uint8_t,std::uint8_t,std::uint8_t>& rgb_vals){

    std::stringstream output;

    output << delim << (is_foreground ? 38 : 48) << ";" << "2" <<  ';' << static_cast<int>(std::get<0>(rgb_vals)) << ';' << static_cast<int>(std::get<1>(rgb_vals))  << ';' << static_cast<int>(std::get<2>(rgb_vals)) << "m";
    
    return output.str();

}

//same as above, but 8 bit instead
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
    
    if(replace_rhs){
            input.replace(index,full_token.size(),replace_lhs + replace_rhs.value());
    }else{
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

    //maps regex enum values to their companion function.
    //the regex enum is converted to an int
    //then passed as an index to the array 'regexes' in the anon namespace in this source file.

    //"full token" is the token parsed from the input (i.e '(F200,200,200)' is a full token)
    //"input" is the full string input
    //"index" is the index of the full part of the token (i.e '(' )
    //the next parameter checks if the first component of the regex match (either char F or B) is F, to set the next change to be on the foreground.
    //then, a value is given (for rgb a tuple, for 8bit an std::uint8_t)
    //this value represents the color to be used in the forming of the ansi escape code
    //next another value of the same type is given optionally, and, if it exists, it does the opposite of the previous
    //i.e, if the first value set the foreground, the second will set the background, and vice versa.


    //marked static because this should only be defined once 
    const static std::unordered_map<REGEX_VALUES,std::function<void(std::smatch& components)>> regex_function_map{

    {REGEX_VALUES::RGB_NORMAL,        
        [&full_token,&input,&index](std::smatch& components){
            sansic::internal::do_rgb(
            full_token,
            input,
            index,
            sansic::internal::smatch_is_foreground_insensitive(components[1]),
            std::make_tuple(std::stoi(components[2]),std::stoi(components[3]),std::stoi(components[4])),
            std::nullopt);
            }},

    {REGEX_VALUES::RGB_COMBINED,
        [&full_token,&input,&index](std::smatch& components){
            sansic::internal::do_rgb(
            full_token,
            input,
            index,
            sansic::internal::smatch_is_foreground_insensitive(components[1]),
            std::make_tuple(std::stoi(components[2]),std::stoi(components[3]),std::stoi(components[4])),
            std::optional<std::tuple<std::uint8_t,uint8_t,uint8_t>>(std::make_tuple(std::stoi(components[6]),std::stoi(components[7]),std::stoi(components[8]))));
            }},


    {REGEX_VALUES::C8BIT_NORMAL,
        [&full_token,&input,&index](std::smatch& components){
            sansic::internal::do_8bit(
            full_token,
            input,
            index,
            sansic::internal::smatch_is_foreground_insensitive(components[1]),
            std::stoi(components[2]),
            std::nullopt);
        }},

    {REGEX_VALUES::C8BIT_COMBINED,
        [&full_token,&input,&index](std::smatch& components){
            sansic::internal::do_8bit(
            full_token,
            input,
            index,
            sansic::internal::smatch_is_foreground_insensitive(components[1]),
            std::stoi(components[2]),
            std::stoi(components[4]));
        }},

    };

    //regex group output
    std::smatch components{};

    //iterate through the map, 
    //check if the regex expression found by subscripting the regexes array matches the token, 
    //then perform the paired function if so.
    for(auto& [regex_expression_value,mapped_function] : regex_function_map){
        if(std::regex_match(full_token,components,regexes[static_cast<int>(regex_expression_value)])){
            mapped_function(components);
        }
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

