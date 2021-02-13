#include "trie_dictionary.h"
#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <cstring>
#include <climits>
#include "dictionary_file.h"
#include <dirent.h>
#include <unistd.h>
#include <utility>
#include <limits>

trie_dictionary::trie_dictionary()
{
    T_root = new dictionary_node; //Root doesn't store key
    T_root->key = new char[2]; //use * represents the root key
    char r[] = "*";
    strcpy(T_root->key, r);
    T_root->encoding = -1;
    for(int i=0; i<MAX_SEN; i++)
    {
        T_root->child[i] = NULL;
    }
    encoding = -1; //Initialize the encoding number
    words_num = 0;
    pattern_num = 0;
    fname = get_resently_dictionary();
    get_keys();
}

std::string trie_dictionary::get_resently_dictionary()
{
    /*
     *get the files name in ./dictionary folder
     *reference from: https://blog.csdn.net/adong76/article/details/39432467
     */
    struct dirent *ptr;    
    DIR *dir;
    char *wd(getcwd(NULL,0));
    std::string cwd(wd);
    std::string PATH = cwd + "/dictionary";
    dir=opendir(PATH.c_str());

    std::string fname_str = "";
    std::vector<std::string> vfname;
    while((ptr=readdir(dir))!=NULL)
    {
        if(ptr->d_name[0] == '.')
            continue;
        fname_str = ptr->d_name;
        fname_str = fname_str.substr(fname_str.find_last_of('_')+1);
        vfname.push_back(fname_str);
    }
    closedir(dir);
    fname_str = vfname[0];
    for(unsigned int i=1; i<vfname.size(); i++)
    {
        if(fname_str < vfname[i])
        {
            fname_str = vfname[i];
        }
    }

    return "./dictionary/dictionary_" + fname_str;
}

trie_dictionary::trie_dictionary(std::string dictionary_fname)
{
    T_root = new dictionary_node; //Root doesn't store key
    T_root->key = new char[2]; //use * represents the root key
    char r[] = "*";
    strcpy(T_root->key, r);
    T_root->encoding = -1;
    for(int i=0; i<MAX_SEN; i++)
    {
        T_root->child[i] = NULL;
    }
    encoding = -1; //Initialize the encoding number
    fname = dictionary_fname;
    words_num = 0;
    pattern_num = 0;

    get_keys();
}

void trie_dictionary::get_keys()
{
    std::ifstream filelist;
    filelist.open(fname); //get the document files name
    if(!filelist.is_open())
    {
        std::cout << "File: " << fname << " opened failed!" << std::endl;
        return;
    }

    std::string trash_info;
    std::string words_num_str;
    std::string pattern_num_str;
    filelist >> trash_info >> pattern_num_str >> trash_info >> words_num_str; //get words number and newly added words number

    words_num = std::stoi(words_num_str);
    pattern_num = std::stoi(pattern_num_str);
    
    std::string key;
    std::string code_str;
    int code;
    while(getline(filelist, key))
    {       
        code_str = key.substr(key.find('\t')+1);
        key = key.substr(0, key.length()-code_str.length()-1);
        code = atoi(code_str.c_str());
        insert(key, code);
    }
    filelist.close();
    encoding = words_num + pattern_num - 1; //Code of the last key
}

/*
 *Delete the nodes of trie
 */
void trie_dictionary::delNode(dictionary_node* node)
{
    for(int i=0; i<MAX_SEN; i++)
    {
        if(node->child[i] != NULL)
        {
            delNode(node->child[i]);
        }
    }
    delete node->key;
    delete node;
}

trie_dictionary::~trie_dictionary()
{
    delNode(T_root);
}

/*
 *copy key
 */
void trie_dictionary::copy_key(std::string key, dictionary_node* node)
{
    if(node->key == NULL)
    {
        node->key = new char[key.length()+1];
        strcpy(node->key, key.c_str());
    }
    else
    {
        delete node->key;
        node->key = new char[key.length()+1];
        strcpy(node->key, key.c_str());
    }
}

/*
 *Create a new dictionary trie node
 */
dictionary_node* trie_dictionary::createNode(std::string character)
{
    dictionary_node* newNode = new dictionary_node;
    newNode->key = NULL;
    copy_key(character, newNode);
    newNode->encoding = -1; 

    for(int i=0; i<MAX_SEN; i++)
    {
        newNode->child[i] = NULL;
    }

    return newNode;
}

