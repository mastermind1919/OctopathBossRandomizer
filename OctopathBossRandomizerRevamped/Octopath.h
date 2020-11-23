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
// bool vector
typedef std::vector<bool> boolvector;

// for comparing vector to int
struct compare {
    int key;
    compare(int const& i) : key(i) { }

    bool operator()(int const& i) {
        return (i == key);
    }
};

// Cross file function declerations
intvector configParser(std::string input, std::wstring* pakPathPointer);
void configWriter(std::string input, intvector configs, wchar_t* pakPath);
vectorvector randomizeBosses(std::mt19937 rng, vectorvector fixedBosses, int forcedCharacter, bool mixChapter14, bool mixChapter24, bool randomizeShrine, bool includeShrine, bool randomizeGate, bool includeGate, bool includeGaldera, bool includeDuplicate, bool fullRandom);
vectorvector fixedTier(std::mt19937 rng, intvector config);
void spoiler(std::deque<int> input, unsigned int seedInput);
void soloSpoiler(std::deque<int> input, unsigned int seedInput, std::deque<int> characters);
int randomToFile(std::mt19937 rng, vectorvector inputVector, bool soloTraveler, unsigned int seedInput);