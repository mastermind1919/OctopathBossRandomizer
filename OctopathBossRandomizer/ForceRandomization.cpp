#include "Octopath.h"


using namespace std;

// function for checking if a value is in the matrix, including the fixed vector
bool matrixFixedChecker(vectorvector input, int ranboss, int endRange) {
	bool match = false;
	for (int i = 0; i <= endRange; i++) {
		if (!input[i].empty() && !std::none_of(input[i].begin(), input[i].end(), compare(ranboss))) {
			match = true;
			break;
		}
	}
	return match;
}

// function for checking if the boss should be excluded
// Todo: Check implications on boss curve by doing this
bool excludeFixedChecker(int input, bool includeShrineBosses, bool includeGateBosses, bool includeGaldera) {
	bool exclude = false;
	// exclude 33 - 36 if excluding shrine bosses
	if (includeShrineBosses == false && 33 <= input && input <= 36) {
		exclude = true;
	}
	// exclude 37 - 44 if excluding gate bosses
	if (includeGateBosses == false && 37 <= input && input <= 44 && exclude == false) {
		exclude = true;
	}
	// exclude 45 and 46 if excluding Galdera
	if (includeGaldera == false && 45 <= input && input <= 46 && exclude == false) {
		exclude = true;
	}
	return exclude;
}

int uniformFixedRandom(mt19937 rng, vectorvector chapterBosses, int lowerLimit, int upperLimit, int tier, bool includeShrine, bool includeGate, bool includeGaldera, bool includeDuplicate) {
	int ranboss;
	bool match;
	bool exclude;
	do {
		// Exclude checking
		do {
			exclude = false;
			uniform_int_distribution <mt19937::result_type> bosses(lowerLimit, upperLimit);
			ranboss = bosses(rng);
			exclude = excludeFixedChecker(ranboss, includeShrine, includeGate, includeGaldera);
		} while (exclude == true);
		// if includeDuplicate is false, check if returned boss is not already included
		if (includeDuplicate == false) {
			match = matrixFixedChecker(chapterBosses, ranboss, tier);
		}
		else {
			match = false;
		}
	} while (match == true);
	return ranboss;

}

vectorvector fixedTier(mt19937 rng, intvector config) {
	// input config is 18 long, relevant information is 11 - 17
	intvector internalConfig(7);
	internalConfig[0] = config[11];
	internalConfig[1] = config[12];
	internalConfig[2] = config[13];
	internalConfig[3] = config[14];
	internalConfig[4] = config[15];
	internalConfig[5] = config[16];
	internalConfig[6] = config[17];
	// Set bool for duplicate
	bool includeDuplicate;
	config[8] == 1 ? includeDuplicate = true : includeDuplicate = false;
	// Vector for storing fixed bosses
	vectorvector fixedBosses(7);
	// Chapter 1
	for (int i = 0; i < internalConfig[0]; i++) {
		fixedBosses[0].push_back(uniformFixedRandom(rng, fixedBosses, 0, 7, 0, false, false, false, includeDuplicate));
	}
	// Chapter 2
	for (int i = 0; i < internalConfig[1]; i++) {
		fixedBosses[1].push_back(uniformFixedRandom(rng, fixedBosses, 8, 15, 1, false, false, false, includeDuplicate));
	}
	// Chapter 3
	for (int i = 0; i < internalConfig[2]; i++) {
		fixedBosses[2].push_back(uniformFixedRandom(rng, fixedBosses, 16, 23, 2, false, false, false, includeDuplicate));
	}
	// Chapter 4
	for (int i = 0; i < internalConfig[3]; i++) {
		fixedBosses[3].push_back(uniformFixedRandom(rng, fixedBosses, 24, 32, 3, false, false, false, includeDuplicate));
	}
	// Shrine Bosses
	for (int i = 0; i < internalConfig[4]; i++) {
		fixedBosses[4].push_back(uniformFixedRandom(rng, fixedBosses, 33, 36, 4, true, false, false, includeDuplicate));
	}
	// Gate Bosses
	for (int i = 0; i < internalConfig[5]; i++) {
		fixedBosses[5].push_back(uniformFixedRandom(rng, fixedBosses, 37, 44, 5, false, true, false, includeDuplicate));
	}
	// Galdera
	for (int i = 0; i < internalConfig[6]; i++) {
		fixedBosses[6].push_back(uniformFixedRandom(rng, fixedBosses, 45, 46, 6, false, false, true, includeDuplicate));
	}

	return fixedBosses;
}