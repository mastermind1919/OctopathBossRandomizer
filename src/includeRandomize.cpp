#include "Octopath.h"

// Randomizer for none mixed include options

// Function for indexing the options
int generateRandom(mt19937 rng, bool includeShrineBosses, bool includeGateBosses, bool includeGaldera) {
	int ranboss;
	bool exclude;
	if (includeShrineBosses == true && includeGateBosses == false && includeGaldera == false) {
		uniform_int_distribution <mt19937::result_type> bosses(24, 36);
		ranboss = bosses(rng);
	}
	else if (includeShrineBosses == true && includeGateBosses == true && includeGaldera == false) {
		uniform_int_distribution <mt19937::result_type> bosses(24, 44);
		ranboss = bosses(rng);
	}
	else if (includeShrineBosses == true && includeGateBosses == true && includeGaldera == true) {
		uniform_int_distribution <mt19937::result_type> bosses(24, 46);
		ranboss = bosses(rng);
	}
	else if (includeShrineBosses == true && includeGateBosses == false && includeGaldera == true) {
		do {
			exclude = false;
			uniform_int_distribution <mt19937::result_type> bosses(24, 44);
			ranboss = bosses(rng);
			// exclude the gate bosses
			if (37 <= ranboss && ranboss <= 44) {
				exclude = true;
			}
		} while (exclude == true);
	}
	else if (includeShrineBosses == false && includeGateBosses == true && includeGaldera == false) {
		do {
			exclude = false;
			uniform_int_distribution <mt19937::result_type> bosses(24, 44);
			ranboss = bosses(rng);
			// exclude the shrine bosses
			if (33 <= ranboss && ranboss <= 36) {
				exclude = true;
			}
		} while (exclude == true);
	}
	else if (includeShrineBosses == false && includeGateBosses == true && includeGaldera == true) {
		do {
			exclude = false;
			uniform_int_distribution <mt19937::result_type> bosses(24, 46);
			ranboss = bosses(rng);
			// exclude the shrine bosses
			if (33 <= ranboss && ranboss <= 36) {
				exclude = true;
			}
		} while (exclude == true);
	}
	else if (includeShrineBosses == false && includeGateBosses == false && includeGaldera == true) {
		do {
			exclude = false;
			uniform_int_distribution <mt19937::result_type> bosses(24, 46);
			ranboss = bosses(rng);
			// exclude the shrine bosses and gate bosses
			if (33 <= ranboss && ranboss <= 44) {
				exclude = true;
			}
		} while (exclude == true);
	}
	// Catch all option
	else{
		cout << "Something has gone horribly wrong in includeRandom. Contact mastermind1919 with the config file attached" << endl;
		throw;
		ranboss = 0;
	}
	return ranboss;
}

