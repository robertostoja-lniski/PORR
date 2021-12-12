#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <stdlib.h>
#include <omp.h>
#include <map>

int main() {
	std::fstream file;
    std::string word;
	auto filename = "./test.ft.txt";
	file.open(filename);
	std::vector<std::string> words;	

	while (file >> word){
	  	words.push_back(word);
	}
	int size = words.size();
	std::cout << "size of " << size << std::endl;
	auto start = std::chrono::high_resolution_clock::now();
	int words_size = words.size(); //zeby for bylo w kanonicznej postaci
	omp_set_num_threads(5); 

	std::map<std::string, int> map;
	#pragma omp parallel shared(words_size)
	{
		#pragma omp for
	  		for (int i = 0; i < words_size ; i++) {
	  			map[words[i]]+=1;
	  		}
	}
	auto end = std::chrono::high_resolution_clock::now();
	
	int k = 0;
	for(auto const& x : map){
		std::cout << k << ": " << x.first << " " << x.second << std::endl;
		k++;
		if(k>500)//wypisanie pierwszych elementow mapy, tylko do testu czy
			break;
	}
	std::cout << "words.size(): " << words.size() << std::endl;
    std::cout << "Elapsed: " << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << std::endl;
    
    return 0;
} 