/*
 *Calculate the index of a letter.
 */
int trie_dictionary::calIndex(char letter)
{
    char a = 'a';
    char A = 'A';
    int index = -1;
    if(letter >= 'a' && letter <= 'z')
    {
        index = (int)letter%(int)a;
    }
    else if(letter >= 'A' && letter <= 'Z')
    {
        index = (int)letter%(int)A;
    }
    else if(letter == '\'')
    {
    	index = 26;
	}
    else if(letter == '#') //Represents any words or space between words in the pattern
    {
        index = 27;
    }
    else if(letter == ' ')
    {
        index = 28;
    }
    else if(letter == '-')
    { 
        index = 29;
    }
    return index;
}


/*
 *Input a key into dictionary trie
 */
void trie_dictionary::insert(std::string key)
{
    //Extract words from key 
    std::string pattern = key;
    std::vector<std::string> v_words;
    int pos1 = -1;
    int pos2 = -1;
    std::string word;
    while(pattern.length())
    {
        if(pattern[0] == '#' || pattern[0] == ' ')
        {
            word = pattern[0];
            v_words.push_back(word);
            pattern = pattern.substr(1, pattern.length()-1);
        }
        if(pattern.length() == 0)
        {
            break;
        }
        pos1 = pattern.find("#");
        pos2 = pattern.find(" ");
        if(pos1 == -1 && pos2 == -1)
        {
            v_words.push_back(pattern);
            break;
        }
        if(pos1 == -1 && pos2 != -1)
        {
            word = pattern.substr(0, pos2);
            v_words.push_back(word);
            pattern = pattern.substr(pos2, pattern.length()-1);
        }
        else if(pos2 == -1 && pos1 != -1)
        {
            word = pattern.substr(0, pos1);
            v_words.push_back(word);
            pattern = pattern.substr(pos1, pattern.length()-1);
        }
        else if(pos1 < pos2)
        {
            word = pattern.substr(0, pos1);
            v_words.push_back(word);
            pattern = pattern.substr(pos1, pattern.length()-1);
        }
        else if(pos1 > pos2)
        {
            word = pattern.substr(0, pos2);
            v_words.push_back(word);
            pattern = pattern.substr(pos2, pattern.length()-1);
        }
        else
        {
            std::cout << "Shouldn't be here!" << std::endl;
        }
    }
/*    for(int i=0; i<v_words.size(); i++)
    {
        std::cout << "split pattern words v_words[" << i << "]: " << v_words[i] << std::endl;
    }*/
    if(v_words.size() == 0)
    {
        return;
    }

    encoding++; //The encoding of the current key
    dictionary_node* p = T_root;
    int index = -1;
    unsigned int right = 0;
    while(right < v_words.size())
    {
        index = calIndex(v_words[right][0]);
        if(index == -1) //invalid index
        {
            return;
        }
        if(p->child[index] == NULL) //no this node, add the rest of the pattern
        {
            for(unsigned int i=right; i<v_words.size(); i++)
            {
                index = calIndex(v_words[i][0]);
                dictionary_node* newNode = createNode(v_words[i]);
                if(v_words[i].length() == 1) //single character
                {
                    newNode->feature = false;
                }
                else //multi character
                {
                    newNode->feature = true;
                }
                p->child[index] = newNode;
                p = p->child[index];
            }
            p->encoding = encoding;
            return;
        }
        else if(p->child[index]->feature == false) 
        {
            p = p->child[index];
            v_words[right] = v_words[right].substr(1, v_words[right].length()-1);
            if(v_words[right].length() == 0)
            {
                right++;
            }
        }
        else if(p->child[index]->feature == true)
        {
            unsigned int i = 0;
            std::string key_str(p->child[index]->key);
            while(i<v_words[right].length() && i<key_str.length())
            {
                if(v_words[right][i] != key_str[i])
                {
                    break;
                }
                i++;
            }
            if(i == v_words[right].length() && i == key_str.length())
            {
                if(p->child[index]->encoding == -1 && right == v_words.size()-1) //v_words[right] is substring
                {
                    p->child[index]->encoding = encoding;
                    return;
                }
                else if(p->child[index]->encoding >= 0 && right == v_words.size()-1) //key is duplicate
                {
                    encoding--;
                    return;
                }
                else
                {
                    right++;
                    p = p->child[index];
                    continue;
                }
            }
            if(i == v_words[right].length() && i < key_str.length()) //split p->child[index]->key
            {
                std::string temp_str = key_str.substr(i, key_str.length()-1); //the remanent of p->child[index]->key after removes key
                dictionary_node* newNode = createNode(temp_str);
                if(temp_str.length() == 1) //single character
                {
                    newNode->feature = false;
                }
                else //multi character
                {
                    newNode->feature = true;
                }
                newNode->encoding = p->child[index]->encoding;
                p->child[index]->encoding = -1;
                for(int k=0; k<MAX_SEN; k++) //move down the children
                {
                    if(p->child[index]->child[k])
                    {
                        newNode->child[k] = p->child[index]->child[k];
                        p->child[index]->child[k] = NULL;
                    }
                }
                int in = calIndex(temp_str[0]);
                p->child[index]->child[in] = newNode;
                copy_key(v_words[right], p->child[index]);
                key_str = std::string(p->child[index]->key);
                if(key_str.length() == 1) //single character
                {
                    p->child[index]->feature = false;
                }
                else //multi character
                {
                    p->child[index]->feature = true;
                }
                if(right == v_words.size()-1)
                {
                    p->child[index]->encoding = encoding;
                    return;
                }
                else
                {
                    right++;
                    p = p->child[index];
                    continue;
                }
                
            }
            if(i < v_words[right].length() && i == key_str.length())
            {
                v_words[right] = v_words[right].substr(i, v_words[right].length()-1);
                p = p->child[index];
                continue;
            }
            if(i < v_words[right].length() && i < key_str.length())
            {
                std::string temp_str = key_str.substr(i, key_str.length()-1); //the remanent of p->child[index]->key after removes the same characters
                dictionary_node* newNode = createNode(temp_str);
                if(temp_str.length() == 1) //single character
                {
                    newNode->feature = false;
                }
                else //multi character
                {
                    newNode->feature = true;
                }
                newNode->encoding = p->child[index]->encoding;
                for(int k=0; k<MAX_SEN; k++) //move down the children
                {
                    if(p->child[index]->child[k])
                    {
                        newNode->child[k] = p->child[index]->child[k];
                        p->child[index]->child[k] = NULL;
                    }
                }
                int in = calIndex(temp_str[0]);
                p->child[index]->child[in] = newNode;
                copy_key(key_str.substr(0, i), p->child[index]);
                key_str = std::string(p->child[index]->key);
                if(key_str.length() == 1) //single character
                {
                    p->child[index]->feature = false;
                }
                else //multi character
                {
                    p->child[index]->feature = true;
                }
                p->child[index]->encoding = -1;

                //insert the remanent key
                v_words[right] = v_words[right].substr(i, v_words[right].length()-1);
                newNode = createNode(v_words[right]);
                if(v_words[right].length() == 1) //single character
                {
                    newNode->feature = false;
                }
                else //multi character
                {
                    newNode->feature = true;
                }
                if(right == v_words.size()-1)
                {
                    newNode->encoding = encoding;
                    in = calIndex(v_words[right][0]);
                    p->child[index]->child[in] = newNode;
                    return;
                }
                else
                {
                    right++;
                    p = p->child[index];
                    continue;
                }
                
            }
        }
    }
    if(p != T_root) 
    {  
        if(p->encoding < 0)
        {
            p->encoding = encoding;
        }
        else
        {
            encoding--;
        }
    }
}

