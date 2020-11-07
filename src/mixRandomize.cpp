#include "Octopath.h"

using namespace std;
// mixed chapter 1-4 and chapter 2-4 bosses
// also takes into account the duplicates

// Uses the functions excludeChecker and matrixChecker from includeRandomize

vectorvector mixRandomize(std::mt19937 rng, vectorvector fixedVector, bool mixChapter24, bool mixChapter14, bool randomizeShrineBosses, bool includeShrineBosses, bool randomizeGateBosses, bool includeGateBosses, bool includeGaldera, bool includeDuplicate) {
	// All potions of the vector can change now

	// note: vector starts from 0, so chapter 1 is 0, chapter 2 is 1, etc...
	vectorvector chapterbosses(7);

	// First check for duplicate option
	if (includeDuplicate == false) {
		// Check for mixing of chapter 1 - 4
		if (mixChapter14 == true) {
			// Have no bars on randomizing chapter 1 bosses
			// Chapter 1
			for (int j = 0; j < (8 - fixedVector[0].size()); j++) {
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
					match = matrixChecker(chapterbosses, ranboss, 0);
				} while (match == true);
				chapterbosses[0].push_back(ranboss);
			}
			chapterbosses[0] = insertRandom(rng, chapterbosses[0], fixedVector[0]);
			// Chapter 2
			for (int j = 0; j < (8 - fixedVector[1].size()); j++) {
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
					match = matrixChecker(chapterbosses, ranboss, 1);
				} while (match == true);
				chapterbosses[1].push_back(ranboss);
			}
			chapterbosses[1] = insertRandom(rng, chapterbosses[1], fixedVector[1]);
			// Chapter 3
			for (int j = 0; j < (8 - fixedVector[2].size()); j++) {
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
					match = matrixChecker(chapterbosses, ranboss, 2);
				} while (match == true);
				chapterbosses[2].push_back(ranboss);
			}
			chapterbosses[2] = insertRandom(rng, chapterbosses[2], fixedVector[2]);
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
			// Chapters 2
			for (int j = 0; j < (8 - fixedVector[1].size()); j++) {
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
					match = matrixChecker(chapterbosses, ranboss, 1);
				} while (match == true);
				chapterbosses[1].push_back(ranboss);
			}
			chapterbosses[1] = insertRandom(rng, chapterbosses[1], fixedVector[1]);

			// Chapter 3
			for (int j = 0; j < (8 - fixedVector[2].size()); j++) {
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
					match = matrixChecker(chapterbosses, ranboss, 2);
				} while (match == true);
				chapterbosses[2].push_back(ranboss);
			}
			chapterbosses[2] = insertRandom(rng, chapterbosses[2], fixedVector[2]);
		}

		// Chapter 4 bosses
		for (int i = 0; i < (9 - fixedVector[3].size()); i++) {
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
		chapterbosses[3] = insertRandom(rng, chapterbosses[3], fixedVector[3]);

		// Shrine Bosses
		for (int i = 0; i < (4 - fixedVector[4].size()); i++) {
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
		chapterbosses[4] = insertRandom(rng, chapterbosses[4], fixedVector[4]);

		// Gate Bosses
		for (int i = 0; i < (8 - fixedVector[5].size()); i++) {
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
		chapterbosses[5] = insertRandom(rng, chapterbosses[5], fixedVector[5]);

		// Galdera
		for (int i = 0; i < (2 - fixedVector[6].size()); i++) {
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
		chapterbosses[6] = insertRandom(rng, chapterbosses[6], fixedVector[6]);
	}
	// Duplicates Allowed
	else {
		// Check for mixing of chapter 1 - 4
		if (mixChapter14 == true) {
			// Have no bars on randomizing chapter 1 bosses
			// Chapter 1
			for (int j = 0; j < (8 - fixedVector[0].size()); j++) {
				int ranboss;
				bool exclude;
				// Exclude checking
				do {
					exclude = false;
					uniform_int_distribution <mt19937::result_type> bosses(0, 46);
					ranboss = bosses(rng);
					exclude = excludeChecker(ranboss, includeShrineBosses, includeGateBosses, includeGaldera);
				} while (exclude == true);
				chapterbosses[0].push_back(ranboss);
			}
			chapterbosses[0] = insertRandom(rng, chapterbosses[0], fixedVector[0]);
			// Chapter 2
			for (int j = 0; j < (8 - fixedVector[1].size()); j++) {
				int ranboss;
				bool exclude;
				// Exclude checking
				do {
					exclude = false;
					uniform_int_distribution <mt19937::result_type> bosses(0, 46);
					ranboss = bosses(rng);
					exclude = excludeChecker(ranboss, includeShrineBosses, includeGateBosses, includeGaldera);
				} while (exclude == true);
				chapterbosses[1].push_back(ranboss);
			}
			chapterbosses[1] = insertRandom(rng, chapterbosses[1], fixedVector[1]);
			// Chapter 3
			for (int j = 0; j < (8 - fixedVector[2].size()); j++) {
				int ranboss;
				bool exclude;
				// Exclude checking
				do {
					exclude = false;
					uniform_int_distribution <mt19937::result_type> bosses(0, 46);
					ranboss = bosses(rng);
					exclude = excludeChecker(ranboss, includeShrineBosses, includeGateBosses, includeGaldera);
				} while (exclude == true);
				chapterbosses[2].push_back(ranboss);
			}
			chapterbosses[2] = insertRandom(rng, chapterbosses[2], fixedVector[2]);
			// Chapter 4 bosses
			for (int i = 0; i < (9 - fixedVector[3].size()); i++) {
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
			chapterbosses[3] = insertRandom(rng, chapterbosses[3], fixedVector[3]);

			// Shrine Bosses
			for (int i = 0; i < (4 - fixedVector[4].size()); i++) {
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
			chapterbosses[4] = insertRandom(rng, chapterbosses[4], fixedVector[4]);

			// Gate Bosses
			for (int i = 0; i < (8 - fixedVector[5].size()); i++) {
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
			chapterbosses[5] = insertRandom(rng, chapterbosses[5], fixedVector[5]);

			// Galdera
			for (int i = 0; i < (2 - fixedVector[6].size()); i++) {
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
			chapterbosses[6] = insertRandom(rng, chapterbosses[6], fixedVector[6]);
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
			// Chapter 2
			for (int j = 0; j < (8 - fixedVector[1].size()); j++) {
				int ranboss;
				bool exclude;
				do {
					exclude = false;
					uniform_int_distribution <mt19937::result_type> bosses(8, 46);
					ranboss = bosses(rng);
					exclude = excludeChecker(ranboss, includeShrineBosses, includeGateBosses, includeGaldera);
				} while (exclude == true);
				chapterbosses[1].push_back(ranboss);
			}
			chapterbosses[1] = insertRandom(rng, chapterbosses[1], fixedVector[1]);
			// Chapter 3
			for (int j = 0; j < (8 - fixedVector[2].size()); j++) {
				int ranboss;
				bool exclude;
				do {
					exclude = false;
					uniform_int_distribution <mt19937::result_type> bosses(8, 46);
					ranboss = bosses(rng);
					exclude = excludeChecker(ranboss, includeShrineBosses, includeGateBosses, includeGaldera);
				} while (exclude == true);
				chapterbosses[2].push_back(ranboss);
			}
			chapterbosses[2] = insertRandom(rng, chapterbosses[2], fixedVector[2]);
			// Chapter 4 bosses
			for (int i = 0; i < (9 - fixedVector[3].size()); i++) {
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
			chapterbosses[3] = insertRandom(rng, chapterbosses[3], fixedVector[3]);

			// Shrine Bosses
			for (int i = 0; i < (4 - fixedVector[4].size()); i++) {
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
			chapterbosses[4] = insertRandom(rng, chapterbosses[4], fixedVector[4]);
			// Gate Bosses
			for (int i = 0; i < (8 - fixedVector[5].size()); i++) {
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
			chapterbosses[5] = insertRandom(rng, chapterbosses[5], fixedVector[5]);

			// Galdera
			for (int i = 0; i < (2 - fixedVector[6].size()); i++) {
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
			chapterbosses[6] = insertRandom(rng, chapterbosses[6], fixedVector[6]);
		}
	}
	return chapterbosses;
}