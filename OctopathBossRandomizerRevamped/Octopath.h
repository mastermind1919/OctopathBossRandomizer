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