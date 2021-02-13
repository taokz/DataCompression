#pragma once

#include <string>
#include <vector>

class dictionary_file 
{
    private:
        std::string fwords; //Words file name
        std::string fpatterns; //Pattern file name
        unsigned int words_num; //the total number of words has read
        unsigned int patterns_num; //The number of patterns
    public:
        dictionary_file();
        dictionary_file(std::string fname_words, std::string fname_patterns);
        ~dictionary_file();
 
        void create();
        void rewrite(std::vector<std::string> v_words, std::string fname);//v_words: New words; fname: Resently dictionary

        int get_words_num()
        {
            return words_num;
        }
        unsigned int get_patterns_num()
        {
            return patterns_num;
        }
};

