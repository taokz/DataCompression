cmake_minimum_required(VERSION 3.13)
project(DataCompression)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(DataCompression
        config.h
        main.cpp
        Makefile
        max_heap.cpp
        max_heap.h
        #readfile.cpp
        #readfile.h
        trie_sentence.cpp
        trie_sentence.h
        trie_single.cpp
        trie_single.h
        word_freq.cpp
        word_freq.h
        sentence_freq.h sentence_freq.cpp)
