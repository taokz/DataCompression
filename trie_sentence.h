/*
 * Sentence trie
 */
#pragma once

#include "config.h"
#include "max_heap.h"

class trie_sentence
{
  private:
    sentence_trie_node*  T_root;
    void delNode(sentence_trie_node* node);
    void copy_key(std::string key, sentence_trie_node* node);
    sentence_trie_node* createNode(std::string character);
    int calIndex(char letter);
    void output_tr(sentence_trie_node* node, max_heap* cmax_heap, std::string output_word, unsigned int limit_freq);
    void output_d(sentence_trie_node* node, std::string output_word);
  public:
    trie_sentence();
    ~trie_sentence();
  public:
    void insert(std::string key);
    unsigned int search(std::string key);
    void traverse(max_heap* cmax_heap, unsigned int limit_freq = 1);
    void display();
};

