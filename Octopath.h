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
vectorvector baseRandomize(bool randomizeShrineBosses, bool randomizeGateBosses);
int randomToFile(vectorvector input);
void spoiler(deque<int> input);
vector<bool> configParser(string input);