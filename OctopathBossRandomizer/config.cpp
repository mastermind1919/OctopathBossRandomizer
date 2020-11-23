#include "Octopath.h"
#include <windows.h>
// use Unicode
#ifndef UNICODE
#define UNICODE
#endif


intvector configParser(std::string input, std::wstring* pakPathPointer) {

    // Use simpler config writer with a psuedohashing program
    intvector configs(18);
    std::wstring lines;
    std::wifstream configFile(input.c_str());
    if (configFile.is_open() == true) {
        for (std::string line; std::getline(configFile, lines); ) {
            // ignore lines containing "//" and empty lines
            if (lines.find_first_of(L"//") == std::string::npos && lines != L"") {
                // Stores the location of quotes
                intvector quotes;
                int evenTest = 0;
                for (int i = 0; i < lines.size(); i++) {
                    if (lines[i] == '\"') {
                        if (evenTest == 0 || evenTest == 2) {
                            quotes.push_back(i + 1);
                            evenTest++;
                        }
                        else {
                            quotes.push_back(i - 1);
                            evenTest++;
                        }
                    }
                }
                // The first portion of the line is the Alphabetic hash of the options
                std::wstring hashing = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
                // Mixing Options
                if (lines.at(0) == hashing.at(17)) {
                    configs[0] = 0;
                }
                if (lines.at(0) == hashing.at(15)) {
                    configs[0] = 1;
                }
                if (lines.at(0) == hashing.at(14)) {
                    configs[0] = 2;
                }

                // Randomize Shrine Options
                if (lines.at(1) == hashing.at(1)) {
                    configs[1] = 0;
                }
                if (lines.at(1) == hashing.at(7)) {
                    configs[1] = 1;
                }
                if (lines.at(1) == hashing.at(4)) {
                    configs[1] = 2;
                }

                // Randomize Gate Options
                if (lines.at(2) == hashing.at(7)) {
                    configs[2] = 0;
                }
                if (lines.at(2) == hashing.at(20)) {
                    configs[2] = 1;
                }
                if (lines.at(2) == hashing.at(23)) {
                    configs[2] = 2;
                }

                // Galdera Options
                if (lines.at(3) == hashing.at(21)) {
                    configs[3] = 0;
                }
                if (lines.at(3) == hashing.at(6)) {
                    configs[3] = 1;
                }

                // PC win condition 
                if (lines.at(4) == hashing.at(3)) {
                    configs[4] = 0;
                }
                if (lines.at(4) == hashing.at(0)) {
                    configs[4] = 1;
                }

                // Full Random
                configs[5] = _wtoi(std::wstring(1, lines.at(5)).c_str());

                // Solo Random
                configs[6] = _wtoi(std::wstring(1, lines.at(6)).c_str());

                // Force Bosses 
                configs[7] = _wtoi(std::wstring(1, lines.at(7)).c_str());

                // Include Duplicates
                if (lines.at(8) == hashing.at(12)) {
                    configs[8] = 0;
                }
                if (lines.at(8) == hashing.at(4)) {
                    configs[8] = 1;
                }

                // Forcing PC option
                if (lines.at(9) == hashing.at(8)) {
                    configs[9] = 0;
                }
                if (lines.at(9) == hashing.at(13)) {
                    configs[9] = 1;
                }
                if (lines.at(9) == hashing.at(19)) {
                    configs[9] = 2;
                }

                // Force PC character
                configs[10] = _wtoi(std::wstring(1, lines.at(10)).c_str());

                // Force Boss Options
                configs[11] = _wtoi(std::wstring(1, lines.at(11)).c_str());
                configs[12] = _wtoi(std::wstring(1, lines.at(12)).c_str());
                configs[13] = _wtoi(std::wstring(1, lines.at(13)).c_str());
                configs[14] = _wtoi(std::wstring(1, lines.at(14)).c_str());
                configs[15] = _wtoi(std::wstring(1, lines.at(15)).c_str());
                configs[16] = _wtoi(std::wstring(1, lines.at(16)).c_str());
                configs[17] = _wtoi(std::wstring(1, lines.at(17)).c_str());

                // Pak dir
                std::wstring pakDir = lines.substr(quotes[0], quotes[1]);
                std::wstring trimmedpakDir = pakDir.substr(0, pakDir.length() - 1);
                *pakPathPointer = trimmedpakDir;

            }
        }
    }
    else {
        // If config not found, use default values
        configs[0] = 0;
        configs[1] = 1;
        configs[2] = 1;
        configs[3] = 0;
        configs[4] = 0;
        configs[5] = 0;
        configs[6] = 0;
        configs[7] = 0;
        configs[8] = 0;
        configs[9] = 1;
        configs[10] = 0;
        configs[11] = 0;
        configs[12] = 0;
        configs[13] = 0;
        configs[14] = 0;
        configs[15] = 0;
        configs[16] = 0;
        configs[17] = 0;
        *pakPathPointer = L"C:\\Program Files (x86)\\Steam\\Steamapps\\Common\\OCTOPATH TRAVELER\\Octopath_Traveler\\Content\\Paks";
    }

    configFile.close();
    return configs;

}

void configWriter(std::string input, intvector configs, wchar_t* pakPath) {

    // Write to config file using the same psuedohashing 
    std::wstring lines;
    std::wofstream configFile(input.c_str());
    // Write a first line for explaining
    configFile << "// Please do not modify this file" << std::endl;

    // The first portion of the line is the Alphabetic hash of the options
    std::wstring hashing = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    // Mixing Options
    switch (configs[0]) {
    case 0:
        configFile << hashing.at(17);
        break;
    case 1:
        configFile << hashing.at(15);
        break;
    case 2:
        configFile << hashing.at(14);
        break;
    }
    // Randomize Shrine Options
    switch (configs[1]) {
    case 0:
        configFile << hashing.at(1);
        break;
    case 1:
        configFile << hashing.at(7);
        break;
    case 2:
        configFile << hashing.at(4);
        break;
    }

    // Randomize Gate Options
    switch (configs[2]) {
    case 0:
        configFile << hashing.at(7);
        break;
    case 1:
        configFile << hashing.at(20);
        break;
    case 2:
        configFile << hashing.at(23);
        break;
    }

    // Galdera Options
    switch (configs[3]) {
    case 0:
        configFile << hashing.at(21);
        break;
    case 1:
        configFile << hashing.at(6);
        break;
    }

    // PC win condition 
    switch (configs[4]) {
    case 0:
        configFile << hashing.at(3);
        break;
    case 1:
        configFile << hashing.at(0);
        break;
    }

    // Full Random
    configFile << configs[5];

    // Solo Random
    configFile << configs[6];

    // Force Bosses 
    configFile << configs[7];

    // Include Duplicates
    switch (configs[8]) {
    case 0:
        configFile << hashing.at(12);
        break;
    case 1:
        configFile << hashing.at(4);
        break;
    }

    // Forcing PC option
    switch (configs[9]) {
    case 0:
        configFile << hashing.at(8);
        break;
    case 1:
        configFile << hashing.at(13);
        break;
    case 2:
        configFile << hashing.at(19);
        break;
    }
    // Force Boss Options
    configFile << configs[10] << configs[11] << configs[12] << configs[13] << configs[14] << configs[15] << configs[16] << configs[17];

    // Pak dir
    configFile << "\"" << pakPath << "\"" << std::endl;;

    configFile.close();
}