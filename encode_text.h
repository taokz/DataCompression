#pragma once

#include <string>
#include "trie_dictionary.h"
#include "trie_single.h"
#include "vector_dictionary.h"

class encode_text 
{
    private:
        bool is_letter(char con);
        bool is_end_of_sentence(std::string &word);
        bool is_regular_word(std::string &word);
        bool is_word(std::string &word);
    public:
        encode_text();
        ~encode_text();
 
        /*
         *org_fname: The file name that will be encoded 
         *encoded_fname: The file name after encoded
         */
        void encode(trie_dictionary *const ctrie_dictionary, trie_single* const ctrie_single, std::string org_fname, std::string encoded_fname);
        
        /*
         *encoded_fname: File name which will be decode 
         *decoded_fname: Output file name 
         */
        void decode(std::string encoded_fname, std::string decoded_fname);
};