void trie_dictionary::insert(std::string key, int code)
{
    //Extract words from key 
    std::string pattern = key;
    std::vector<std::string> v_words;
    int pos1 = -1;
    int pos2 = -1;
    std::string word;
    while(pattern.length())
    {
        if(pattern[0] == '#' || pattern[0] == ' ')
        {
            word = pattern[0];
            v_words.push_back(word);
            pattern = pattern.substr(1, pattern.length()-1);
        }
        if(pattern.length() == 0)
        {
            break;
        }
        pos1 = pattern.find("#");
        pos2 = pattern.find(" ");
        if(pos1 == -1 && pos2 == -1)
        {
            v_words.push_back(pattern);
            break;
        }
        if(pos1 == -1 && pos2 != -1)
        {
            word = pattern.substr(0, pos2);
            v_words.push_back(word);
            pattern = pattern.substr(pos2, pattern.length()-1);
        }
        else if(pos2 == -1 && pos1 != -1)
        {
            word = pattern.substr(0, pos1);
            v_words.push_back(word);
            pattern = pattern.substr(pos1, pattern.length()-1);
        }
        else if(pos1 < pos2)
        {
            word = pattern.substr(0, pos1);
            v_words.push_back(word);
            pattern = pattern.substr(pos1, pattern.length()-1);
        }
        else if(pos1 > pos2)
        {
            word = pattern.substr(0, pos2);
            v_words.push_back(word);
            pattern = pattern.substr(pos2, pattern.length()-1);
        }
        else
        {
            std::cout << "Shouldn't be here!" << std::endl;
        }
    }
    if(v_words.size() == 0)
    {
        return;
    }

    dictionary_node* p = T_root;
    int index = -1;
    unsigned int right = 0;
    while(right < v_words.size())
    {
        index = calIndex(v_words[right][0]);
        if(index == -1) //invalid index
        {
            return;
        }
        if(p->child[index] == NULL) //no this node, add the rest of the pattern
        {
            for(unsigned int i=right; i<v_words.size(); i++)
            {
                index = calIndex(v_words[i][0]);
                dictionary_node* newNode = createNode(v_words[i]);
                if(v_words[i].length() == 1) //single character
                {
                    newNode->feature = false;
                }
                else //multi character
                {
                    newNode->feature = true;
                }
                p->child[index] = newNode;
                p = p->child[index];
            }
            p->encoding = code;
            return;
        }
        else if(p->child[index]->feature == false) 
        {
            p = p->child[index];
            v_words[right] = v_words[right].substr(1, v_words[right].length()-1);
            if(v_words[right].length() == 0)
            {
                right++;
            }
        }
        else if(p->child[index]->feature == true)
        {
            unsigned int i = 0;
            std::string key_str(p->child[index]->key);
            while(i<v_words[right].length() && i<key_str.length())
            {
                if(v_words[right][i] != key_str[i])
                {
                    break;
                }
                i++;
            }
            if(i == v_words[right].length() && i == key_str.length())
            {
                if(p->child[index]->encoding == -1 && right == v_words.size()-1) //v_words[right] is substring
                {
                    p->child[index]->encoding = code;
                    return;
                }
                else if(p->child[index]->encoding >= 0 && right == v_words.size()-1) //key is duplicate
                {
                    return;
                }
                else
                {
                    right++;
                    p = p->child[index];
                    continue;
                }
            }
            if(i == v_words[right].length() && i < key_str.length()) //split p->child[index]->key
            {
                std::string temp_str = key_str.substr(i, key_str.length()-1); //the remanent of p->child[index]->key after removes key
                dictionary_node* newNode = createNode(temp_str);
                if(temp_str.length() == 1) //single character
                {
                    newNode->feature = false;
                }
                else //multi character
                {
                    newNode->feature = true;
                }
                newNode->encoding = p->child[index]->encoding;
                p->child[index]->encoding = -1;
                for(int k=0; k<MAX_SEN; k++) //move down the children
                {
                    if(p->child[index]->child[k])
                    {
                        newNode->child[k] = p->child[index]->child[k];
                        p->child[index]->child[k] = NULL;
                    }
                }
                int in = calIndex(temp_str[0]);
                p->child[index]->child[in] = newNode;
                copy_key(v_words[right], p->child[index]);
                key_str = std::string(p->child[index]->key);
                if(key_str.length() == 1) //single character
                {
                    p->child[index]->feature = false;
                }
                else //multi character
                {
                    p->child[index]->feature = true;
                }
                if(right == v_words.size()-1)
                {
                    p->child[index]->encoding = code;
                    return;
                }
                else
                {
                    right++;
                    p = p->child[index];
                    continue;
                }
                
            }
            if(i < v_words[right].length() && i == key_str.length())
            {
                v_words[right] = v_words[right].substr(i, v_words[right].length()-1);
                p = p->child[index];
                continue;
            }
            if(i < v_words[right].length() && i < key_str.length())
            {
                std::string temp_str = key_str.substr(i, key_str.length()-1); //the remanent of p->child[index]->key after removes the same characters
                dictionary_node* newNode = createNode(temp_str);
                if(temp_str.length() == 1) //single character
                {
                    newNode->feature = false;
                }
                else //multi character
                {
                    newNode->feature = true;
                }
                newNode->encoding = p->child[index]->encoding;
                for(int k=0; k<MAX_SEN; k++) //move down the children
                {
                    if(p->child[index]->child[k])
                    {
                        newNode->child[k] = p->child[index]->child[k];
                        p->child[index]->child[k] = NULL;
                    }
                }
                int in = calIndex(temp_str[0]);
                p->child[index]->child[in] = newNode;
                copy_key(key_str.substr(0, i), p->child[index]);
                key_str = std::string(p->child[index]->key);
                if(key_str.length() == 1) //single character
                {
                    p->child[index]->feature = false;
                }
                else //multi character
                {
                    p->child[index]->feature = true;
                }
                p->child[index]->encoding = -1;

                //insert the remanent key
                v_words[right] = v_words[right].substr(i, v_words[right].length()-1);
                newNode = createNode(v_words[right]);
                if(v_words[right].length() == 1) //single character
                {
                    newNode->feature = false;
                }
                else //multi character
                {
                    newNode->feature = true;
                }
                in = calIndex(v_words[right][0]);
                p->child[index]->child[in] = newNode;
                p = newNode;
                if(right == v_words.size()-1)
                {
                    newNode->encoding = code;
                    return;
                }
                else
                {
                    right++;
                    continue;
                }
                
            }
        }
    }
    if(p != T_root) 
    {      
        if(p->encoding < 0)
        {
            p->encoding = code;
        }
    }
}

