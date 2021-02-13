#pragma once

#include <string>
#include <vector>
#include "trie_single.h"
#include "trie_sentence.h"
#include "max_heap.h"

class sentence_freq 
{
    private:
        std::vector<std::string> vfname; //files name
        unsigned int sentences_num; //the total number of sentences has read
        bool is_letter(char con);
        bool is_end_of_sentence(std::string &word);
        void lower_case(std::string &word);
        bool is_regular_word(std::string &word);
        int wordsInPattern(std::string pattern);
    public:
        sentence_freq();
        ~sentence_freq();
 
        void cal_sentences_freq( trie_single *const ctrie_single, std::string out_fname = "sentence_freq.txt");//out_fname: the name of the result file 

        //ctrie_sentence: the trie where pattern stored; ctrie_single: the trie where common words stored
        void insert_sentences(std::string fname, trie_sentence *const ctrie_sentence, trie_single *const ctrie_single);
        void write_to_file(std::string out_fname, max_heap *const cmax_heap);
        int getFileNum()
        {
            return vfname.size();
        }
        unsigned int get_sentences_num()
        {
            return sentences_num;
        }
};

