#include "trie_sentence.h"
#include <iostream>
#include <cstring>

trie_sentence::trie_sentence()
{
    T_root = new sentence_trie_node; //Root doesn't store key
    T_root->key = new char[2]; //use * represents the root key
    char r[] = "*";
    strcpy(T_root->key, r);
    T_root->freq = 0;
    for(int i=0; i<MAX_SEN; i++)
    {
        T_root->child[i] = NULL;
    }
}

/*
 *Delete the nodes of trie
 */
void trie_sentence::delNode(sentence_trie_node* node)
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

trie_sentence::~trie_sentence()
{
    delNode(T_root);
}

/*
 *copy key
 */
void trie_sentence::copy_key(std::string key, sentence_trie_node* node)
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
 *Create a new trie node
 */
sentence_trie_node* trie_sentence::createNode(std::string character)
{
    sentence_trie_node* newNode = new sentence_trie_node;
    newNode->key = NULL;
    copy_key(character, newNode);
    newNode->freq = 0;

    for(int i=0; i<MAX_SEN; i++)
    {
        newNode->child[i] = NULL;
    }

    return newNode;
}

/*
 *Calculate the index of a letter.
 */
int trie_sentence::calIndex(char letter)
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
 *Input a pattern into trie
 */
void trie_sentence::insert(std::string key)
{
    sentence_trie_node* p = T_root;
    int index = -1;
    while(key.length())
    {
        index = calIndex(key[0]);
        if(index == -1) //invalid index
        {
            return;
        }
        if(p->child[index] == NULL) //no this node, add it
        {
            sentence_trie_node* newNode = createNode(key);
            if(key.length() == 1) //single character
            {
                newNode->feature = false;
            }
            else //multi character
            {
                newNode->feature = true;
            }
            newNode->freq++;
            p->child[index] = newNode;
            return;
        }
        else if(p->child[index]->feature == false) 
        {
            p = p->child[index];
            key = key.substr(1, key.length()-1);
        }
        else if(p->child[index]->feature == true)
        {
            unsigned int i = 0;
            std::string key_str(p->child[index]->key);
            while(i<key.length() && i<key_str.length())
            {
                if(key[i] != key_str[i])
                {
                    break;
                }
                i++;
            }
            if(i == key.length() && i == key_str.length())
            {
                p->child[index]->freq++;
                return;
            }
            if(i == key.length() && i < key_str.length()) //split p->child[index]->key
            {
                std::string temp_str = key_str.substr(i, key_str.length()-1); //the remanent of p->child[index]->key after removes key
                sentence_trie_node* newNode = createNode(temp_str);
                if(temp_str.length() == 1) //single character
                {
                    newNode->feature = false;
                }
                else //multi character
                {
                    newNode->feature = true;
                }
                newNode->freq = p->child[index]->freq;
                for(int k=0; k<Max; k++) //move down the children
                {
                    if(p->child[index]->child[k])
                    {
                        newNode->child[k] = p->child[index]->child[k];
                        p->child[index]->child[k] = NULL;
                    }
                }
                int in = calIndex(temp_str[0]);
                p->child[index]->child[in] = newNode;
                copy_key(key, p->child[index]);
                key_str = std::string(p->child[index]->key);
                if(key_str.length() == 1) //single character
                {
                    p->child[index]->feature = false;
                }
                else //multi character
                {
                    p->child[index]->feature = true;
                }
                p->child[index]->freq = 1;
                return;
            }
            if(i < key.length() && i == key_str.length())
            {
                key = key.substr(i, key.length()-1);
                p = p->child[index];
                continue;
            }
            if(i < key.length() && i < key_str.length())
            {
                std::string temp_str = key_str.substr(i, key_str.length()-1); //the remanent of p->child[index]->key after removes the same characters
                sentence_trie_node* newNode = createNode(temp_str);
                if(temp_str.length() == 1) //single character
                {
                    newNode->feature = false;
                }
                else //multi character
                {
                    newNode->feature = true;
                }
                newNode->freq = p->child[index]->freq;
                for(int k=0; k<Max; k++) //move down the children
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
                p->child[index]->freq = 0;

                //insert the remanent key
                key = key.substr(i, key.length()-1);
                newNode = createNode(key);
                if(key.length() == 1) //single character
                {
                    newNode->feature = false;
                }
                else //multi character
                {
                    newNode->feature = true;
                }
                newNode->freq++;
                in = calIndex(key[0]);
                p->child[index]->child[in] = newNode;
                return;
            }
        }
    }
    if(p != T_root) 
    {
        p->freq++;
    }
}

/*
 *Search a word in the trie
 */
unsigned int trie_sentence::search(std::string key)
{
    int index = -1;
    sentence_trie_node* p = T_root;
    while(key.length())
    {
        index = calIndex(key[0]);
        if(index == -1)//invalid letter
        {
            return 0;
        }
        if(p->child[index] != NULL)
        {
            std::string key_str(p->child[index]->key);
            if(key_str == key)
            {
                return p->child[index]->freq;
            }
            else if(key_str.length() > key.length())
            {
                return 0;
            }
            else if(key_str != key.substr(0, key_str.length()))
            {
                return 0;
            }
            else
            {
                p = p->child[index];
                key = key.substr(1, key.length()-1);
            }
        }
        else 
        {
        	return 0;
		}
    }

    return 0;
}

/*
 *Output trie node for debuging
 */
void trie_sentence::output_tr(sentence_trie_node* node, max_heap* cmax_heap, std::string output_word, unsigned int limit_freq)
{
    if(strcmp(node->key, "*") != 0) //exclude root node
    {
        std::string key_str(node->key);
        output_word = output_word + key_str;
    }
    
    for(int i=0; i<MAX_SEN; i++)
    {
        if(node->child[i] != NULL)
        {
            output_tr(node->child[i], cmax_heap, output_word, limit_freq);
        }
    }

    if(node->freq >= limit_freq) {
        heap_node newNode(output_word, node->freq);
        cmax_heap->insertKey(newNode);
    }
}

void trie_sentence::traverse(max_heap* cmax_heap, unsigned int limit_freq)
{
    std::string output_word = "";
    output_tr(T_root, cmax_heap, output_word, limit_freq);
}


void trie_sentence::output_d(sentence_trie_node* node, std::string output_word)
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

    if(node->freq > 0) {
        std::cout << "pattern: " << output_word << "  frequence: " << node->freq << std::endl;
    }
}
/*
 *print out the words information in the trie
 */
void trie_sentence::display()
{
    std::string output_word = "";
    output_d(T_root, output_word);
}


