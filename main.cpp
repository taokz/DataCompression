#include <iostream>
#include "word_freq.h"
#include "trie_single.h"
#include "sentence_freq.h"
#include "trie_sentence.h"
#include "trie_dictionary.h"
#include "encode_text.h"
#include "dictionary_file.h"


int main(int argc, char** argv) {

	word_freq* cword_freq = new word_freq;
	cword_freq->cal_words_freq();
    trie_single* ctrie_single = new trie_single;
    cword_freq->get_common_words("word_freq.txt", ctrie_single, 800);
    //ctrie_single->display();        

    sentence_freq* csentence_freq = new sentence_freq;
    csentence_freq->cal_sentences_freq(ctrie_single);

	delete csentence_freq;
        
    dictionary_file* cdictionary_file = new dictionary_file;
    cdictionary_file->create();
    delete cdictionary_file;
    trie_dictionary* ctrie_dictionary = new trie_dictionary();
    //trie_dictionary* ctrie_dictionary = new trie_dictionary("./dictionary/dictionary_20190508105256");
    //ctrie_dictionary->display();
    encode_text* cencode_text = new encode_text;

    std::string keep_encode = "N"; //If it is Y/y, exit program; else continue
    std::string fname = "";
    while(keep_encode != "Y" && keep_encode != "y")
    {
        keep_encode = "";
        std::cout << "Please input command or Y/y to exit this program." << std::endl;
        std::cout << "filename e: For encoding" << std::endl;
        std::cout << "filename d: For decoding" << std::endl;
        getline (std::cin, keep_encode);
        if(keep_encode == "Y" || keep_encode == "y") //Exit the program
        {
            delete cword_freq;
            delete ctrie_single;
            delete ctrie_dictionary;
            delete cencode_text;
        
            return 0;
        }
        else if(keep_encode.substr(keep_encode.find(' ')+1) == "e")
        {
            fname = keep_encode.substr(0, keep_encode.find(' '));
            std::string encode_fname = fname + ".encoded";
            cencode_text->encode(ctrie_dictionary, ctrie_single, fname, encode_fname); 
        }
        else if(keep_encode.substr(keep_encode.find(' ')+1) == "d")
        {
            fname = keep_encode.substr(0, keep_encode.find(' '));
            std::string decode_fname = fname + ".decoded";
            cencode_text->decode(fname, decode_fname);
        }
        else
        {
            continue;
        }
    }
}



