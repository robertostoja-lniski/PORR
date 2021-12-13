#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <stdlib.h>
#include <algorithm>
#include <cmath>
#include <omp.h>
#include <map>
#include <climits>
#include <atomic>

#define PROGRESS_CHECK 0

using cntMap = std::map<std::string, std::atomic<int>>;

void removeNotAllowedChars(std::string& word) {
    std::string res;
    word.erase(std::remove_if(word.begin(), word.end(), [](auto const& c) {return !std::isalpha(c);}), word.end());
    std::transform(word.begin(), word.end(), word.begin(), [](auto const& c){ return std::tolower(c); });
//    remove words with duplicated letter
    if (word.empty() || word.size() < 2) {
        return;
    }

    for (int i = 0; i < word.size() - 1; i++) {
        if (word[i] == word[i + 1]) {
            return;
        }
    }
}

void readData(std::vector<std::string>& words, cntMap& map, std::string filename) {
    std::fstream file;
    std::string word;
    file.open(filename);

    while (file >> word){
        removeNotAllowedChars(word);
        if (!word.empty()) {
            words.push_back(word);
            map.emplace(word, 0);
        }
    }
}

void printPogres(size_t curr, size_t total) {
    size_t step = 100;
    size_t chunk = total / step;
    if(curr % chunk == 0) {
        if (curr != 0) {
            std::cout << std::lround(curr * step / static_cast<double>(total)) << "%" << std::endl;
        }
    }
}

double checkExecTime(cntMap& map, std::vector<std::string>& words) {
    size_t size = words.size();
    std::cout << "size of " << size << std::endl;
    if (words.size() > INT_MAX) {
        throw std::runtime_error("Shortening dataset may lead to unexpected results! Choose smaller dataset");
    }
    int words_size = words.size(); //zeby for bylo w kanonicznej postaci
    auto start = std::chrono::high_resolution_clock::now();

    {
        for (size_t i = 0; i < words_size ; i++) {
            map[words[i]]+=1;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    return elapsedTime;
}

double checkOmpExecTime(cntMap& map, std::vector<std::string>& words, int thread) {
    size_t size = words.size();
    std::cout << "size of " << size << std::endl;
    if (words.size() > INT_MAX) {
        throw std::runtime_error("Shortening dataset may lead to unexpected results! Choose smaller dataset");
    }
    int words_size = words.size(); //zeby for bylo w kanonicznej postaci
	omp_set_num_threads(thread);
    auto start = std::chrono::high_resolution_clock::now();

	#pragma omp parallel shared(words_size)
    {
		#pragma omp for
        for (size_t i = 0; i < words_size ; i++) {
            map[words[i]]+=1;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    return elapsedTime;
}

void printResult(const cntMap& map, int num) {

    std::vector<std::pair<std::string, int>> res;
    for(auto const& elem : map) {
        res.push_back(elem);
    }

    sort(res.begin(), res.end(), [](auto a, auto b) {return a.second > b.second;});
    for (int i = 0; i < res.size() && i < num; i++) {
        std::cout << i << ": " << res[i].first << " " << res[i].second << std::endl;
    }
}

int main(int argc, char** argv) {

    if (argc != 3) {
        std::cerr << "run ./app file num_threads" << std::endl;
        return 1;
    }

    int threadNum;
    try {
        threadNum = std::atoi(argv[2]);
    } catch (...) {
        std::cerr << "Not a numeric thread num!" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
	std::vector<std::string> words;

    cntMap initMap;
    readData(words, initMap, filename);

    cntMap map;
    auto execTime = checkExecTime(map, words);
    printResult(map, 500);

    std::cout << "Thread num, elapsed, optimised elapsed, shortening percent" << std::endl;
    for (int i = 1; i <= threadNum; i++) {
        auto ompExecTime = checkOmpExecTime(initMap, words, i);

        for (auto& [key, val] : initMap) {
            initMap.emplace(key, 0);
        }

        std::cout << "|" << i << "|" << execTime << "|" << ompExecTime << "|";
        std::cout << 100 - static_cast<double>(ompExecTime) * 100 / static_cast<double>(execTime) << std::endl;
    }

    return 0;
} 
