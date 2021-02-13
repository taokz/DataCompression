#pragma once

#include <string>
#include <vector>
#include "trie_single.h"
#include "max_heap.h"

class word_freq 
{
    private:
        std::vector<std::string> vfname; //files name
        unsigned int words_num; //the total number of words has read
        bool is_letter(char con);
        bool is_regular_word(std::string &word);
        void remove_punctuation(std::string &word);
    public:
        word_freq();
        ~word_freq();
 
        void cal_words_freq(std::string out_fname = "word_freq.txt");//out_fname: the name of the result file 
        void insert_words(std::string fname, trie_single *const ctrie_single);
        void write_to_file(std::string out_fname, max_heap *const cmax_heap);

        //get the common words from file "fname", save the words into "ctrie_single", "limit_count" is the smallest count of words
	    void get_common_words(std::string fname, trie_single *const ctrie_single, unsigned int limit_count);
        unsigned int getFileNum()
        {
            return vfname.size();
        }
        unsigned int get_words_num()
        {
            return words_num;
        }
};


