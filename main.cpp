#include "main.hpp"

int main(int argc, const char * argv[]) {
    
    srand(time(NULL));
    WordSearchGame game;
    std::string line;
    bool wordFound;
    unsigned int numGuesses = 0;
                
    do {
        displayMainMenu(&game);
        
        /* Sets game difficulty to Easy by default  */
        if(game.getDifficulty() == 0) {
            game.setDifficulty(1);
        }
        
        try {
            switch(menuChoice) {
                case 1:
                    /* Draws grid (gameboard) to size determined by game difficulty */
                    game.initGrid();
                    /* Open input file stream and populate wordbank */
                    game.fillWordbank();
                    /* Verify the vector (wordbank) was populated after attempting to read from data stream */
                    if(game.getWordbankSize() == 0) {
                        /* Avoid accessing an empty container */
                        throw std::runtime_error("Gameplay in this difficulty will be \e[1m\e[4mimpossible\e[0m.");
                    }
                    /* Write words to game board */
                    game.pinWordsToGrid();
                    /* Fill empty spaces with random letters */
                    game.finalizeGrid();
                    /* Reset user attempt counter variable */
                    numGuesses = 0;
                    
                    do {
                        /* MARK: Clear screen contents */
                        system("clear");
                        
                        /* Display the number of words remaining to find */
                        std::cout << "Words To Find: " << "\e[1m\033[32m" << game.getNumWordsInPlay() - game.getWordsFound() << "\e[0m" << std::endl;
                        
                        /* Utilizes overloaded ostream operator to print gameboard */
                        std::cout << game;
                        
                        /* Avoids printing during first execution */
                        if( numGuesses > 0 ) {
                            if(wordFound == true) {
                                std::cout << "Found word(\"\033[32m\e[1m" << line << "\e[0m\")!" << std::endl;
                            }
                            else {
                                std::cout << "Couldn't find word(\"\033[31m\e[1m" << line << "\e[0m\")." << std::endl;
                            }
                        }
                                                
                        std::cout << "*** Type found word in \e[1m\e[4mALL CAPS\e[0m below || Type 'q' to quit ***" << std::endl;
                        std::cout << ">>" << std::flush;
                        std::getline(std::cin, line);
                        std::cout << std::endl;
                        if(line.at(0) != 'q') {
                            numGuesses++;
                            wordFound = game.checkForWord(line);
                        }
                    }while( line.at(0) != 'q' && ( game.getNumWordsInPlay() != game.getWordsFound() ) );
    
                    /* MARK: Prompt user with a scenario appropriate message before returning to main menu */
                    if( game.getWordsFound() == game.getNumWordsInPlay() ) {
                        
                        std::cout << "Total # Of Words To Find: " << game.getNumWordsInPlay() << std::endl;
                        std::cout << "Total # Of Words Found:   " << game.getWordsFound() << std::endl;
                        std::cout << "Total # Of Attempts:      " << numGuesses << std::endl;
                        
                        
                        /* FIXME: *** NEEDS ADJUSTMENTS *** */
                        //std::cout << "SCORE: " << ( game.getNumWordsInPlay() + game.getWordsFound() ) - numGuesses << std::endl;
                        
                        
                        std::cout << std::endl;
                        std::cout << "Congratulations! You win!" << std::endl;
                    }
                    else {
                        std::cout << "Sorry! Maybe next time!" << std::endl;
                    }
                    waitForUser();
                    break;
                case 2:
                    changeDifficulty(&game);
                    break;
                case 3:
                    std::cout << "\n\e[3mShutting down...\e[0m" << std::endl;
                    break;
            }
        }
        catch(const std::runtime_error & excpt) {
            std::cout << excpt.what() << std::endl;
            waitForUser();
        }
    }while(menuChoice != 3);
    return 0;
}

void waitForUser() {
    std::cout << "\nPress \033[34m\e[1mENTER\e[0m to return to Main Menu..."; std::cin.ignore();
    std::cout << std::endl;
}

void changeDifficulty(WordSearchGame *game) {
    /* Store unclean input given by user */
    std::string line;
    
    /* MARK: Reset difficulty for loop control below. If not included, an empty line would print error message and return to main menu. This way, the user remains within the function until a valid integer is given. */
    game->setDifficulty(0);
    
    do {
            std::cout << "\e[1m*** Select difficulty ***\e[0m" << std::endl;
            std::cout << "\t 1 → Easy" << std::endl;
            std::cout << "\t 2 → Medium" << std::endl;
            std::cout << "\t 3 → Hard" << std::endl;
            
            /* Read the entire line of input given by user into a string */
            std::cout << ">>"; std::getline(std::cin, line);
            
            try { /* Attempt to parse user input */
                if(line.size() != 1) {
                    throw std::domain_error("Length of input was incorrect. Please enter an integer.");
                }
                else {
                    game->setDifficulty(atoi(&line.at(0)));
                    if(game->getDifficulty() < 1 || game->getDifficulty() > 3) {
                        game->setDifficulty(0);
                        throw std::domain_error("Invalid integer entered. Enter 1-3.");
                    }
                    else { break; }
                }
            }
            catch(const std::domain_error &excpt) {
                std::cout << excpt.what() << std::endl;
            }
    }while(game->getDifficulty() == 0);
    
    std::cout << "\nDifficulty setting has been updated to ";
    game->printDifficulty();
    waitForUser();
}

void displayMainMenu(WordSearchGame *game) {
    /* Store unclean input given by user*/
    std::string line;
    /* MARK: Reset menu selection to prevent possible crosstalk from a previous chosen route. */
    menuChoice = -1;
    /* MARK: Clear screen contents */
    system("clear");
    
    /* Loop terminates when user enters a valid integer */
    do {
        /* Print Main Menu */
        std::cout << "\e[1m\033[34m*** AMH WordSearch ***\e[0m" << std::endl;
        std::cout << "\t1 → Start WordSearch" << std::endl;
        std::cout << "\t2 → Change Difficulty ";
        game->printDifficulty();
        std::cout << "\t3 → Quit" << std::endl;
        /* Read the entire line of input given by user into a string */
        std::cout << ">>"; std::getline(std::cin, line);
        
        try { /* Attempt to parse user input */
            /* Check if  */
            if(line.size() != 1) {
                throw std::runtime_error("\n\e[4mLength of input was incorrect. Please enter an integer.\e[0m\n");
            }
            else {
                menuChoice = atoi(&line.at(0));
                if(menuChoice < 1 || menuChoice > 3) {
                    menuChoice = -1;
                    throw std::runtime_error("Invalid integer entered. Enter 1-3.");
                }
            }
        }
        catch(const std::runtime_error &excpt) {
            std::cout << excpt.what() << std::endl;
        }
        
    }while(menuChoice == -1);
}