/*
 *Search the encoding of a key
 */
int trie_dictionary::search(std::string key)
{
    int index = -1;
    dictionary_node* p = T_root;
    while(key.length() && p != NULL)
    {
        index = calIndex(key[0]);
        if(index == -1)//invalid letter
        {
            return -1;
        }
        if(p->child[index] != NULL)
        {
            std::string key_str(p->child[index]->key);
            if(key_str == key)
            {
                return p->child[index]->encoding;
            }
            else if(key_str.length() > key.length())
            {
                return -1;
            }
            else if(key_str != key.substr(0, key_str.length()))
            {
                return -1;
            }
            else
            {
                key = key.substr(key_str.length());
                p = p->child[index];
            }
        }
        else 
        {
        	return -1;
		}
    }

    return -1;
}

dictionary_node* trie_dictionary::search_location(std::string key, dictionary_node* start)
{
   // std::cout << "search_location"<< std::endl;
    int index = -1;
    dictionary_node* p = start;
    while(key.length() && p != NULL)
    {
        index = calIndex(key[0]);
        if(index == -1)//invalid letter
        {
            return NULL;
        }
        if(p->child[index] != NULL)
        {
            std::string key_str(p->child[index]->key);
            if(key_str == key)
            {
                return p->child[index];
            }
            else if(key_str.length() > key.length())
            {
                return NULL;
            }
            else if(key_str != key.substr(0, key_str.length()))
            {
                return NULL;
            }
            else
            {
                key = key.substr(key_str.length());
                p = p->child[index];
            }
        }
        else 
        {
        	return NULL;
		}
    }
    return NULL;
}

