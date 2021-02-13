#include "dictionary_file.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include "trie_sentence.h"
#include "max_heap.h"

dictionary_file::dictionary_file()
{
    words_num = 0;
    patterns_num = 0;
    fwords = "word_freq.txt";
    fpatterns = "sentence_freq.txt";
}


dictionary_file::dictionary_file(std::string fname_words, std::string fname_patterns)
{
    words_num = 0;
    patterns_num = 0;
    fwords = fname_words;
    fpatterns = fname_patterns;
}
dictionary_file::~dictionary_file()
{}

std::string get_time_str()
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[50];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,sizeof(buffer),"%Y%m%d%I%M%S",timeinfo);
    std::string time_str(buffer);
    return time_str;
}

/*
 *User patterns file and words file create a dictionary
 *Write the pattern at first since the number of pattern is stable, while the number of words is changing
 */
void dictionary_file::create()
{
    std::vector<std::string> v_keys;
    //Open pattern file
    std::fstream fs_pattern;
    fs_pattern.open(fpatterns, std::ios::in);
    if(!fs_pattern.is_open())
    {
        std::cout << fpatterns << " open failed, can NOT create dictionary!" << std::endl;
        return;
    }

    std::string key;
    std::string trash_info;
    getline(fs_pattern, trash_info);  //Skip the first line
    while(!fs_pattern.eof())
    {
        getline(fs_pattern, key);
        key = key.substr(0, key.find('\t'));
        if(key.length())
        {
            v_keys.push_back(key);
            key = "";
        }
    }
    fs_pattern.close();
    patterns_num = v_keys.size();

    //Open word file
    std::fstream fs_word;
    fs_word.open(fwords, std::ios::in);
    if(!fs_word.is_open())
    {
        std::cout << fwords << " open failed, can NOT create dictionary!" << std::endl;
        return;
    }
    getline(fs_word, trash_info);  //Skip the first line
    while(!fs_word.eof())
    {
        getline(fs_word, key);
        key = key.substr(0, key.find('\t'));
        if(key.length())
        {
            v_keys.push_back(key);
            key = "";
        }
    }
    fs_word.close();
    words_num = v_keys.size() - patterns_num;

    //Open dictionary file
    std::fstream fs_dictionary;
    std::string path = "./dictionary/dictionary_" + get_time_str();
    fs_dictionary.open(path, std::ios::out);
    if(!fs_dictionary.is_open())
    {
        std::cout << "Can NOT create " << path << "!" << std::endl;
        return;
    }
    fs_dictionary << "patterns: " << patterns_num << "  " << "words:    " << words_num << '\n';
    for(unsigned int i=0; i<v_keys.size(); i++)
    {
        fs_dictionary << v_keys[i] << "\t" << i << '\n';
    }

    fs_dictionary.close();
}


void dictionary_file::rewrite(std::vector<std::string> v_words, std::string fname)
{
    //Load the old dictionary
    std::fstream fs_dictionary_old;
    fs_dictionary_old.open(fname, std::ios::in);
    if(!fs_dictionary_old.is_open())
    {
        std::cout << fname << " open failed! Can NOT rewrite dictionary." << std::endl;
        return; 
    }
    std::string key;
    std::string trash_info;
    std::string pattern_num_str;
    std::string words_num_str;
    fs_dictionary_old >> trash_info >> pattern_num_str >> trash_info >> words_num_str;
    patterns_num = stoi(pattern_num_str, nullptr);
    words_num = stoi(words_num_str, nullptr);

    std::vector<std::string> v_keys;
    while(!fs_dictionary_old.eof())
    {
        getline(fs_dictionary_old, key);
        if(key != "")
        {
            v_keys.push_back(key);
            key = "";
        }
    }
    fs_dictionary_old.close();

    //Create a new dictionary
    std::fstream fs_dictionary_new;
    std::string path = "./dictionary/dictionary_" + get_time_str();
    fs_dictionary_new.open(path, std::ios::out);
    if(!fs_dictionary_new.is_open())
    {
        std::cout << "Can NOT create " << path << "!" << std::endl;
        return;
    }
    fs_dictionary_new << "patterns: " << patterns_num << "  " << "words:    " << words_num + v_words.size() << '\n';
    //Old keys
    for(unsigned int i=0; i<v_keys.size(); i++)
    {
        fs_dictionary_new << v_keys[i] << '\n';
    }
    //New keys
    for(unsigned int i=0; i<v_words.size(); i++)
    {
        fs_dictionary_new << v_words[i] << '\n';
    }

    fs_dictionary_new.close();
    words_num = words_num + v_words.size();
}
