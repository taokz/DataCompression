#include "vector_dictionary.h"
#include <fstream>
#include <iostream>

vector_dictionary::vector_dictionary(std::string fname)
{
    std::ifstream filelist;
    filelist.open(fname); 
    if(!filelist.is_open())
    {
        std::cout << "File: " << fname << " opened failed!" << std::endl;
        return;
    }

    std::string trash_info;
    std::string words_num_str;
    std::string patterns_num_str;
    filelist >> trash_info >> patterns_num_str >> trash_info >> words_num_str; 

    words_num = std::stoi(words_num_str);
    patterns_num = std::stoi(patterns_num_str);

    std::string key;
    std::string code_str;
    filelist.get();
    while(getline(filelist, key))
    {       
        code_str = key.substr(key.find('\t')+1);
        key = key.substr(0, key.length()-code_str.length()-1);
        vdictionary.push_back(key);
    }
    filelist.close();
}

vector_dictionary::~vector_dictionary()
{}
