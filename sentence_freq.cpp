#include "sentence_freq.h"
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <unistd.h>
#include <cstring>


sentence_freq::sentence_freq()
{
    sentences_num = 0;

    /*
     *get the files name in ./file folder
     *reference from: https://blog.csdn.net/adong76/article/details/39432467
     */
    struct dirent *ptr;
    DIR *dir;
    char *wd(getcwd(NULL,0));
    std::string cwd(wd);
    std::string PATH = cwd + "/file";
    dir=opendir(PATH.c_str());

    while((ptr=readdir(dir))!=NULL)
    {
        if(ptr->d_name[0] == '.')
            continue;
        vfname.push_back(ptr->d_name);
    }

    closedir(dir);
}

sentence_freq::~sentence_freq()
{}

bool sentence_freq::is_letter(char con)
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
bool sentence_freq::is_end_of_sentence(std::string &word)
{
    bool is_end = false;
    while(word.length()) //Remove non-letter from begin of the word
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

void sentence_freq::lower_case(std::string &word)
{
    for(unsigned int i=0; i<word.length(); i++)
    {
        if(word[i] >= 'A' && word[i] <= 'Z')
        {
            word[i] = word[i] + 32; //change the upper-case letter to lower-case
        }
    }
}

bool sentence_freq::is_regular_word(std::string &word) //Word that all characters are letters
{
    if(word.length() == 0)
    {
        return false;
    }
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
            if(word.find("-", i+1) == -1)
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

int sentence_freq::wordsInPattern(std::string pattern)
{
	int num = 0;
	int pos_space = -1;
    int pos_hash = -1;
    while(pattern.length())
    {
        pos_space = pattern.find_first_of(" ");
        pos_hash = pattern.find_first_of("#");
        if(pos_space == -1 && pos_hash == -1)
        {
            num++;
            break;
        }
        else if(pos_space == -1 && pos_hash != -1)
        {
            pattern = pattern.find_first_of(pos_hash+1, pattern.length()-pos_hash);
            num++;
        }
        else if(pos_space != -1 && pos_hash == -1)
        {
            pattern = pattern.substr(pos_space+1, pattern.length()-pos_space);
            num++;
        }
        else if(pos_space < pos_hash)
        {
            pattern = pattern.substr(pos_space+1, pattern.length()-pos_space);
            num++;
        }
        else if(pos_space > pos_hash)
        {
            pattern = pattern.find_first_of(pos_hash+1, pattern.length()-pos_hash);
            num++;
        }
    }
	return num;
}

/*
 *Read the file and add the patterns into tire 
 */
void sentence_freq::insert_sentences(std::string fname, trie_sentence *const ctrie_sentence, trie_single *const ctrie_single)
{	
	std::fstream filecon;
    fname = "./file/" + fname;
	filecon.open(fname.c_str());
	if(!filecon.is_open())
	{
		std::cout << "File: " << fname << " opened failed!" << std::endl;
		return;
	}

    std::string word = "";
	std::string pattern = "";
    int neighbor_distance = 0;
	while(filecon >> word)
	{
		bool isEnd = is_end_of_sentence(word); //Judge the char is valid
        lower_case(word);
		if(!isEnd) //Word is not the end of a sentence
		{
            if(ctrie_single->search(word))//Common word
            {
                if(neighbor_distance <= NEIGH_DISTANCE)
                {
                    if(pattern.length() != 0 && pattern[pattern.length()-1] != '#')
                    {
                        pattern += " ";
                    }
                    pattern += word;
                }
                else //the distance of two common words longer than NEIGH_DISTANCE, insert the pattern and begin a new one
                {
                    if(pattern[pattern.length()-1] == '#')
                    {
                        pattern = pattern.substr(0, pattern.length()-1);
                    }
                    if(pattern.length() > 0 && wordsInPattern(pattern) > 1)
                    {
                        ctrie_sentence->insert(pattern);
                    }
                    pattern = ""; //Reset the pattern
                    pattern += word;
                }   
                neighbor_distance = 0;              
            }
            else
            {
                neighbor_distance++;
                if(pattern.length() != 0 && pattern[pattern.length()-1] != '#') 
                {
                    pattern += '#';
                }
            }
		}
		else if(isEnd) //Word is the end of the sentence
		{
            if(ctrie_single->search(word))
            {
                if(pattern.length() != 0 && pattern[pattern.length()-1] != '#')
                {
                    pattern += " ";
                }
                pattern += word;
            }
                if(pattern.length() > 0 && pattern[pattern.length()-1] == '#')
                {
                    pattern = pattern.substr(0, pattern.length()-1);
                }
			if(pattern.length() > 0 && wordsInPattern(pattern) > 1)
			{
				ctrie_sentence->insert(pattern);
                sentences_num++;
			}
            pattern = ""; // Reset the pattern
            neighbor_distance = 0;
		}
	}
    filecon.close();
}

/*
 *write the sorded words information into out_file
 */
void sentence_freq::write_to_file(std::string out_fname, max_heap *const cmax_heap)
{
    std::ofstream filelist;
    filelist.open(out_fname, std::ios::out); //read only, get the document files name
    if(!filelist.is_open())
    {
    	std::cout << "File: " << out_fname << " opened failed!" << std::endl;
    	return;
    }
    filelist << "Total number of sentences: " << sentences_num << "; List format: key	frequency	percentage" << '\n';

    unsigned int heap_size = cmax_heap->size();
    for(unsigned int i=0; i<heap_size; i++)
	{
		heap_node extract_node = cmax_heap->extractMax();
		filelist << extract_node.key << "\t" << extract_node.freq << "\t" << (float)extract_node.freq/(float)sentences_num*100 << '\n';
	}
    filelist.close();
}
/*
 *read the files in the list and calculate the frequency of the sentences
 */
void sentence_freq::cal_sentences_freq(trie_single *const ctrie_single, std::string out_fname)
{
    if(vfname.size() == 0)
    {
        std::cout << "No file to read!" << std::endl;
        return;
    }
    trie_sentence *const ctrie_sentence = new trie_sentence();
    for(unsigned int i=0; i<vfname.size(); i++)
    {
        insert_sentences(vfname[i], ctrie_sentence, ctrie_single);
    }
    max_heap *const cmax_heap = new max_heap();
	ctrie_sentence->traverse(cmax_heap, 40);

    write_to_file(out_fname, cmax_heap);

    delete ctrie_sentence;
	delete cmax_heap;
}



