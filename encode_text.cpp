#include "encode_text.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "vector_dictionary.h"
#include <limits>

encode_text::encode_text()
{
}

encode_text::~encode_text()
{}

bool encode_text::is_letter(char con)
{
    if((con >= 'a' && con <= 'z') || (con >= 'A' && con <= 'Z'))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
 *Judge if the word is the end of the sentence and remove the punctuations from word
 */
bool encode_text::is_end_of_sentence(std::string &word)
{
    bool is_end = false;
    while(word.length())//Remove non-letter from begin of the word
    {
        if(!is_letter(word[0]))
        {
            word = word.substr(1, word.length()-1);
        }
        else
        {
            break;
        }
    }

	if(word[word.length()-1] == '.' || word[word.length()-1] == ',' || word[word.length()-1] == ':' \
    || word[word.length()-1] == '!' || word[word.length()-1] == '?' || word[word.length()-1] == ';' \
    || word[word.length()-1] == '\"' || word[word.length()-1] == '\'' || word[word.length()-1] == ')' \
    || word[word.length()-1] == ']' || word[word.length()-1] == '}')
	{
        word = word.substr(0, word.length()-1); 
		is_end = true;
	}
    while(word.length()) //Remove non-letter from end of the word
    {
        if(!is_letter(word[word.length()-1]))
        {
            word = word.substr(0, word.length()-1);
        }
        else
        {
            break;
        }
    }

    return is_end;
}


bool encode_text::is_regular_word(std::string &word) //Word that all characters are letters
{
    for(unsigned int i=0; i<word.length(); i++)
    {
        if(word[i] >= 'a' && word[i] <= 'z')
        {
            continue;
        }
        else if(word[i] >= 'A' && word[i] <= 'Z')
        {
            word[i] = word[i] + 32; //change the upper-case letter to lower-case
            continue;
        }
        else if(word[i] == '\'')
        {
            if((i+2 == word.length() && (word[i+1] == 't' || word[i+1] == 's')) ||
               (i+3 == word.length() && (word[i+1] == 'r' || word[i+1] == 'v') && word[i+2] == 'e'))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else if(word[i] == '-')
        {
            if(word.find('-', i+1) == -1)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
        
    }
    return true;
}

bool encode_text::is_word(std::string &word) //Word contain letter
{
    bool isWord = false;
    for(unsigned int i=0; i<word.length(); i++)
    {
        if(word[i] >= 'a' && word[i] <= 'z')
        {
            isWord = true;
        }
        else if(word[i] >= 'A' && word[i] <= 'Z')
        {
            word[i] = word[i] + 32; //change the upper-case letter to lower-case
            isWord = true;
        }
    }
    return isWord;
}

/*
 *Read the file and add the patterns into tire 
 */
void encode_text::encode(trie_dictionary *const ctrie_dictionary, trie_single* const ctrie_single, std::string org_fname, std::string encoded_fname)
{
	std::ifstream fileorg;
	fileorg.open(org_fname.c_str());
	if(!fileorg.is_open())
	{
		std::cout << "File: " << org_fname << " opened failed!" << std::endl;
		return;
	}
    
    std::ofstream filestp;
    std::string stp_fname = org_fname + ".stp";
    filestp.open(stp_fname.c_str(), std::ios::out);
    if(!filestp.is_open())
    {
        std::cout << "File: " << stp_fname << " opened failed!" << std::endl;
        return;
    }

    //Read the whole content of file
    std::stringstream buffer;
    buffer << fileorg.rdbuf();
    fileorg.close();

    buffer.seekg(0, std::ios::end);
    int buf_size = buffer.tellg();
    buffer.seekg(0, std::ios::beg);
	
    //Encode
    std::string word = "";
	std::vector<std::string> sentence;
    int* res_buffer = new int[(int)(buf_size*0.7)];
    unsigned int offset = 0;
    bool is_same_file = false;
    bool isWord = false;
    int pos = -1;
    std::string sub_word = "";
    std::string additional_words; //Words not in the dictionary
	while(buffer >> word)
	{
        do
        {
            sub_word = "";
            pos = word.find("--");
            if(pos > -1 && (pos != 0 || pos != word.length()-3)) 
            {
                sub_word = word.substr(0, pos);
                word = word.substr(pos+1);
            }
            else
            {
                sub_word = word;
                word = "";
            }
            if(sub_word != "")
            {
                isWord = is_word(sub_word);
                if(isWord)
                {
                    if(is_end_of_sentence(sub_word))  //this is not the end of a word
                    {
                        sentence.push_back(sub_word); 
                        filestp << sub_word << " ";
                        ctrie_dictionary->encoding_sentence(is_same_file, sentence, res_buffer, offset, ctrie_single, additional_words);
                        sentence.clear();
                    }
                    else
                    {
                        filestp << sub_word << " ";
                        sentence.push_back(sub_word);
                    }
                }
            } 
        }while(word != "");
	}
    if(sentence.size())
    {
        ctrie_dictionary->encoding_sentence(is_same_file, sentence, res_buffer, offset, ctrie_single, additional_words);
    }
    filestp.close();

    std::ofstream fileres;
	fileres.open(encoded_fname.c_str(), std::ios::binary);
    if(!fileres.is_open())
	{
		std::cout << "File: " << encoded_fname << " opened failed!" << std::endl;
		return;
	}
    fileres << ctrie_dictionary->get_dictionary_name() << '\n';
    fileres << additional_words << '\n';
/*    for(unsigned int i=0; i<offset; i++)
    {
        std::cout << res_buffer[i] << " ";
    }

    std::cout << std::endl;*/
    fileres.write((char*)(res_buffer), sizeof(int)*offset);
    fileres.close();

    std::ofstream filetest;
    filetest.open("code.txt");
    if(!filetest.is_open())
        {
                std::cout << "File: " << encoded_fname << " opened failed!" << std::endl;
                return;
        }
    for(unsigned int i=0; i<offset; i++)
    {
        filetest << res_buffer[i] << " ";
    }
    filetest.close();

    delete[] res_buffer;
}

void encode_text::decode(std::string encoded_fname, std::string decoded_fname)
{
    std::ifstream file_encoded;
	file_encoded.open(encoded_fname.c_str(), std::ios::binary);
	if(!file_encoded.is_open())
	{
		std::cout << "File: " << encoded_fname << " opened failed!" << std::endl;
		return;
	}

    std::string dictionary_name = "";
    getline(file_encoded, dictionary_name, '\n');

    //Get additional words
    std::string additional_words_str = "";
    getline(file_encoded, additional_words_str, '\n');
    std::vector<std::string> vadditional_words;
    int pos = -1;
    while(additional_words_str.length())
    {
        pos = additional_words_str.find(' ');
        if(pos == 0) //There is a space at the begin of the string
        {
            additional_words_str = additional_words_str.substr(pos+1);
            continue;
        }
        if(pos == -1)
        {
            vadditional_words.push_back(additional_words_str);
            break;
        }
        vadditional_words.push_back(additional_words_str.substr(0, pos));
        additional_words_str = additional_words_str.substr(pos+1);
    }
/*    for(unsigned int i=0; i<vadditional_words.size(); i++)
    {
        std::cout << "vadditional_words[" << i << "]: " << vadditional_words[i] << std::endl;
    }*/

    //Create the dictionary for decoding
    vector_dictionary* cvector_dictionary = new vector_dictionary(dictionary_name);
    std::string content = "";
    std::string sentence = "";
    int max_code = cvector_dictionary->get_keys_num() + vadditional_words.size()-1;
    std::cout << "max code: " << max_code << std::endl;
    int code = -1;

    while(file_encoded.read((char*)&code, sizeof(code)))
    {
        if(code == -1)
        {
            std::cout << "Encoded file includes -1 !!!" << std::endl;
            return;
        }
        else if(code < (int)cvector_dictionary->get_patterns_num()) //This is a pattern
        {
            sentence = cvector_dictionary->get_key(code);
            pos = sentence.find_first_of('#');
            if(pos == -1) //There is no uncommon words in the pattern
            {
                content += sentence + " ";
            }
            else
            {
                while(pos != -1)
                {
                    content += sentence.substr(0, pos) + " ";
                    sentence = sentence.substr(pos+1);
                    if(file_encoded.read((char*)&code, sizeof(code))) //The code after '#' is the number of uncommon words '#' represents
                    {
                        for(int i=code; i>0; i--)
                        {
                            if(file_encoded.read((char*)&code, sizeof(code)))
                            {
                                if(code == -1)
                                {
                                    std::cout << "Encoded file includes -1 !!!" << std::endl;
                                    return;
                                }
                                else if(code < cvector_dictionary->get_dictionary_size())
                                {
                                    content += cvector_dictionary->get_key(code) + " ";
                                }
                                else if(code <= max_code)
                                {
                                    content += vadditional_words[code-cvector_dictionary->get_dictionary_size()] + " ";
                                }
                                else
                                {
                                    std::cout << "Code " << code << " bigger than max code !!!" << std::endl;
                                    return;
                                }
                            }
                        }
                    }
                    else
                    {
                        std::cout << "Reach file end, but pattern does not finish parsing !!!" << std::endl;
                        return;
                    }
                    pos = sentence.find_first_of('#');
                }
                if(sentence.length())
                {
                    content += sentence + " ";
                }
            }
        }
        else if(code < cvector_dictionary->get_dictionary_size())
        {
            content += cvector_dictionary->get_key(code) + " ";
        }
        else if(code <= max_code)
        {
            content += vadditional_words[code-cvector_dictionary->get_dictionary_size()] + " ";
        }
     /*   else if(code == INT32_MAX)
        {
            content[content.length()-1] = '\n';
        }*/
        else
        {
            std::cout << "Code " << code << " bigger than max code !!!" << std::endl;
            return;
        }
    }
    file_encoded.close();
    delete cvector_dictionary;

    std::ofstream file_decoded;
	file_decoded.open(decoded_fname.c_str());
	if(!file_decoded.is_open())
	{
		std::cout << "File: " << decoded_fname << " opened failed!" << std::endl;
		return;
	}

    file_decoded << content;
    file_decoded.close();
}
