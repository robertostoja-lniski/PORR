#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

int main() {

    std::fstream file;
    std::string word;
    auto filename = "./amazon_reviews.txt";

    file.open(filename);
    std::vector<std::string> words;

    auto start = std::chrono::high_resolution_clock::now();

    while (file >> word)
    {
        words.push_back(word);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Elasped: " << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << std::endl;
    std::cout << "Vec count: " << words.size() << std::endl;

    return 0;
}
