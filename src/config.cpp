#include "Octopath.h"
#include <windows.h>
// use Unicode
#ifndef UNICODE
#define UNICODE
#endif


std::tuple <std::vector<bool>, std::vector<int>> configParser(std::string input, std::wstring* pakPathPointer) {
    // Enum for config parameters
    enum class parameters {
        mixChapter24Bosses,
        mixChapter14Bosses,
        randomizeShrineBosses,
        includeShrineBosses,
        randomizeGateBosses,
        includeGateBosses,
        includeGaldera,
        enableDuplicate,
        trueRandom,
        pakPath,
        forceBosses,
        forceBoss1,
        forceBoss2,
        forceBoss3,
        forceBoss4,
        forceBoss5,
        forceBoss6,
        forceGaldera
    };

    // Map for strings to enum values for switch statement
    std::map<std::wstring, parameters> mapStringToParameters = {
        {L"mixChapter24Bosses", parameters::mixChapter24Bosses},
        {L"mixChapter14Bosses", parameters::mixChapter14Bosses},
        {L"randomizeShrineBosses", parameters::randomizeShrineBosses},
        {L"includeShrineBosses", parameters::includeShrineBosses},
        {L"randomizeGateBosses", parameters::randomizeGateBosses},
        {L"includeGateBosses", parameters::includeGateBosses},
        {L"includeGaldera", parameters::includeGaldera},
        {L"enableDuplicate", parameters::enableDuplicate},
        {L"trueRandom", parameters::trueRandom},
        {L"pakPath", parameters::pakPath},
        {L"forceBossesOption", parameters::forceBosses},
        {L"forceBoss1", parameters::forceBoss1},
        {L"forceBoss2", parameters::forceBoss2},
        {L"forceBoss3", parameters::forceBoss3},
        {L"forceBoss4", parameters::forceBoss4},
        {L"forceBoss5", parameters::forceBoss5},
        {L"forceBoss6", parameters::forceBoss6},
        {L"forceGaldera", parameters::forceGaldera}
    };

    bool mixChapter24Bosses = false; // Randomize with no regard to the difference between the chapter 2-4 bosses. Should be simple enough to conquor.
    bool mixChapter14Bosses = false; // Many of the Chapter 2-4 bosses have moves before you can move. Turn on with caution.
    bool randomizeShrineBosses = false; // Randomize the shrine bosses. the job given is still the one on the pedastal.
    bool includeShrineBosses = false; // Include the shrine bosses in the regular rotation. Unless mix chapters are set, shrine bosses are exclusive to chapter 4.
    bool randomizeGateBosses = false; // Randomizes the gate of finis bosses among themselves
    bool includeGateBosses = false; // Gate bosses are harder, most sturdy versions of the chapter 4 bosses. Turn on with caution.
    bool includeGaldera = false; // Warning, Galdera is very difficult at even the point in which chapter 4 bosses are fought. Enable with EXTREME caution.
    bool enableDuplicate = false; // Full randomize. Randomizes the bosses with no regard to one per story (You can fight the same boss multiple times in multiple chapters)
    bool trueRandom = false; // True Random. Enables enableDuplicate and includeGaldera and randomizes with no regard to your sanity. Be prepared for a fight and multiple Galderas
    bool forceBoss = false; // Option for forcing same tier bosses
    std::string pakPath; // Pak path for GUI
    // for force bosses
    std::vector<int> forceBosses(6);
    bool forceGaldera = false;

    // Config File Parser
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
                std::wstring variableName = lines.substr(quotes[0], quotes[1]);
                std::wstring variableValue = lines.substr(quotes[2], quotes[3]);
                switch (mapStringToParameters[variableName]) {
                case parameters::mixChapter24Bosses:
                    if (variableValue.find(L"true") != std::string::npos) {
                        mixChapter24Bosses = true;
                    }
                    else {
                        mixChapter24Bosses = false;
                    }
                    break;
                case parameters::mixChapter14Bosses:
                    if (variableValue.find(L"true") != std::string::npos) {
                        mixChapter14Bosses = true;
                    }
                    else {
                        mixChapter14Bosses = false;
                    }
                    break;
                case parameters::randomizeShrineBosses:
                    if (variableValue.find(L"true") != std::string::npos) {
                        randomizeShrineBosses = true;
                    }
                    else {
                        randomizeShrineBosses = false;
                    }
                    break;
                case parameters::includeShrineBosses:
                    if (variableValue.find(L"true") != std::string::npos) {
                        includeShrineBosses = true;
                    }
                    else {
                        includeShrineBosses = false;
                    }
                    break;
                case parameters::randomizeGateBosses:
                    if (variableValue.find(L"true") != std::string::npos) {
                        randomizeGateBosses = true;
                    }
                    else {
                        randomizeGateBosses = false;
                    }
                    break;
                case parameters::includeGateBosses:
                    if (variableValue.find(L"true") != std::string::npos) {
                        includeGateBosses = true;
                    }
                    else {
                        includeGateBosses = false;
                    }
                    break;
                case parameters::includeGaldera:
                    if (variableValue.find(L"true") != std::string::npos) {
                        includeGaldera = true;
                    }
                    else {
                        includeGaldera = false;
                    }
                    break;
                case parameters::enableDuplicate:
                    if (variableValue.find(L"true") != std::string::npos) {
                        enableDuplicate = true;
                    }
                    else {
                        enableDuplicate = false;
                    }
                    break;
                case parameters::trueRandom:
                    if (variableValue.find(L"true") != std::string::npos) {
                        trueRandom = true;
                    }
                    else {
                        trueRandom = false;
                    }
                    break;
                case parameters::pakPath:
                {
                    std::wstring trimmedVariableValue = variableValue.substr(0, variableValue.length() - 1);
                    *pakPathPointer = trimmedVariableValue;
                }
                break;
                case parameters::forceBosses:
                    if (variableValue.find(L"true") != std::string::npos) {
                        forceBoss = true;
                    }
                    else {
                        forceBoss = false;
                    }
                    break;
                case parameters::forceBoss1:
                {
                    std::wstring trimmedVariableValue = variableValue.substr(0, variableValue.length() - 1);
                    forceBosses[0] = std::stoi(trimmedVariableValue);
                }
                break;
                case parameters::forceBoss2:
                {
                    std::wstring trimmedVariableValue = variableValue.substr(0, variableValue.length() - 1);
                    forceBosses[1] = std::stoi(trimmedVariableValue);
                }
                break;
                case parameters::forceBoss3:
                {
                    std::wstring trimmedVariableValue = variableValue.substr(0, variableValue.length() - 1);
                    forceBosses[2] = std::stoi(trimmedVariableValue);
                }
                break;
                case parameters::forceBoss4:
                {
                    std::wstring trimmedVariableValue = variableValue.substr(0, variableValue.length() - 1);
                    forceBosses[3] = std::stoi(trimmedVariableValue);
                }
                break;
                case parameters::forceBoss5:
                {
                    std::wstring trimmedVariableValue = variableValue.substr(0, variableValue.length() - 1);
                    forceBosses[4] = std::stoi(trimmedVariableValue);
                }
                break;
                case parameters::forceBoss6:
                {
                    std::wstring trimmedVariableValue = variableValue.substr(0, variableValue.length() - 1);
                    forceBosses[5] = std::stoi(trimmedVariableValue);
                }
                break;
                case parameters::forceGaldera:
                    if (variableValue.find(L"true") != std::string::npos) {
                        forceGaldera = true;
                    }
                    else {
                        forceGaldera = false;
                    }
                    break;
                default:
                    std::cout << "Unknown config line option." << std::endl;
                    break;
                }

            }
        }
    }
    else {
        // If config not found, use default values
        mixChapter24Bosses = false;
        mixChapter14Bosses = false;
        randomizeShrineBosses = true;
        includeShrineBosses = false;
        randomizeGateBosses = true;
        includeGateBosses = false;
        includeGaldera = false;
        enableDuplicate = false;
        trueRandom = false;
        forceBoss = false;
        forceGaldera = false;
        *pakPathPointer = L"C:\\Program Files (x86)\\Steam\\Steamapps\\Common\\OCTOPATH TRAVELER\\Octopath_Traveler\\Content\\Paks";
        for (int i = 0; i < 6; i++) {
            forceBosses[i] = 0;
        }

    }
    
    configFile.close();
    std::vector<bool> outputBool = { mixChapter24Bosses, mixChapter14Bosses, randomizeShrineBosses, includeShrineBosses, randomizeGateBosses, includeGateBosses, includeGaldera, enableDuplicate, trueRandom, forceBoss, forceGaldera };
    return std::make_tuple(outputBool, forceBosses);
}