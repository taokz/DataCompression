#pragma once

#include <string>
#include <vector>

class vector_dictionary
{
    private:
        unsigned int patterns_num;
        unsigned int words_num;
        std::vector<std::string> vdictionary;
    public:
        vector_dictionary(std::string fname);
        ~vector_dictionary();
        std::string get_key(int code) {
            return vdictionary[code];
        }
        int get_dictionary_size()
        {
            return vdictionary.size();
        }
        unsigned int get_patterns_num()
        {
            return patterns_num;
        }
        unsigned int get_keys_num()
        {
            return patterns_num + words_num;
        }
};