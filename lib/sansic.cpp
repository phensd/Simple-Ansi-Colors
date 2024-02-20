#include "sansic.hpp"
#include <iostream>

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
        const std::regex rgb_normal_regex{"\\(\\s*(F|B)(\\d{1,3})\\s*[,|\\-\\_]\\s*(\\d{1,3})\\s*[,|\\-\\_]\\s*(\\d{1,3})\\s*\\)$"};


        //same as above, but allows setting both background and foreground at once
        //ex. (F200,100,200,B200,100,200)
        const std::regex rgb_combined_regex{"\\(\\s*(F|B)(\\d{1,3})\\s*[,|\\-\\_]\\s*(\\d{1,3})\\s*[,|\\-\\_]\\s*(\\d{1,3})\\s*[,|\\-\\_]\\s*(F|B)(\\d{1,3})\\s*[,|\\-\\_]\\s*(\\d{1,3})\\s*[,|\\-\\_]\\s*(\\d{1,3})\\s*\\)$"};


        
    }//end of anon namespace


//makes sure rgb values are no greater than 255, no less than 0.
std::tuple<int,int,int>& sansic::internal::util::conform_rgb_vals(std::tuple<int,int,int>&& rgb_vals){
        auto r = std::get<0>(rgb_vals);
        auto g = std::get<1>(rgb_vals);
        auto b = std::get<2>(rgb_vals);

        std::cout << "Before: R" << r << " G" << g << " B" << b << "\n";

        if(r > 255) r = 255;
        if(g > 255) g = 255;
        if(b > 255) b = 255;

        if(r < 0) r = 0;
        if(g < 0) g = 0;
        if(b < 0) b = 0;

        std::cout << "After: R" << r << " G" << g << " B" << b << "\n";


        return rgb_vals;
    }


//forms a 24 bit ansi escape code
//https://en.wikipedia.org/wiki/ANSI_escape_code#24-bit
std::string sansic::internal::form_24bit_ansi(const std::string& delim, bool is_foreground,std::tuple<int,int,int>& rgb_vals){

    std::stringstream output;

    output << delim << (is_foreground ? 38 : 48) << ";" << "2" <<  ';' << std::get<0>(rgb_vals) << ';' << std::get<1>(rgb_vals)  << ';' << std::get<2>(rgb_vals) << "m";
    
    return output.str();

}


//takes syntax such as (F200,300,100) and creates a 24 bit ansi code out of it
void sansic::internal::do_rgb_normal(std::smatch& components, const std::string& full_token,std::string& input, int& index){

    //the RGB values extracted from the regex components
    //"conform" makes sure each value is no greater than 255 and no less than 0.
    std::tuple<int,int,int> rgb_vals {sansic::internal::util::conform_rgb_vals( {std::stoi(components[2]),std::stoi(components[3]),std::stoi(components[4])} )};



    std::string replace {form_24bit_ansi(ansi_esc,components[1] == "F",rgb_vals)};
    input.replace(index,full_token.size(),replace);
    input += get_reset();

}

//takes syntax such as (F200,300,100,B200,100,200) and creates a 24 bit ansi code out of it
void sansic::internal::do_rgb_combined(std::smatch& components,const std::string& full_token,std::string& input, int& index){


    std::tuple<int,int,int> rgb_vals_lhs {sansic::internal::util::conform_rgb_vals( {std::stoi(components[2]),std::stoi(components[3]),std::stoi(components[4])} )};
    std::tuple<int,int,int> rgb_vals_rhs {sansic::internal::util::conform_rgb_vals( {std::stoi(components[6]),std::stoi(components[7]),std::stoi(components[8])} )};


    std::string replace_lhs {form_24bit_ansi(ansi_esc,components[1] == "F",rgb_vals_lhs)};
    std::string replace_rhs {form_24bit_ansi(ansi_esc,!(components[1] == "F"),rgb_vals_rhs)};

    input.replace(index,full_token.size(),replace_lhs + replace_rhs);

    input += get_reset();

}

void sansic::internal::parse_token(const std::string& full_token,std::string& input, int&& index){ 

    //regex group output
    std::smatch components{};

    //parse the token passed, if it maxes a regex, do the function associated with that regex with the token
    if(std::regex_match(full_token,components,rgb_normal_regex)) do_rgb_normal(components,full_token,input,index);
    if(std::regex_match(full_token,components,rgb_combined_regex)) do_rgb_combined(components,full_token,input,index);



}


//creates a string that resets all colors
const std::string sansic::internal::get_reset() {
    return ansi_esc + "0m";
}




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

