#include "Octopath.h"

using namespace std;

// Randomizer for none mixed include options

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

// function for checking if the variable should be excluded
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

vectorvector includeRandomize(mt19937 rng, bool randomizeShrineBosses, bool includeShrineBosses, bool randomizeGateBosses, bool includeGateBosses, bool includeGaldera, bool includeDuplicate) {
	// First three chpater bosses are the same as base, only the chapter 4 potentially changes

	// note: vector starts from 0, so chapter 1 is 0, chapter 2 is 1, etc...
	vectorvector chapterbosses(7);

	// Check for duplicates config option
	if (includeDuplicate == false) {
		// No duplicates allowed
		// Chapters 1 - 3 are randomized, but not included
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 8; j++) {
				int ranboss;
				bool match;
				do {
					match = false;
					uniform_int_distribution <mt19937::result_type> bosses(i * 8, i * 8 + 7);
					ranboss = bosses(rng);
					if (!chapterbosses[i].empty() && !std::none_of(chapterbosses[i].begin(), chapterbosses[i].end(), compare(ranboss))) {
						match = true;
					}
				} while (match == true);
				chapterbosses[i].push_back(ranboss);
			}
		}

		// Chapter 4 bosses
		for (int i = 0; i < 9; i++) {
			int ranboss;
			bool match;
			bool exclude;
			do {
				// Exclude checking
				do {
					exclude = false;
					uniform_int_distribution <mt19937::result_type> bosses(24, 46);
					ranboss = bosses(rng);
					exclude = excludeChecker(ranboss, includeShrineBosses, includeGateBosses, includeGaldera);
				} while (exclude == true);
				// check if returned value is not in matrix
				match = matrixChecker(chapterbosses, ranboss, 3);
			} while (match == true);
			chapterbosses[3].push_back(ranboss);
		}

		// Shrine Bosses
		for (int i = 0; i < 4; i++) {
			int ranboss;
			bool match;
			if (includeShrineBosses == true) {
				bool exclude;
				do {
					// Exclude checking
					do {
						exclude = false;
						uniform_int_distribution <mt19937::result_type> bosses(24, 46);
						ranboss = bosses(rng);
						exclude = excludeChecker(ranboss, includeShrineBosses, includeGateBosses, includeGaldera);
					} while (exclude == true);
					// check if returned value is not in matrix
					match = matrixChecker(chapterbosses, ranboss, 4);
				} while (match == true);
				chapterbosses[4].push_back(ranboss);
			}
			// Randomize among the shrine bosses
			else if (randomizeShrineBosses == true) {
				do {
					match = false;
					uniform_int_distribution <mt19937::result_type> bosses(33, 36);
					ranboss = bosses(rng);
					if (!chapterbosses[4].empty() && !std::none_of(chapterbosses[4].begin(), chapterbosses[4].end(), compare(ranboss))) {
						match = true;
					}
				} while (match == true);
				chapterbosses[4].push_back(ranboss);
			}
			// No randomization
			else {
				chapterbosses[4].push_back(i + 33);
			}
		}

		// Gate Bosses
		for (int i = 0; i < 8; i++) {
			int ranboss;
			bool match;
			if (includeGateBosses == true) {
				bool exclude;
				do {
					// Exclude checking
					do {
						exclude = false;
						uniform_int_distribution <mt19937::result_type> bosses(24, 46);
						ranboss = bosses(rng);
						exclude = excludeChecker(ranboss, includeShrineBosses, includeGateBosses, includeGaldera);
					} while (exclude == true);
					// check if returned value is not in matrix
					match = matrixChecker(chapterbosses, ranboss, 5);
				} while (match == true);
				chapterbosses[5].push_back(ranboss);
			}
			// Randomize among the shrine bosses
			else if (randomizeGateBosses == true) {
				do {
					match = false;
					uniform_int_distribution <mt19937::result_type> bosses(37, 44);
					ranboss = bosses(rng);
					if (!chapterbosses[5].empty() && !std::none_of(chapterbosses[5].begin(), chapterbosses[5].end(), compare(ranboss))) {
						match = true;
					}
				} while (match == true);
				chapterbosses[5].push_back(ranboss);
			}
			// No randomization
			else {
				chapterbosses[5].push_back(i + 37);
			}
		}

		// Galdera
		for (int i = 0; i < 2; i++) {
			int ranboss;
			bool match;
			if (includeGaldera == true) {
				bool exclude;
				do {
					// Exclude checking
					do {
						exclude = false;
						uniform_int_distribution <mt19937::result_type> bosses(24, 46);
						ranboss = bosses(rng);
						exclude = excludeChecker(ranboss, includeShrineBosses, includeGateBosses, includeGaldera);
					} while (exclude == true);
					// check if returned value is not in matrix
					match = matrixChecker(chapterbosses, ranboss, 6);
				} while (match == true);
				chapterbosses[6].push_back(ranboss);
			}
			// No randomization
			else {
				chapterbosses[6].push_back(i + 45);
			}
		}
	
	}

	// Enable Duplicates
	else {
		// Duplicates allowed
		// Chapters 1 - 3 are randomized, but not included
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 8; j++) {
				int ranboss;
				uniform_int_distribution <mt19937::result_type> bosses(i * 8, i * 8 + 7);
				ranboss = bosses(rng);
			chapterbosses[i].push_back(ranboss);
			}
		}

		// Chapter 4 bosses
		for (int i = 0; i < 9; i++) {
			int ranboss;
			bool exclude;
			// Exclude checking
			do {
				exclude = false;
				uniform_int_distribution <mt19937::result_type> bosses(24, 46);
				ranboss = bosses(rng);
				exclude = excludeChecker(ranboss, includeShrineBosses, includeGateBosses, includeGaldera);
			} while (exclude == true);
			chapterbosses[3].push_back(ranboss);
		}

		// Shrine Bosses
		for (int i = 0; i < 4; i++) {
			int ranboss;
			if (includeShrineBosses == true) {
				bool exclude;
				// Exclude checking
				do {
					exclude = false;
					uniform_int_distribution <mt19937::result_type> bosses(24, 46);
					ranboss = bosses(rng);
					exclude = excludeChecker(ranboss, includeShrineBosses, includeGateBosses, includeGaldera);
				} while (exclude == true);
				chapterbosses[4].push_back(ranboss);
			}
			// Randomize among the shrine bosses
			else if (randomizeShrineBosses == true) {
				uniform_int_distribution <mt19937::result_type> bosses(33, 36);
				ranboss = bosses(rng);
				chapterbosses[4].push_back(ranboss);
			}
			// No randomization
			else {
				chapterbosses[4].push_back(i + 33);
			}
		}

		// Gate Bosses
		for (int i = 0; i < 8; i++) {
			int ranboss;
			if (includeGateBosses == true) {
				bool exclude;
				// Exclude checking
				do {
					exclude = false;
					uniform_int_distribution <mt19937::result_type> bosses(24, 46);
					ranboss = bosses(rng);
					exclude = excludeChecker(ranboss, includeShrineBosses, includeGateBosses, includeGaldera);
				} while (exclude == true);
				chapterbosses[5].push_back(ranboss);
			}
			// Randomize among the shrine bosses
			else if (randomizeGateBosses == true) {
				uniform_int_distribution <mt19937::result_type> bosses(37, 44);
				ranboss = bosses(rng);
				chapterbosses[5].push_back(ranboss);
			}
			// No randomization
			else {
				chapterbosses[5].push_back(i + 37);
			}
		}

		// Galdera
		for (int i = 0; i < 2; i++) {
			int ranboss;
			if (includeGaldera == true) {
				bool exclude;
				// Exclude checking
				do {
					exclude = false;
					uniform_int_distribution <mt19937::result_type> bosses(24, 46);
					ranboss = bosses(rng);
					exclude = excludeChecker(ranboss, includeShrineBosses, includeGateBosses, includeGaldera);
				} while (exclude == true);
				chapterbosses[6].push_back(ranboss);
			}
			// No randomization
			else {
				chapterbosses[6].push_back(i + 45);
			}
		}
	}

	return chapterbosses;
}
