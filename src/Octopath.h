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
vectorvector includeRandomize(std::mt19937 rng, vectorvector fixedVector, bool randomizeShrineBosses, bool includeShrineBosses, bool randomizeGateBosses, bool includeGateBosses, bool includeGaldera, bool includeDuplicate);
vectorvector mixRandomize(std::mt19937 rng, vectorvector fixedVector, bool mixChapter24, bool mixChapter14, bool randomizeShrineBosses, bool includeShrineBosses, bool randomizeGateBosses, bool includeGateBosses, bool includeGaldera, bool includeDuplicate);
vectorvector trueRandomFunction(std::mt19937 rng, vectorvector fixedVector);
vectorvector fixedTeir(std::mt19937 rng, vectorvector inputVector, std::vector<int> forceNumbers, bool forceGaldera, bool includeDuplicate);
int randomToFile(vectorvector inputVector, unsigned int seedInput);
void spoiler(std::deque<int> input, unsigned int seedInput);
std::tuple <std::vector<bool>, std::vector<int>> configParser(std::string input, std::wstring* pakPathPointer);
bool configWriter(std::string input, bool mixChapter24Bosses, bool mixChapter14Bosses, bool randomizeShrineBosses, bool includeShrineBosses, bool randomizeGateBosses, bool includeGateBosses, bool includeGaldera, bool enableDuplicate, bool trueRandom, std::wstring pakPathPointer, bool forceBossesOption, std::vector<int> forceBossesVector, bool forceGaldera); bool matrixChecker(vectorvector input, int ranboss, int endRange);
bool excludeChecker(int input, bool includeShrineBosses, bool includeGateBosses, bool includeGaldera);
std::vector<int> insertRandom(std::mt19937 rng, std::vector<int> randomInput, std::vector<int> fixedInput);