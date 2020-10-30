#include "Octopath.h"

using namespace std;
// mixed chapter 1-4 and chapter 2-4 bosses
// also takes into account the duplicates

// Uses the functions excludeChecker and matrixChecker from includeRandomize

vectorvector mixRandomize(std::mt19937 rng, bool mixChapter24, bool mixChapter14, bool randomizeShrineBosses, bool includeShrineBosses, bool randomizeGateBosses, bool includeGateBosses, bool includeGaldera, bool includeDuplicate) {
	// All potions of the vector can change now

	// note: vector starts from 0, so chapter 1 is 0, chapter 2 is 1, etc...
	vectorvector chapterbosses(7);

	// First check for duplicate option
	if (includeDuplicate == false) {
		// Check for mixing of chapter 1 - 4
		if (mixChapter14 == true) {
			// Have no bars on randomizing chapter 1 bosses
			// Do the chapter 1 - 3 bosses in one loop
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 8; j++) {
					int ranboss;
					bool match;
					bool exclude;
					do {
						// Exclude checking
						do {
							exclude = false;
							uniform_int_distribution <mt19937::result_type> bosses(0, 46);
							ranboss = bosses(rng);
							exclude = excludeChecker(ranboss, includeShrineBosses, includeGateBosses, includeGaldera);
						} while (exclude == true);
						// check if returned value is not in matrix
						match = matrixChecker(chapterbosses, ranboss, i);
					} while (match == true);
					chapterbosses[i].push_back(ranboss);
				}
			}
		}
		else {
			// Keep chapter 1 randomized with similar tier
			// Chapter 1
			for (int i = 0; i < 8; i++) {
				int ranboss;
				bool match;
				do {
					match = false;
					uniform_int_distribution <mt19937::result_type> bosses(0, 7);
					ranboss = bosses(rng);
					if (!chapterbosses[0].empty() && !std::none_of(chapterbosses[0].begin(), chapterbosses[0].end(), compare(ranboss))) {
						match = true;
					}
				} while (match == true);
				chapterbosses[0].push_back(ranboss);
			}
			// Chapters 2 - 3
			for (int i = 1; i < 3; i++) {
				for (int j = 0; j < 8; j++) {
					int ranboss;
					bool match;
					bool exclude;
					do {
						// Exclude checking
						do {
							exclude = false;
							uniform_int_distribution <mt19937::result_type> bosses(0, 46);
							ranboss = bosses(rng);
							exclude = excludeChecker(ranboss, includeShrineBosses, includeGateBosses, includeGaldera);
						} while (exclude == true);
						// check if returned value is not in matrix
						match = matrixChecker(chapterbosses, ranboss, i);
					} while (match == true);
					chapterbosses[i].push_back(ranboss);
				}
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
					uniform_int_distribution <mt19937::result_type> bosses(0, 46);
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
						uniform_int_distribution <mt19937::result_type> bosses(0, 46);
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
						uniform_int_distribution <mt19937::result_type> bosses(0, 46);
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
						uniform_int_distribution <mt19937::result_type> bosses(0, 46);
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
	// Duplicates Allowed
	else {
		// Check for mixing of chapter 1 - 4
		if (mixChapter14 == true) {
			// Have no bars on randomizing chapter 1 bosses
			// Do the chapter 1 - 3 bosses in one loop
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 8; j++) {
					int ranboss;
					bool exclude;
					// Exclude checking
					do {
						exclude = false;
						uniform_int_distribution <mt19937::result_type> bosses(0, 46);
						ranboss = bosses(rng);
						exclude = excludeChecker(ranboss, includeShrineBosses, includeGateBosses, includeGaldera);
					} while (exclude == true);
					chapterbosses[i].push_back(ranboss);
				}
			}
			// Chapter 4 bosses
			for (int i = 0; i < 9; i++) {
				int ranboss;
				bool exclude;
				do {
					exclude = false;
					uniform_int_distribution <mt19937::result_type> bosses(0, 46);
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
						uniform_int_distribution <mt19937::result_type> bosses(0, 46);
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
						uniform_int_distribution <mt19937::result_type> bosses(0, 46);
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
						uniform_int_distribution <mt19937::result_type> bosses(0, 46);
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
		else {
			// Keep chapter 1 randomized with similar tier
			// Chapter 1
			for (int i = 0; i < 8; i++) {
				int ranboss;
				uniform_int_distribution <mt19937::result_type> bosses(0, 7);
				ranboss = bosses(rng);
				chapterbosses[0].push_back(ranboss);
			}
			// Chapters 2 - 3
			for (int i = 1; i < 3; i++) {
				for (int j = 0; j < 8; j++) {
					int ranboss;
					bool exclude;
						do {
							exclude = false;
							uniform_int_distribution <mt19937::result_type> bosses(8, 46);
							ranboss = bosses(rng);
							exclude = excludeChecker(ranboss, includeShrineBosses, includeGateBosses, includeGaldera);
						} while (exclude == true);
					chapterbosses[i].push_back(ranboss);
				}
			}
			// Chapter 4 bosses
			for (int i = 0; i < 9; i++) {
				int ranboss;
				bool exclude;
				do {
					exclude = false;
					uniform_int_distribution <mt19937::result_type> bosses(8, 46);
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
						uniform_int_distribution <mt19937::result_type> bosses(8, 46);
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
						uniform_int_distribution <mt19937::result_type> bosses(8, 46);
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
						uniform_int_distribution <mt19937::result_type> bosses(8, 46);
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
	}
	return chapterbosses;
}