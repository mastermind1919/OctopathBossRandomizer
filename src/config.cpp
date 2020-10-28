#include "Octopath.h"

vector<bool> configParser(string input) {
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
        trueRandom
    };

    // Map for strings to enum values for switch statement
    map<string, parameters> mapStringToParameters = {
        {"mixChapter24Bosses", parameters::mixChapter24Bosses},
        {"mixChapter14Bosses", parameters::mixChapter14Bosses},
        {"randomizeShrineBosses", parameters::randomizeShrineBosses},
        {"includeShrineBosses", parameters::includeShrineBosses},
        {"randomizeGateBosses", parameters::randomizeGateBosses},
        {"includeGateBosses", parameters::includeGateBosses},
        {"includeGaldera", parameters::includeGaldera},
        {"enableDuplicate", parameters::enableDuplicate},
        {"trueRandom", parameters::trueRandom}
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


    // Config File Parser
    string lines;
    ifstream configFile(input.c_str());
    for (string line; getline(configFile, lines); ) {
        // ignore lines containing "//" and empty lines
        if (lines.find_first_of("//") == string::npos && lines != "") {
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
            string variableName = lines.substr(quotes[0], quotes[1]);
            string variableValue = lines.substr(quotes[2], quotes[3]);
            switch (mapStringToParameters[variableName]) {
            case parameters::mixChapter24Bosses:
                if (variableValue.find("true") != string::npos) {
                    mixChapter24Bosses = true;
                }
                else {
                    mixChapter24Bosses = false;
                }
                break;
            case parameters::mixChapter14Bosses:
                if (variableValue.find("true") != string::npos) {
                    mixChapter14Bosses = true;
                }
                else {
                    mixChapter14Bosses = false;
                }
                break;
            case parameters::randomizeShrineBosses:
                if (variableValue.find("true") != string::npos) {
                    randomizeShrineBosses = true;
                }
                else {
                    randomizeShrineBosses = false;
                }
                break;
            case parameters::includeShrineBosses:
                if (variableValue.find("true") != string::npos) {
                    includeShrineBosses = true;
                }
                else {
                    includeShrineBosses = false;
                }
                break;
            case parameters::randomizeGateBosses:
                if (variableValue.find("true") != string::npos) {
                    randomizeGateBosses = true;
                }
                else {
                    randomizeGateBosses = false;
                }
                break;
            case parameters::includeGateBosses:
                if (variableValue.find("true") != string::npos) {
                    includeGateBosses = true;
                }
                else {
                    includeGateBosses = false;
                }
                break;
            case parameters::includeGaldera:
                if (variableValue.find("true") != string::npos) {
                    includeGaldera = true;
                }
                else {
                    includeGaldera = false;
                }
                break;
            case parameters::enableDuplicate:
                if (variableValue.find("true") != string::npos) {
                    enableDuplicate = true;
                }
                else {
                    enableDuplicate = false;
                }
                break;
            case parameters::trueRandom:
                if (variableValue.find("true") != string::npos) {
                    trueRandom = true;
                }
                else {
                    trueRandom = false;
                }
                break;
            default:
                cout << "Unknown config line option." << endl;
                break;
            }
        }
    }
    configFile.close();
    vector<bool> output = { mixChapter24Bosses, mixChapter14Bosses, randomizeShrineBosses, includeShrineBosses, randomizeGateBosses, includeGateBosses, includeGaldera, enableDuplicate, trueRandom };
    return output;
}