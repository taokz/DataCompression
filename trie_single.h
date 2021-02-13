/*
 * Single words trie
 * This is a compressed trie structure
 */

#pragma once

#include "config.h"
#include "max_heap.h"

class trie_single
{
    private:
      trie_node*  T_root;
      int words_num;
      void delNode(trie_node* node);
      void copy_key(std::string key, trie_node* node);
      trie_node* createNode(std::string key);
      int calIndex(char letter);
      void output_d(trie_node* node, std::string output_word);
      void output_tr(trie_node* node, max_heap* cMax_Heap, std::string output_word);
    public:
      trie_single();
      ~trie_single();
    public:
      void insert(std::string key);
      unsigned int search(std::string key);  //return frequence of word
      void traverse(max_heap* cmax_heap);
      void display();
      int get_words_num()
      {
          return words_num;
      }
};



