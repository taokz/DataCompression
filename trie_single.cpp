#include "trie_single.h"
#include <iostream>
#include <cstring>

trie_single::trie_single()
{
    T_root = new trie_node; //root doesn't store key
    T_root->key = new char[2]; //use * represents the root key
    char r[] = "*";
    strcpy(T_root->key, r);
    for(int i=0; i<Max; i++)
    {
        T_root->child[i] = NULL;
    }
    T_root->freq = 0;
    words_num = 0;
}

/*
 * delete trie node
 */
void trie_single::delNode(trie_node* node)
{
    for(int i=0; i<Max; i++)
    {
        if(node->child[i] != NULL)
        {
            delNode(node->child[i]);
        }
    }
    delete node->key;
    delete node;
}

trie_single::~trie_single()
{
    delNode(T_root);
}

/*
 *copy key
 */
void trie_single::copy_key(std::string key, trie_node* node)
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
 * create a new node
 */
trie_node* trie_single::createNode(std::string key)
{
    trie_node* newNode = new trie_node;
    newNode->key = NULL;
    copy_key(key, newNode);

    for(int i=0; i<Max; i++)
    {
        newNode->child[i] = NULL;
    }
    newNode->freq = 0;

    return newNode;
}

/*
 * calculate the index of the letter
 */
int trie_single::calIndex(char letter)
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
    else if(letter == '-')
    {
        index = 27;
    }
    return index;
}

/*
 *insert a key word
 *1. word exists: word count(the "freq" field of the leaf node) plus 1 
 *2. word prefix exists: insert the remain letters of the word in the child node of the prefix node
 *3. word dosen't exist(all the letters not exist): insert the word in the child node of the root node
 */
void trie_single::insert(std::string key)
{
    trie_node* p = T_root;
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
            trie_node* newNode = createNode(key);
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
            words_num++;
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
                if(p->freq == 0)
                {
                    words_num++;
                }

                p->child[index]->freq++;
                return;
            }
            if(i == key.length() && i < key_str.length()) //split p->child[index]->key
            {
                std::string temp_str = key_str.substr(i, key_str.length()-1); //the remanent of p->child[index]->key after removes key
                trie_node* newNode = createNode(temp_str);
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
                words_num++;
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
                trie_node* newNode = createNode(temp_str);
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
                words_num++;
                return;
            }
        }
    }
    if(p != T_root) 
    {
        if(p->freq == 0)
        {
            words_num++;
        }
        p->freq++;
    }
}

/*
 *search the word and return the frequence of the word
 */
unsigned int trie_single::search(std::string key)
{
    int index = -1;
    trie_node* p = T_root;
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


void trie_single::output_d(trie_node* node, std::string output_word)
{
    std::string key_str(node->key);
    output_word = output_word + key_str;
    for(int i=0; i<Max; i++)
    {
        if(node->child[i] != NULL)
        {
            output_d(node->child[i], output_word);
        }
    }

    if(node->freq > 0) {
        std::cout << "word: " << output_word << "  frequence: " << node->freq << std::endl;
    }
}
/*
 *print out the words information in the trie
 */
void trie_single::display()
{   std::cout << "display trie_single" << std::endl;
    std::string output_word = "";
    output_d(T_root, output_word);
}


void trie_single::output_tr(trie_node* node, max_heap* cMax_Heap, std::string output_word)
{
    if(strcmp(node->key, "*") != 0) //exclude root node
    {
        std::string key_str(node->key);
        output_word = output_word + key_str;
    }
    
    for(int i=0; i<Max; i++)
    {
        if(node->child[i] != NULL)
        {
            output_tr(node->child[i], cMax_Heap, output_word);
        }
    }

    if(node->freq > 0) {
        heap_node newNode(output_word, node->freq);
        cMax_Heap->insertKey(newNode);
    }
}
/*
 *output the words information into maxheap
 */
void trie_single::traverse(max_heap* cmax_heap)
{
    std::string output_word = "";
    output_tr(T_root, cmax_heap, output_word);
}



