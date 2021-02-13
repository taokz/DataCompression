/*
 * Public data structure definition
 */

#pragma once

#include <string>
#define Max 28
#define MAX_SEN 30
#define NEIGH_DISTANCE 5  //the maximum number of uncommon words between two common words that makes these two common words in the same pattern

struct trie_node
{
    char*   key;
    trie_node*  child[Max];   //link list of children
    unsigned int freq;        //word count/frequence
    bool feature;             //true: multi-character; false: single-character
};

struct sentence_trie_node
{
    char*   key;                
    sentence_trie_node*  child[MAX_SEN];   //link list of children
    unsigned int freq;                     //pattern occurrence times
    bool feature;             //true: multi-character; false: single-character
};

struct dictionary_node
{
    char*   key;                
    dictionary_node*  child[MAX_SEN];   //link list of children
    int encoding;                 //encoding of the key
    bool feature;             //true: multi-character; false: single-character
};

struct heap_node
{
    std::string key;
    unsigned int freq;
    heap_node(std::string k, unsigned int f) {key = k; freq = f;}
};