vectorvector includeRandomize(mt19937 rng, bool randomizeShrineBosses, bool includeShrineBosses, bool randomizeGateBosses, bool includeGateBosses, bool includeGaldera) {
	// First three chpater bosses are the same as base, only the chapter 4 potentially changes

	// Since we are only randomizing the bosses among themselves, create a vector with 4 indexes to hold the boss integers
	// note: vector starts from 0, so chapter 1 is 0, chapter 2 is 1, etc...
	vectorvector chapterbosses(7);

	// Randomize, with check for duplicates
	for (int i = 0; i < 3; i++) {
		intvector currentbosses;
		for (int j = 0; j < 8; j++) {
			// variable for checking if boss is already picked
			bool match;
			int ranboss;
			// keep generating random number until number doesn't match any previous bosses
			do {
				match = false;
				uniform_int_distribution<mt19937::result_type> bosses(8 * i, 8 * i + 7);
				ranboss = bosses(rng);
				// check if boss is already choosen
				currentbosses = chapterbosses[i];
				if (!currentbosses.empty() && !std::none_of(currentbosses.begin(), currentbosses.end(), compare(ranboss))) {
					match = true;
				}

			} while (match == true);
			// push onto vector the varible that is generated
			chapterbosses[i].push_back(ranboss);
		}
	}
	// For chapter four, check which options are checked, and randomize.
	// Assume that randomizing the other bosses is enabled.
	for (int i = 0; i < 9; i++) {
		bool match;
		int ranboss;
		do {
			match = false;
			bool exclude;
			if (includeShrineBosses == true && includeGateBosses == false && includeGaldera == false) {
				uniform_int_distribution <mt19937::result_type> bosses(24, 36);
				ranboss = bosses(rng);
			}
			else if (includeShrineBosses == true && includeGateBosses == true && includeGaldera == false) {
				uniform_int_distribution <mt19937::result_type> bosses(24, 44);
				ranboss = bosses(rng);
			}
			else if (includeShrineBosses == true && includeGateBosses == true && includeGaldera == true) {
				uniform_int_distribution <mt19937::result_type> bosses(24, 46);
				ranboss = bosses(rng);
			}
			else if (includeShrineBosses == true && includeGateBosses == false && includeGaldera == true) {
				do {
					exclude = false;
					uniform_int_distribution <mt19937::result_type> bosses(24, 44);
					ranboss = bosses(rng);
					// exclude the gate bosses
					if (37 <= ranboss && ranboss <= 44) {
						exclude = true;
					}
				} while (exclude == true);
			}
			else if (includeShrineBosses == false && includeGateBosses == true && includeGaldera == false) {
				do {
					exclude = false;
					uniform_int_distribution <mt19937::result_type> bosses(24, 44);
					ranboss = bosses(rng);
					// exclude the shrine bosses
					if (33 <= ranboss && ranboss <= 36) {
						exclude = true;
					}
				} while (exclude == true);
			}
			else if (includeShrineBosses == false && includeGateBosses == true && includeGaldera == true) {
				do {
					exclude = false;
					uniform_int_distribution <mt19937::result_type> bosses(24, 46);
					ranboss = bosses(rng);
					// exclude the shrine bosses
					if (33 <= ranboss && ranboss <= 36) {
						exclude = true;
					}
				} while (exclude == true);
			}
			else if (includeShrineBosses == false && includeGateBosses == false && includeGaldera == true) {
				do {
					exclude = false;
					uniform_int_distribution <mt19937::result_type> bosses(24, 46);
					ranboss = bosses(rng);
					// exclude the shrine bosses and gate bosses
					if (33 <= ranboss && ranboss <= 44) {
						exclude = true;
					}
				} while (exclude == true);
			}
			// Catch all option
			else {
				cout << "Something has gone horribly wrong in includeRandom. Contact mastermind1919 with the config file attached" << endl;
				throw;
				ranboss = 0;
			}
			// Since this is the first layer, only care about chapter 4 bosses
			if (!chapterbosses[3].empty() && !std::none_of(chapterbosses[3].begin(), chapterbosses[3].end(), compare(ranboss))) {
				match = true;
			}
		} while (match == true);
		// push onto the vector the variable that is generated
		chapterbosses[3].push_back(ranboss);
	}
	// Shrine bosses
	// check for shrine boss option first, then call randomizer
	if (includeShrineBosses == true) {
		for (int i = 0; i < 4; i++) {
			bool match;
			int ranboss;
			do {
				match = false;
				bool exclude;
				if (includeShrineBosses == true && includeGateBosses == false && includeGaldera == false) {
					uniform_int_distribution <mt19937::result_type> bosses(24, 36);
					ranboss = bosses(rng);
				}
				else if (includeShrineBosses == true && includeGateBosses == true && includeGaldera == false) {
					uniform_int_distribution <mt19937::result_type> bosses(24, 44);
					ranboss = bosses(rng);
				}
				else if (includeShrineBosses == true && includeGateBosses == true && includeGaldera == true) {
					uniform_int_distribution <mt19937::result_type> bosses(24, 46);
					ranboss = bosses(rng);
				}
				else if (includeShrineBosses == true && includeGateBosses == false && includeGaldera == true) {
					do {
						exclude = false;
						uniform_int_distribution <mt19937::result_type> bosses(24, 44);
						ranboss = bosses(rng);
						// exclude the gate bosses
						if (37 <= ranboss && ranboss <= 44) {
							exclude = true;
						}
					} while (exclude == true);
				}
				else if (includeShrineBosses == false && includeGateBosses == true && includeGaldera == false) {
					do {
						exclude = false;
						uniform_int_distribution <mt19937::result_type> bosses(24, 44);
						ranboss = bosses(rng);
						// exclude the shrine bosses
						if (33 <= ranboss && ranboss <= 36) {
							exclude = true;
						}
					} while (exclude == true);
				}
				else if (includeShrineBosses == false && includeGateBosses == true && includeGaldera == true) {
					do {
						exclude = false;
						uniform_int_distribution <mt19937::result_type> bosses(24, 46);
						ranboss = bosses(rng);
						// exclude the shrine bosses
						if (33 <= ranboss && ranboss <= 36) {
							exclude = true;
						}
					} while (exclude == true);
				}
				else if (includeShrineBosses == false && includeGateBosses == false && includeGaldera == true) {
					do {
						exclude = false;
						uniform_int_distribution <mt19937::result_type> bosses(24, 46);
						ranboss = bosses(rng);
						// exclude the shrine bosses and gate bosses
						if (33 <= ranboss && ranboss <= 44) {
							exclude = true;
						}
					} while (exclude == true);
				}
				// Catch all option
				else {
					cout << "Something has gone horribly wrong in includeRandom. Contact mastermind1919 with the config file attached" << endl;
					throw;
					ranboss = 0;
				}
				// check both chapter 4 and shrine bosses
				if (!chapterbosses[3].empty() && !std::none_of(chapterbosses[3].begin(), chapterbosses[3].end(), compare(ranboss))) {
					match = true;
				}
				else if (!chapterbosses[4].empty() && !std::none_of(chapterbosses[4].begin(), chapterbosses[4].end(), compare(ranboss))) {
					match = true;
				}
			} while (match == true);
			chapterbosses[4].push_back(ranboss);
		}
	}
	// check for randomization option first, do not include in chapter 4s
	else {
		if (randomizeShrineBosses == true) {
			for (int i = 0; i < 4; i++) {
				bool match;
				int ranboss;
				do {
					match = false;
					uniform_int_distribution<mt19937::result_type> bosses(33, 36);
					ranboss = bosses(rng);
					if (!chapterbosses[4].empty() && !std::none_of(chapterbosses[4].begin(), chapterbosses[4].end(), compare(ranboss))) {
						match = true;
					}
				} while (match == true);
				chapterbosses[4].push_back(ranboss);
			}
		}
		else {
			for (int i = 0; i < 4; i++) {
				chapterbosses[4].push_back(i + 33);
			}
		}
	}
	
	// Gate bosses
	// Check for include option first
	if (includeGateBosses == true) {
		for (int i = 0; i < 8; i++) {
			bool match;
			int ranboss;
			do {
				match = false;
				bool exclude;
				if (includeShrineBosses == true && includeGateBosses == false && includeGaldera == false) {
					uniform_int_distribution <mt19937::result_type> bosses(24, 36);
					ranboss = bosses(rng);
				}
				else if (includeShrineBosses == true && includeGateBosses == true && includeGaldera == false) {
					uniform_int_distribution <mt19937::result_type> bosses(24, 44);
					ranboss = bosses(rng);
				}
				else if (includeShrineBosses == true && includeGateBosses == true && includeGaldera == true) {
					uniform_int_distribution <mt19937::result_type> bosses(24, 46);
					ranboss = bosses(rng);
				}
				else if (includeShrineBosses == true && includeGateBosses == false && includeGaldera == true) {
					do {
						exclude = false;
						uniform_int_distribution <mt19937::result_type> bosses(24, 44);
						ranboss = bosses(rng);
						// exclude the gate bosses
						if (37 <= ranboss && ranboss <= 44) {
							exclude = true;
						}
					} while (exclude == true);
				}
				else if (includeShrineBosses == false && includeGateBosses == true && includeGaldera == false) {
					do {
						exclude = false;
						uniform_int_distribution <mt19937::result_type> bosses(24, 44);
						ranboss = bosses(rng);
						// exclude the shrine bosses
						if (33 <= ranboss && ranboss <= 36) {
							exclude = true;
						}
					} while (exclude == true);
				}
				else if (includeShrineBosses == false && includeGateBosses == true && includeGaldera == true) {
					do {
						exclude = false;
						uniform_int_distribution <mt19937::result_type> bosses(24, 46);
						ranboss = bosses(rng);
						// exclude the shrine bosses
						if (33 <= ranboss && ranboss <= 36) {
							exclude = true;
						}
					} while (exclude == true);
				}
				else if (includeShrineBosses == false && includeGateBosses == false && includeGaldera == true) {
					do {
						exclude = false;
						uniform_int_distribution <mt19937::result_type> bosses(24, 46);
						ranboss = bosses(rng);
						// exclude the shrine bosses and gate bosses
						if (33 <= ranboss && ranboss <= 44) {
							exclude = true;
						}
					} while (exclude == true);
				}
				// Catch all option
				else {
					cout << "Something has gone horribly wrong in includeRandom. Contact mastermind1919 with the config file attached" << endl;
					throw;
					ranboss = 0;
				}
				// check Chapter 4, Shrine Bosses, and Gate bosses (Shouldn't matter if option is disabled, shouldn't match anyway)
				if (!chapterbosses[3].empty() && !std::none_of(chapterbosses[3].begin(), chapterbosses[3].end(), compare(ranboss))) {
					match = true;
				}
				else if (!chapterbosses[4].empty() && !std::none_of(chapterbosses[4].begin(), chapterbosses[4].end(), compare(ranboss))) {
					match = true;
				}
				else if (!chapterbosses[5].empty() && !std::none_of(chapterbosses[5].begin(), chapterbosses[5].end(), compare(ranboss))) {
					match = true;
				}

			} while (match == true);
			chapterbosses[5].push_back(ranboss);

		}
	}
	// check for randomization option first, do not include in chapter 4s
	else {
		if (randomizeGateBosses == true) {
			for (int i = 0; i < 8; i++) {
				bool match;
				int ranboss;
				do {
					match = false;
					uniform_int_distribution<mt19937::result_type> bosses(37, 44);
					ranboss = bosses(rng);
					if (!chapterbosses[5].empty() && !std::none_of(chapterbosses[5].begin(), chapterbosses[5].end(), compare(ranboss))) {
						match = true;
					}
				} while (match == true);
				chapterbosses[5].push_back(ranboss);
			}
		}
		else {
			for (int i = 0; i < 8; i++) {
				chapterbosses[5].push_back(i + 37);
			}
		}
	}

	// Galdera
	// Just check for include, use normal otherwise
	if (includeGaldera == true) {
		for (int i = 0; i < 2; i++) {
			bool match;
			int ranboss;
			do {
				match = false;
				bool exclude;
				if (includeShrineBosses == true && includeGateBosses == false && includeGaldera == false) {
					uniform_int_distribution <mt19937::result_type> bosses(24, 36);
					ranboss = bosses(rng);
				}
				else if (includeShrineBosses == true && includeGateBosses == true && includeGaldera == false) {
					uniform_int_distribution <mt19937::result_type> bosses(24, 44);
					ranboss = bosses(rng);
				}
				else if (includeShrineBosses == true && includeGateBosses == true && includeGaldera == true) {
					uniform_int_distribution <mt19937::result_type> bosses(24, 46);
					ranboss = bosses(rng);
				}
				else if (includeShrineBosses == true && includeGateBosses == false && includeGaldera == true) {
					do {
						exclude = false;
						uniform_int_distribution <mt19937::result_type> bosses(24, 44);
						ranboss = bosses(rng);
						// exclude the gate bosses
						if (37 <= ranboss && ranboss <= 44) {
							exclude = true;
						}
					} while (exclude == true);
				}
				else if (includeShrineBosses == false && includeGateBosses == true && includeGaldera == false) {
					do {
						exclude = false;
						uniform_int_distribution <mt19937::result_type> bosses(24, 44);
						ranboss = bosses(rng);
						// exclude the shrine bosses
						if (33 <= ranboss && ranboss <= 36) {
							exclude = true;
						}
					} while (exclude == true);
				}
				else if (includeShrineBosses == false && includeGateBosses == true && includeGaldera == true) {
					do {
						exclude = false;
						uniform_int_distribution <mt19937::result_type> bosses(24, 46);
						ranboss = bosses(rng);
						// exclude the shrine bosses
						if (33 <= ranboss && ranboss <= 36) {
							exclude = true;
						}
					} while (exclude == true);
				}
				else if (includeShrineBosses == false && includeGateBosses == false && includeGaldera == true) {
					do {
						exclude = false;
						uniform_int_distribution <mt19937::result_type> bosses(24, 46);
						ranboss = bosses(rng);
						// exclude the shrine bosses and gate bosses
						if (33 <= ranboss && ranboss <= 44) {
							exclude = true;
						}
					} while (exclude == true);
				}
				// Catch all option
				else {
					cout << "Something has gone horribly wrong in includeRandom. Contact mastermind1919 with the config file attached" << endl;
					throw;
					ranboss = 0;
				}
				// check Chapter 4, Shrine Bosses, Gate bosses, and Galdera (Shouldn't matter if option is disabled, shouldn't match anyway)
				if (!chapterbosses[3].empty() && !std::none_of(chapterbosses[3].begin(), chapterbosses[3].end(), compare(ranboss))) {
					match = true;
				}
				else if (!chapterbosses[4].empty() && !std::none_of(chapterbosses[4].begin(), chapterbosses[4].end(), compare(ranboss))) {
					match = true;
				}
				else if (!chapterbosses[5].empty() && !std::none_of(chapterbosses[5].begin(), chapterbosses[5].end(), compare(ranboss))) {
					match = true;
				}
				else if (!chapterbosses[6].empty() && !std::none_of(chapterbosses[6].begin(), chapterbosses[6].end(), compare(ranboss))) {
					match = true;
				}
			} while (match == true);
			chapterbosses[6].push_back(ranboss);
		}
	}
	else {
		for (int i = 0; i < 2; i++) {
			chapterbosses[6].push_back(i + 45);
		}
	}
	return chapterbosses;
}
