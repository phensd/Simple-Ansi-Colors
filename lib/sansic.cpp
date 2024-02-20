#include "sansic.hpp"

    //anon namespace for some things that dont need to be seen elsewhere
    namespace {

        const char token_start {'('};
        const char token_end {')'};

        const std::string ansi_esc {"\033["};

        //regex for csv values
        //example matches: [200,300,200] [300, 200  ,100] [20,10,000]
        //groups divided into 1-3 integers
        const std::regex rgb_normal_regex{"\\(\\s*(F|B)(\\d{1,3})\\s*[,|\\-\\_]\\s*(\\d{1,3})\\s*[,|\\-\\_]\\s*(\\d{1,3})\\s*\\)$"};


        //same as above, but allows setting both background and foreground at once
        //ex. (F200,100,200,B200,100,200)
        const std::regex rgb_combined_regex{"\\(\\s*(F|B)(\\d{1,3})\\s*[,|\\-\\_]\\s*(\\d{1,3})\\s*[,|\\-\\_]\\s*(\\d{1,3})\\s*[,|\\-\\_]\\s*(F|B)(\\d{1,3})\\s*[,|\\-\\_]\\s*(\\d{1,3})\\s*[,|\\-\\_]\\s*(\\d{1,3})\\s*\\)$"};


        
    }//end of anon namespace


std::string sansic::internal::form_24bit_ansi(std::string delim, bool is_foreground,std::tuple<std::string,std::string,std::string> rgb_vals){
    std::stringstream output;

    output << delim << (is_foreground ? 38 : 48) << ";" << "2" <<  ';' << std::get<0>(rgb_vals) << ';' << std::get<1>(rgb_vals)  << ';' << std::get<2>(rgb_vals) << "m";
    
    return output.str();

}

void sansic::internal::do_rgb_normal(std::smatch components, const std::string& full_token,std::string& input, int& index){


    std::string replace {form_24bit_ansi(ansi_esc,components[1] == "F",std::make_tuple(components[2],components[3],components[4]))};
    input.replace(index,full_token.size(),replace);
    input += get_reset();


}

void sansic::internal::do_rgb_combined(std::smatch components,const std::string& full_token,std::string& input, int& index){

    std::string replace_lhs {form_24bit_ansi(ansi_esc,components[1] == "F",std::make_tuple(components[2],components[3],components[4]))};
    std::string replace_rhs {form_24bit_ansi(ansi_esc,!(components[1] == "F"),std::make_tuple(components[6],components[7],components[8]))};

    input.replace(index,full_token.size(),replace_lhs + replace_rhs);

    input += get_reset();

}

void sansic::internal::parse_token(const std::string& full_token,std::string& input, int&& index){ 


    std::smatch components{};

    if(std::regex_match(full_token,components,rgb_normal_regex)) do_rgb_normal(components,full_token,input,index);
    if(std::regex_match(full_token,components,rgb_combined_regex)) do_rgb_combined(components,full_token,input,index);



}



const std::string sansic::internal::get_reset() {
        return ansi_esc + "0m";
}




//public 
std::string sansic::form (std::string input){

    for(size_t i {0}; i < input.size(); ++i){

        if(input.at(i) == token_start){

            auto pos = input.find(token_end,i)+1;
            auto full_token {input.substr(i,pos-i)};


            sansic::internal::parse_token(full_token, input,i);




        }

    }

    return input;

}

