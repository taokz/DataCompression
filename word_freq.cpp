#include "word_freq.h"
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <unistd.h>


word_freq::word_freq()
{
    words_num = 0;

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

word_freq::~word_freq()
{}

bool word_freq::is_letter(char con)
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
 *Remove the punctuations from word
 */
void word_freq::remove_punctuation(std::string &word)
{
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
}

/*
 * Word that all characters are letters
 */
bool word_freq::is_regular_word(std::string &word) 
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


/*
 *Read the file and add the words into tire 
 */
void word_freq::insert_words(std::string fname, trie_single *const ctrie_single)
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
	while(filecon >> word)
	{
        remove_punctuation(word);
		bool isRegWord = is_regular_word(word); //Judge the char is valid
		if(isRegWord) //If the char is valid, it means this is not the end of a word
		{
			ctrie_single->insert(word);
		}
	}
	filecon.close();
}

/*
 *write the sorded words information into out_file
 */
void word_freq::write_to_file(std::string out_fname, max_heap *const cmax_heap)
{
    std::ofstream filelist;
    filelist.open(out_fname, std::ios::out); //read only, get the document files name
    if(!filelist.is_open())
    {
    	std::cout << "File: " << out_fname << " opened failed!" << std::endl;
    	return;
    }
    filelist << "Total number of words: " << words_num << "; List format: key	frequency	percentage" << '\n';

    int heap_size = cmax_heap->size();
    for(int i=0; i<heap_size; i++)
	{
		heap_node extract_node = cmax_heap->extractMax();
		filelist << extract_node.key << "\t" << extract_node.freq << "\t" << (float)extract_node.freq/(float)words_num*100 << '\n';
	}
    filelist.close();
}
/*
 *read the files in the list and calculate the frequency of the words
 */
void word_freq::cal_words_freq(std::string out_fname)
{
    if(vfname.size() == 0)
    {
        std::cout << "No file to read!" << std::endl;
        return;
    }
    trie_single *const ctrie_single = new trie_single();
    for(unsigned int i=0; i<vfname.size(); i++)
    {
        insert_words(vfname[i], ctrie_single);
    }
    words_num = ctrie_single->get_words_num();
    max_heap *const cmax_heap = new max_heap();
	ctrie_single->traverse(cmax_heap);

    write_to_file(out_fname, cmax_heap);
    delete ctrie_single;
	delete cmax_heap;
}

void word_freq::get_common_words(std::string fname, trie_single *const ctrie_single, unsigned int limit_count)
{
    std::ifstream filelist;
    filelist.open(fname); //read only, get the document files name
    if(!filelist.is_open())
    {
        std::cout << "File: " << fname << " opened failed!" << std::endl;
        return;
    }
    std::string dummyLine;
    getline(filelist, dummyLine);//skip the first line
    
    std::string key;
    std::string count;
    std::string freq;
    unsigned int i = limit_count;
    while(i >= limit_count && !filelist.eof())
    {    
        filelist >> key >> count >> freq;
        i = std::stoi(count, nullptr);
        ctrie_single->insert(key);
    }
    filelist.close();
}
