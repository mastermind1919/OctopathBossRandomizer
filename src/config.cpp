#include "Octopath.h"
#include <windows.h>
// use Unicode
#ifndef UNICODE
#define UNICODE
#endif

// Convert string to wstring
//std::wstring s2ws(const std::string& s) {
//    int len;
//    int slength = (int)s.length() + 1;
//    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
//    wchar_t* buf = new wchar_t[len];
//    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
//    std::wstring r(buf);
//    delete[] buf;
//    return r;
//}


std::vector<bool> configParser(std::string input, std::wstring* pakPathPointer) {
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
        pakPath
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
        {L"pakPath", parameters::pakPath}
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
    std::string pakPath; // Pak path for GUI


    // Config File Parser
    std::wstring lines;
    std::wifstream configFile(input.c_str());
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
            default:
                std::cout << "Unknown config line option." << std::endl;
                break;
            }
        }
    }
    configFile.close();
    std::vector<bool> output = { mixChapter24Bosses, mixChapter14Bosses, randomizeShrineBosses, includeShrineBosses, randomizeGateBosses, includeGateBosses, includeGaldera, enableDuplicate, trueRandom};
    return output;
}