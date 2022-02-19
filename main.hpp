#ifndef main_hpp
#define main_hpp

#include "WordSearchGame.hpp"

/* Global int storing user's Main Menu route selection */
int menuChoice = -1;

/* Function that waits until the user presses ENTER */
void waitForUser();
void changeDifficulty(WordSearchGame *game);
void displayMainMenu(WordSearchGame *game);

#endif /* main_hpp */
