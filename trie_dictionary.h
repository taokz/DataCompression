/*
 * Dictionary trie
 */
#pragma once

#include "config.h"
#include "trie_single.h"
#include <unordered_map>
#include <string>

class trie_dictionary
{
  private:
    dictionary_node*  T_root; //Use sentence trie structure
    int encoding; //use the sequence number of the key encode the key
    std::string fname; //Dictionary file name
    int words_num; //Number of words
    std::unordered_map<std::string, int> m_new_words; //New words added into dictionary when read new books
    int pattern_num;

    void delNode(dictionary_node* node); //Delete trie node
    void copy_key(std::string key, dictionary_node* node);
    dictionary_node* createNode(std::string character); //Create a new node
    int calIndex(char letter); //Calculate the index of the key
    void output_d(dictionary_node* node, std::string output_word); //Display trie
    dictionary_node* search_location(std::string key, dictionary_node* start); //Search the end of the key from the start node
    void get_keys();//Get keys from file and create dictionary trie
    std::string get_resently_dictionary(); //Get resently dictionary file name
  public:
    trie_dictionary();
    trie_dictionary(std::string dictionary_fname);
    ~trie_dictionary();
  public:
    void insert(std::string key); //Use to create the dictionary
    void insert(std::string key, int code); //Load the keys into dictionary
    int search(std::string key); //Search a key and return the encoding of the key
    void encoding_sentence(bool &is_same_file, std::vector<std::string> &sentence, int* const res_encoding, unsigned int &offset, trie_single* const ctrie_single, std::string &additional_words); //Encoding the sentence
    void display(); //Display trie
    std::string get_dictionary_name()
    {
      return fname;
    }
};