/*
 *Encode the sentence. res_encoding: Sentence encoding sequence; ctrie_single: Common words trie
 *Return the size of res_encoding
 */
void trie_dictionary::encoding_sentence(bool &is_same_file, std::vector<std::string> &sentence, int* const res_encoding, unsigned int &offset, trie_single* const ctrie_single, std::string &additional_words)
{
    //In order to make all non-existing words of each file save into its encoded file
    if(!is_same_file) 
    {
        //Initialize encoding and new words vector
        encoding = words_num + pattern_num -1; 
        m_new_words.clear();
        is_same_file = true;
    }
    if(sentence.size() == 0)
    {
        return;
    }

/*    for(unsigned int i=0; i<sentence.size(); i++)
    {
        std::cout << "sentence[" << i << "]: " << sentence[i] << std::endl;
    }*/

    //Encoding
    unsigned int left = 0;
    while(left < sentence.size())
    {
        if(!ctrie_single->search(sentence[left])) //v_words[left] is not a common word
        {
            if(search(sentence[left]) == -1) //The word is not in dictionary
            {
                std::unordered_map<std::string,int>::const_iterator it = m_new_words.find(sentence[left]);
                if(it == m_new_words.end()) //The word has not been added to the new words list
                {
                    m_new_words.insert({sentence[left], ++encoding});
                    additional_words += sentence[left] + " ";
                    res_encoding[offset] = encoding;
                }
                else
                {
                    res_encoding[offset] = it->second;
                }
            }
            else
            {
                res_encoding[offset] = search(sentence[left]);
            }
            offset++;
            left++;
        }
        else //v_words[left] is a common word, find if it is a pattern
        {
            dictionary_node* p = T_root;
            p = search_location(sentence[left], p); //Move p to the end of the word

            int temp_encoding = -1;
            unsigned int temp_index = 0;
            std::queue< std::pair<unsigned int, std::string> > uncomm_words; //index and key
            std::vector<int> v_uncomm_num; //Uncommon words number between pattern
            int uncomm_num = 0;
            unsigned int right = left+1;

            //Find pattern encoding
            while(p != NULL && right < sentence.size())
            {
                if(ctrie_single->search(sentence[right]))
                {
                    //Two contiguous common words, there is a space between them in the dictionary
                    if(ctrie_single->search(sentence[right-1]))
                    {
                        p = p->child[calIndex(' ')];
                        if(p == NULL)
                        {
                            break;
                        }
                    }
                    p = search_location(sentence[right], p);
                    
                    right++;
                    if(uncomm_num) //Remember the number of uncommon words
                    {
                        v_uncomm_num.push_back(uncomm_num);
                        uncomm_num = 0;
                    }
                }
                else
                {
                    if(strcmp(p->key, "#") != 0)
                    {
                        p = p->child[calIndex('#')];
                        if(p == NULL)
                        {
                            break;
                        }
                    }
                    uncomm_num++;
                    uncomm_words.push(std::make_pair(right, sentence[right]));
                    right++;
                }
                if(p && p->encoding >= 0 && p->encoding < pattern_num) //This ensure the encoding is a pattern
                {
                    temp_encoding = p->encoding;
                    temp_index = right-1;
                }
            }
            if(temp_encoding >= 0 && temp_encoding < pattern_num) //find a pattern 
            {
                //left to temp_index is pattern
                res_encoding[offset] = temp_encoding;//pattern encoding
                offset++;
                for(unsigned int n=0; n<v_uncomm_num.size(); n++)
                {
                    if(uncomm_words.front().first > temp_index) 
                    {
                        break;
                    }
                    res_encoding[offset] = v_uncomm_num[n];//uncommon words number
                    offset++;

                    for(int w=0; w<v_uncomm_num[n]; w++)
                    {
                        if(search(uncomm_words.front().second) == -1) //The word is not in dictionary, then add it
                        {
                            std::unordered_map<std::string,int>::const_iterator it = m_new_words.find(uncomm_words.front().second);
                            if(it == m_new_words.end())
                            {
                                m_new_words.insert({uncomm_words.front().second, ++encoding});
                                additional_words += uncomm_words.front().second + " ";
                                res_encoding[offset] = encoding;
                            }
                            else
                            {
                                res_encoding[offset] = it->second;
                            }
                        }
                        else
                        {
                            res_encoding[offset] = search(uncomm_words.front().second);
                        }
                        offset++;
                        uncomm_words.pop();
                    }
                }
                
                left = temp_index+1;
                p = T_root;
            }
            else //No pattern
            {
                if(search(sentence[left]) == -1) //The word is not in dictionary, then add it
                {
                    std::unordered_map<std::string,int>::const_iterator it = m_new_words.find(sentence[left]);
                    if(it == m_new_words.end())
                    {
                        m_new_words.insert({sentence[left], ++encoding});
                        additional_words += sentence[left] + " ";
                        res_encoding[offset] = encoding;
                    }
                    else
                    {
                        res_encoding[offset] = it->second;
                    }
                }
                else
                {
                    res_encoding[offset] = search(sentence[left]);
                }
                offset++;
                left++;
            }
        }
    }
//    res_encoding[offset] = INT32_MAX;
//    offset++;
}

void trie_dictionary::output_d(dictionary_node* node, std::string output_word)
{
    std::string key_str(node->key);
    output_word = output_word + key_str;
    for(int i=0; i<MAX_SEN; i++)
    {
        if(node->child[i] != NULL)
        {
            output_d(node->child[i], output_word);
        }
    }

    if(node->encoding >= 0) {
        std::cout << "key: " << output_word << "  encoding: " << node->encoding << std::endl;
    }
}
/*
 *print out the words information in the trie
 */
void trie_dictionary::display()
{
    std::string output_word = "";
    output_d(T_root, output_word);
}



