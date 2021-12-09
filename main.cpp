#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <stdlib.h>
#include <omp.h>

int main() {
	std::fstream file;
    std::string word;
	auto filename = "./test.ft.txt";
	file.open(filename);
	std::vector<std::string> words;	
	int total = 0; //suma zliczonych slow przez wszystkie watki

	while (file >> word)
    {
	  	words.push_back(word);
	}
	int size = words.size();
	std::cout << "size of " << size << std::endl;
	auto start = std::chrono::high_resolution_clock::now();
	int words_size = words.size(); //zeby for bylo w kanonicznej postaci
	omp_set_num_threads(2); 
	#pragma omp parallel shared(words_size)
	{
		int k=0; //for jest taka pusta operacja, chodzi o same wykorzystanie podzialu na watki
		#pragma omp for
	  		for (int i = 0; i < words_size ; i++) {k += 1;}
		total+=k;
	}
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << "Elapsed: " << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << std::endl;
	std::cout << "words.size(): " << words.size() << std::endl;

    std::cout << "TOTAL_COUNTED: " << total << std::endl;
    return 0;
} 
