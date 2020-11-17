#include "Octopath.h"

using namespace std;

// Function for inserting a vector into another vector randomly, so choosen fixed bosses are in random locations
std::vector<int> insertRandom(std::mt19937 rng, std::vector<int> randomInput, std::vector<int> fixedInput) {
	// Check if fixed bosses is empty, and return input if so
	if (fixedInput.size() == 0) {
		return randomInput;
	}
	else {
		int upperLimit = randomInput.size() + fixedInput.size();
		std::vector pullFromVector = randomInput;
		pullFromVector.insert(std::end(pullFromVector), std::begin(fixedInput), std::end(fixedInput));
		std::vector<int> pushToVector;
		std::vector<int> matchVector;
		for (int i = 0; i < upperLimit; i++) {
			bool match;
			int bossPosition;
			do {
				match = false;
				uniform_int_distribution<mt19937::result_type> bosses(0, upperLimit - 1);
				bossPosition = bosses(rng);
				if (!matchVector.empty() && !std::none_of(matchVector.begin(), matchVector.end(), compare(bossPosition))) {
					match = true;
				}

			} while (match == true);
			pushToVector.push_back(pullFromVector[bossPosition]);
			matchVector.push_back(bossPosition);
		}
		return pushToVector;
	}
}
std::vector<int> insertRandomFixed(std::mt19937 rng, std::vector<int> randomInput, std::vector<int> fixedInput, int forcedCharacter) {
	// Doesn't matter if fixedInput is empty, we need to force a chapter 1 boss anyway

	int upperLimit = randomInput.size() + fixedInput.size();
	std::vector pullFromVector = randomInput;
	pullFromVector.insert(std::end(pullFromVector), std::begin(fixedInput), std::end(fixedInput));
	std::vector<int> pushToVector;
	std::vector<int> matchVector;
	// First, check if any chapter 1 bosses exist in the vector
	bool chapter1Checker = false;
	for (int i = 0; i < pullFromVector.size(); i++) {
		for (int j = 0; j < 8; j++) {
			if (!std::none_of(pullFromVector.begin(), pullFromVector.end(), compare(j))) {
				chapter1Checker = true;
				break;
			}
		}
	}
	// Now that we know if a chapter 1 boss is in the vector, randomize all but the PC boss
	if (chapter1Checker == true) {
		// start with the PC's boss, and ensure it is a chapter 1 boss
		int fixedBossPosition;
		{
			bool chapter1match;
			int bossPosition;
			do {
				chapter1match = false;
				uniform_int_distribution<mt19937::result_type> bosses(0, upperLimit - 1);
				bossPosition = bosses(rng);
				for (int j = 0; j < 8; j++) {
					if (!std::none_of(pullFromVector.begin(), pullFromVector.end(), compare(j))) {
						chapter1match = true;
						break;
					}
				}
			} while (chapter1match == false);
			fixedBossPosition = bossPosition;
		}
		for (int i = 1; i < forcedCharacter; i++) {
			bool match;
			int bossPosition;
			do {
				match = false;
				uniform_int_distribution<mt19937::result_type> bosses(0, upperLimit - 1);
				bossPosition = bosses(rng);
				// include check for fixedBossPosition
				if (bossPosition == fixedBossPosition || !matchVector.empty() && !std::none_of(matchVector.begin(), matchVector.end(), compare(bossPosition))) {
					match = true;
				}
			} while (match == true);
			pushToVector.push_back(pullFromVector[bossPosition]);
			matchVector.push_back(bossPosition);
		}
		// Add PC boss to vector in correct position
		pushToVector.push_back(pullFromVector[fixedBossPosition]);
		matchVector.push_back(fixedBossPosition);
		for (int i = (forcedCharacter + 1); i < (upperLimit + 1); i++) {
			bool match;
			int bossPosition;
			do {
				match = false;
				uniform_int_distribution<mt19937::result_type> bosses(0, upperLimit - 1);
				bossPosition = bosses(rng);
				if (!matchVector.empty() && !std::none_of(matchVector.begin(), matchVector.end(), compare(bossPosition))) {
					match = true;
				}
			} while (match == true);
			pushToVector.push_back(pullFromVector[bossPosition]);
			matchVector.push_back(bossPosition);
		}
	}
	else {
		// randomize the bosses, but change the PC boss to a chapter 1 boss
		for (int i = 0; i < upperLimit; i++) {
			bool match;
			int bossPosition;
			do {
				match = false;
				uniform_int_distribution<mt19937::result_type> bosses(0, upperLimit - 1);
				bossPosition = bosses(rng);
				if (!matchVector.empty() && !std::none_of(matchVector.begin(), matchVector.end(), compare(bossPosition))) {
					match = true;
				}

			} while (match == true);
			pushToVector.push_back(pullFromVector[bossPosition]);
			matchVector.push_back(bossPosition);
		}
		// Force the chapter 1 boss, since we know there are no unique chapter 1 bosses in the pool
		int ranboss;
		uniform_int_distribution<mt19937::result_type> bosses(0, 7);
		ranboss = bosses(rng);
		pushToVector[forcedCharacter] = ranboss;
	}
	return pushToVector;
}

