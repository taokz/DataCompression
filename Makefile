DEBUG	:=	-g
OPT	:=	#-O2
CXX	:=	g++ $(DEBUG) $(OPT) -std=c++11 -Wall
OBJ	:=	main.o trie_single.o max_heap.o word_freq.o trie_sentence.o trie_dictionary.o sentence_freq.o encode_text.o dictionary_file.o vector_dictionary.o
eng_text: $(OBJ)

	$(CXX)	$(OBJ) -o eng_text


main.o: main.cpp trie_single.h max_heap.h

	$(CXX) -c main.cpp


trie_single.o: trie_single.cpp trie_single.h config.h

	$(CXX) -c trie_single.cpp


max_heap.o: max_heap.cpp max_heap.h config.h

	$(CXX) -c max_heap.cpp


word_freq.o: word_freq.cpp word_freq.h

	$(CXX) -c word_freq.cpp


trie_sentence.o: trie_sentence.cpp trie_sentence.h config.h

	$(CXX) -c trie_sentence.cpp


trie_dictionary.o: trie_dictionary.cpp trie_dictionary.h config.h

	$(CXX) -c trie_dictionary.cpp


sentence_freq.o: sentence_freq.cpp sentence_freq.h

	$(CXX) -c sentence_freq.cpp


encode_text.o: encode_text.cpp encode_text.h
	$(CXX) -c encode_text.cpp


dictionary_file.o: dictionary_file.cpp dictionary_file.h
	$(CXX) -c dictionary_file.cpp


vector_dictionary.o: vector_dictionary.cpp vector_dictionary.h
	$(CXX) -c vector_dictionary.cpp


clean:
	rm *.o eng_text
