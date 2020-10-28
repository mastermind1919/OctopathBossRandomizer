#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <random>
#include <map>
#include <vector>
#include <deque>


using namespace std;

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
vectorvector baseRandomize(mt19937 rng, bool randomizeShrineBosses, bool randomizeGateBosses, bool includeDuplicate);
vectorvector includeRandomize(mt19937 rng, bool randomizeShrineBosses, bool includeShrineBosses, bool randomizeGateBosses, bool includeGateBosses, bool includeGaldera, bool includeDuplicate);
vectorvector mixRandomize(mt19937 rng, bool mixChapter24, bool mixChapter14, bool randomizeShrineBosses, bool includeShrineBosses, bool randomizeGateBosses, bool includeGateBosses, bool includeGaldera, bool includeDuplicate);
vectorvector trueRandom(mt19937 rng);
int randomToFile(vectorvector input);
void spoiler(deque<int> input);
vector<bool> configParser(string input);
bool matrixChecker(vectorvector input, int ranboss, int endRange);
bool excludeChecker(int input, bool includeShrineBosses, bool includeGateBosses, bool includeGaldera);