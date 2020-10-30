#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <random>
#include <map>
#include <vector>
#include <deque>


// Double vector, a matrix essentially
typedef std::vector<std::vector<int>> vectorvector;
// int vector
typedef std::vector<int> intvector;

// for comparing vector to int
struct compare {
    int key;
    compare(int const& i) : key(i) { }

    bool operator()(int const& i) {
        return (i == key);
    }
};

// Function definitions for cross file functions
vectorvector baseRandomize(std::mt19937 rng, bool randomizeShrineBosses, bool randomizeGateBosses, bool includeDuplicate);
vectorvector includeRandomize(std::mt19937 rng, bool randomizeShrineBosses, bool includeShrineBosses, bool randomizeGateBosses, bool includeGateBosses, bool includeGaldera, bool includeDuplicate);
vectorvector mixRandomize(std::mt19937 rng, bool mixChapter24, bool mixChapter14, bool randomizeShrineBosses, bool includeShrineBosses, bool randomizeGateBosses, bool includeGateBosses, bool includeGaldera, bool includeDuplicate);
vectorvector trueRandomFunction(std::mt19937 rng);
int randomToFile(vectorvector input);
void spoiler(std::deque<int> input);
std::vector<bool> configParser(std::string input, std::wstring* pakPathPointer);
bool configWriter(std::string input, bool mixChapter24Bosses, bool mixChapter14Bosses, bool randomizeShrineBosses, bool includeShrineBosses, bool randomizeGateBosses, bool includeGateBosses, bool includeGaldera, bool enableDuplicate, bool trueRandom, std::wstring pakPathPointer);
bool matrixChecker(vectorvector input, int ranboss, int endRange);
bool excludeChecker(int input, bool includeShrineBosses, bool includeGateBosses, bool includeGaldera);