// function for checking if a value is in the vectorvector
bool matrixChecker(vectorvector input, int ranboss, int endRange) {
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
bool excludeChecker(int input, bool includeShrineBosses, bool includeGateBosses, bool includeGaldera) {
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

// Function for randomizing the bosses
int uniformBossRandom(mt19937 rng, vectorvector chapterBosses, int lowerLimit, int upperLimit, int tier, bool includeShrine, bool includeGate, bool includeGaldera, bool includeDuplicate) {
	int ranboss;
	bool match;
	bool exclude;
	do {
		// Exclude checking
		do {
			exclude = false;
			uniform_int_distribution <mt19937::result_type> bosses(lowerLimit, upperLimit);
			ranboss = bosses(rng);
			exclude = excludeChecker(ranboss, includeShrine, includeGate, includeGaldera);
		} while (exclude == true);
		// if includeDuplicate is false, check if returned boss is not already included
		if (includeDuplicate == false) {
			match = matrixChecker(chapterBosses, ranboss, tier);
		}
		else {
			match = false;
		}
	} while (match == true);
	return ranboss;

}


vectorvector randomizeBosses(mt19937 rng, vectorvector fixedBosses, int forcedCharacter, bool mixChapter14, bool mixChapter24, bool randomizeShrine, bool includeShrine, bool randomizeGate, bool includeGate, bool includeGaldera, bool includeDuplicate, bool fullRandom) {
	// for forced chapter 1 character, this is the list
	/* 1 - Tressa
	*  2 - Alfyn
	*  3 - Therion
	*  4 - H'annit
	*  5 - Olberic
	*  6 - Primrose
	*  7 - Cyrus
	*  8 - Ophilia
	*  0 is no forcing for PC
	*/
	vectorvector chapterBosses(7);
	// Chapter 1 is special, do it seperately
	if (mixChapter14 == true || fullRandom == true) {
		// Randomize as normal
		for (int j = 0; j < (8 - fixedBosses[0].size()); j++) {
			chapterBosses[0].push_back(uniformBossRandom(rng, chapterBosses, 0, 46, 0, includeShrine, includeGate, includeGaldera, includeDuplicate));
		}
		if (forcedCharacter == 0) {
			// just randomize the inserts
			chapterBosses[0] = insertRandom(rng, chapterBosses[0], fixedBosses[0]);
		}
		else {
			// Use special function for PC boss
			chapterBosses[0] = insertRandomFixed(rng, chapterBosses[0], fixedBosses[0], forcedCharacter);
		}
	}
	else {
		// Without those options, just randomize the chapter 1 bosses by themselves, no forcing is needed
		for (int i = 0; i < 8; i++) {
			chapterBosses[0].push_back(uniformBossRandom(rng, chapterBosses, 0, 7, 0, false, false, false, includeDuplicate));
		}
	}
	// With Chapter 1 bosses randomized, do chapters 2 and 3
	if (mixChapter24 == true || fullRandom == true) {
		// Do chapter 2 - 3 in a loop
		for (int i = 1; i < 3; i++) {
			if (fullRandom == true) {
				for (int j = 0; j < 8; j++) {
					chapterBosses[i].push_back(uniformBossRandom(rng, chapterBosses, 0, 46, i, true, true, true, true));
				}
			}
			else {
				for (int j = 0; j < 8; j++) {
					chapterBosses[i].push_back(uniformBossRandom(rng, chapterBosses, 0, 46, i, includeShrine, includeGate, includeGaldera, includeDuplicate));
				}
			}
		}
	}
	else {
		// Do chapters 2 - 3 seperately
		// Chapter 2
		for (int i = 0; i < 8; i++) {
			chapterBosses[1].push_back(uniformBossRandom(rng, chapterBosses, 8, 15, 1, false, false, false, includeDuplicate));
		}
		//Chapter 3
		for (int i = 0; i < 8; i++) {
			chapterBosses[2].push_back(uniformBossRandom(rng, chapterBosses, 16, 23, 2, false, false, false, includeDuplicate));
		}
	}
	// Chapter 4
	if (fullRandom == true){
		for (int i = 0; i < 9; i++) {
			chapterBosses[3].push_back(uniformBossRandom(rng, chapterBosses, 0, 46, 3, true, true, true, true));
		}
	}
	else {
		for (int i = 0; i < 9; i++) {
			chapterBosses[3].push_back(uniformBossRandom(rng, chapterBosses, 0, 46, 3, includeShrine, includeGate, includeGaldera, includeDuplicate));
		}
	}
	// Shrine bosses
	if (fullRandom == true) {
		chapterBosses[4].push_back(uniformBossRandom(rng, chapterBosses, 0, 46, 4, true, true, true, true));
	}
	else if (includeShrine == true) {
		chapterBosses[4].push_back(uniformBossRandom(rng, chapterBosses, 0, 46, 4, includeShrine, includeGate, includeGaldera, includeDuplicate));
	}
	else if (randomizeShrine == true) {
		chapterBosses[4].push_back(uniformBossRandom(rng, chapterBosses, 33, 36, 4, true, true, true, includeDuplicate));
	}
	else {
		for (int i = 33; i < 37; i++) {
			chapterBosses[4].push_back(i);
		}
	}
	// Gate Bosses
	if (fullRandom == true) {
		chapterBosses[5].push_back(uniformBossRandom(rng, chapterBosses, 0, 46, 5, true, true, true, true));
	}
	else if (includeGate == true) {
		chapterBosses[5].push_back(uniformBossRandom(rng, chapterBosses, 0, 46, 5, includeShrine, includeGate, includeGaldera, includeDuplicate));
	}
	else if (randomizeGate == true) {
		chapterBosses[5].push_back(uniformBossRandom(rng, chapterBosses, 37, 44, 5, true, true, true, includeDuplicate));
	}
	else {
		for (int i = 37; i < 45; i++) {
			chapterBosses[5].push_back(i);
		}
	}
	// Galdera
	if (fullRandom == true) {
		chapterBosses[6].push_back(uniformBossRandom(rng, chapterBosses, 0, 46, 6, true, true, true, true));
	}
	else if (includeGaldera == true) {
		chapterBosses[6].push_back(uniformBossRandom(rng, chapterBosses, 0, 46, 6, includeShrine, includeGate, includeGaldera, includeDuplicate));
	}
	else {
		for (int i = 45; i < 47; i++) {
			chapterBosses[6].push_back(i);
		}
	}
	return chapterBosses;
}