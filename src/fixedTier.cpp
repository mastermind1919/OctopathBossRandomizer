#include "Octopath.h"


// Force some bosses to be on a tier
vectorvector fixedTeir(std::mt19937 rng, vectorvector inputVector, std::vector<int> forceNumbers, bool forceGaldera, bool includeDuplicate) {
	
	vectorvector fixedVector(7);

	// Check for duplicates option
	if (includeDuplicate == false) {

		// Chapter 1
		if (forceNumbers[0] != 0) {
			// Generate number of bosses specified by user
				for (int i = 0; i < forceNumbers[0]; i++) {
					// variable for checking if boss is already picked
					bool match;
					int ranboss;
					// keep generating random number until number doesn't match any previous bosses
					do {
						match = false;
						std::uniform_int_distribution<std::mt19937::result_type> bosses(0, 7);
						ranboss = bosses(rng);
						// check if boss is already choosen
						if (!fixedVector[0].empty() && !std::none_of(fixedVector[0].begin(), fixedVector[0].end(), compare(ranboss))) {
							match = true;
						}

					} while (match == true);
					fixedVector[0].push_back(ranboss);
				}
		}
		// Chapter 2
		if (forceNumbers[1] != 0) {
			// Generate number of bosses specified by user
				for (int i = 0; i < forceNumbers[1]; i++) {
					// variable for checking if boss is already picked
					bool match;
					int ranboss;
					// keep generating random number until number doesn't match any previous bosses
					do {
						match = false;
						std::uniform_int_distribution<std::mt19937::result_type> bosses(8, 15);
						ranboss = bosses(rng);
						// check if boss is already choosen
						if (!fixedVector[1].empty() && !std::none_of(fixedVector[1].begin(), fixedVector[1].end(), compare(ranboss))) {
							match = true;
						}

					} while (match == true);
					fixedVector[1].push_back(ranboss);
				}
		}
		// Chapter 3
		if (forceNumbers[2] != 0) {
			// Generate number of bosses specified by user
				for (int i = 0; i < forceNumbers[2]; i++) {
					// variable for checking if boss is already picked
					bool match;
					int ranboss;
					// keep generating random number until number doesn't match any previous bosses
					do {
						match = false;
						std::uniform_int_distribution<std::mt19937::result_type> bosses(16, 23);
						ranboss = bosses(rng);
						// check if boss is already choosen
						if (!fixedVector[2].empty() && !std::none_of(fixedVector[2].begin(), fixedVector[2].end(), compare(ranboss))) {
							match = true;
						}

					} while (match == true);
					fixedVector[2].push_back(ranboss);
				}
		}
		// Chapter 4
		if (forceNumbers[3] != 0) {
			// Generate number of bosses specified by user
				for (int i = 0; i < forceNumbers[3]; i++) {
					// variable for checking if boss is already picked
					bool match;
					int ranboss;
					// keep generating random number until number doesn't match any previous bosses
					do {
						match = false;
						std::uniform_int_distribution<std::mt19937::result_type> bosses(24, 32);
						ranboss = bosses(rng);
						// check if boss is already choosen
						if (!fixedVector[3].empty() && !std::none_of(fixedVector[3].begin(), fixedVector[3].end(), compare(ranboss))) {
							match = true;
						}

					} while (match == true);
					fixedVector[3].push_back(ranboss);
				}
		}
		// Shrine bosses
		if (forceNumbers[4] != 0) {
			// Generate number of bosses specified by user
				for (int i = 0; i < forceNumbers[4]; i++) {
					// variable for checking if boss is already picked
					bool match;
					int ranboss;
					// keep generating random number until number doesn't match any previous bosses
					do {
						match = false;
						std::uniform_int_distribution<std::mt19937::result_type> bosses(33, 36);
						ranboss = bosses(rng);
						// check if boss is already choosen
						if (!fixedVector[4].empty() && !std::none_of(fixedVector[4].begin(), fixedVector[4].end(), compare(ranboss))) {
							match = true;
						}

					} while (match == true);
					fixedVector[4].push_back(ranboss);
				}
		}
		// Gate Bosses
		if (forceNumbers[5] != 0) {
			// Generate number of bosses specified by user
				for (int i = 0; i < forceNumbers[5]; i++) {
					// variable for checking if boss is already picked
					bool match;
					int ranboss;
					// keep generating random number until number doesn't match any previous bosses
					do {
						match = false;
						std::uniform_int_distribution<std::mt19937::result_type> bosses(37, 44);
						ranboss = bosses(rng);
						// check if boss is already choosen
						if (!fixedVector[5].empty() && !std::none_of(fixedVector[5].begin(), fixedVector[5].end(), compare(ranboss))) {
							match = true;
						}

					} while (match == true);
					fixedVector[5].push_back(ranboss);
				}
		}
		// Galdera
		if (forceGaldera == true) {
			// Galdera can only be one, otherwise just disable include Galdera
			int ranboss;
			std::uniform_int_distribution<std::mt19937::result_type> bosses(45, 46);
			ranboss = bosses(rng);
			fixedVector[6].push_back(ranboss);
		}
	}
	else {
		// Chapter 1
		if (forceNumbers[0] != 0) {
			for (int i = 0; i < forceNumbers[0]; i++) {
				int ranboss;
				std::uniform_int_distribution<std::mt19937::result_type> bosses(0, 7);
				ranboss = bosses(rng);
				fixedVector[0].push_back(ranboss);
			}
		}
		// Chapter 2
		if (forceNumbers[1] != 0) {
			for (int i = 0; i < forceNumbers[1]; i++) {
				int ranboss;
				std::uniform_int_distribution<std::mt19937::result_type> bosses(8, 15);
				ranboss = bosses(rng);
				fixedVector[1].push_back(ranboss);
			}
		}
		// Chapter 3
		if (forceNumbers[2] != 0) {
			for (int i = 0; i < forceNumbers[2]; i++) {
				int ranboss;
				std::uniform_int_distribution<std::mt19937::result_type> bosses(16, 23);
				ranboss = bosses(rng);
				fixedVector[2].push_back(ranboss);
			}
		}
		// Chapter 4
		if (forceNumbers[3] != 0) {
			for (int i = 0; i < forceNumbers[3]; i++) {
				int ranboss;
				std::uniform_int_distribution<std::mt19937::result_type> bosses(24, 32);
				ranboss = bosses(rng);
				fixedVector[3].push_back(ranboss);
			}
		}
		// Shrine Bosses
		if (forceNumbers[4] != 0) {
			for (int i = 0; i < forceNumbers[4]; i++) {
				int ranboss;
				std::uniform_int_distribution<std::mt19937::result_type> bosses(33, 36);
				ranboss = bosses(rng);
				fixedVector[4].push_back(ranboss);
			}
		}
		// Gate Bosses
		if (forceNumbers[5] != 0) {
			for (int i = 0; i < forceNumbers[5]; i++) {
				int ranboss;
				std::uniform_int_distribution<std::mt19937::result_type> bosses(37, 44);
				ranboss = bosses(rng);
				fixedVector[5].push_back(ranboss);
			}
		}
		// Galdera
		// Can only be one, otherwise just disable include Galdera
		if (forceGaldera == true) {
			int ranboss;
			std::uniform_int_distribution<std::mt19937::result_type> bosses(45, 46);
			ranboss = bosses(rng);
			fixedVector[6].push_back(ranboss);
		}
	}

	return fixedVector;
}