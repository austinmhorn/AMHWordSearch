#include "WordSearchGame.hpp"

std::ostream & operator << (std::ostream &os, const WordSearchGame& gameboard) {
    for(int i = 0; i < gameboard.rows; i++) {
        for(int j = 0; j < gameboard.cols; j++) {
            os << gameboard.grid[i][j] << " ";
        }
        os << std::endl;
    }
    return os;
}

void WordSearchGame::printDifficulty() {
    switch(this->difficulty) {
        case 0:
        case 1:
            std::cout << "(\033[32m\e[1mEasy\e[0m)" << std::endl;
            break;
        case 2:
            std::cout << "(\033[33m\e[1mMedium\e[0m)" << std::endl;
            break;
        case 3:
            std::cout << "(\033[31m\e[1mHard\e[0m)" << std::endl;
            break;
    }
}

void WordSearchGame::initGrid() {
    std::vector<char> gridRow;
    
    /* Clear grid of previous gameplay if needed */
    this->grid.clear();
    /* Reset words from previous gameplay if needed */
    this->wordsFound = 0;
    
    switch(this->difficulty) {
        case 1:
            this->rows           = 10;
            this->cols           = 10;
            this->numWordsInPlay = 5;
            
            for(int i = 0; i < this->rows; i++) {
                gridRow.clear();
                for(int j = 0; j < this->cols; j++) {
                    gridRow.push_back('-');
                }
                this->grid.push_back(gridRow);
            }
            break;
        case 2:
            this->rows           = 15;
            this->cols           = 15;
            this->numWordsInPlay = 7;
            
            for(int i = 0; i < this->rows; i++) {
                gridRow.clear();
                for(int j = 0; j < this->cols; j++) {
                    gridRow.push_back('-');
                }
                this->grid.push_back(gridRow);
            }
            break;
        case 3:
            this->rows           = 20;
            this->cols           = 20;
            this->numWordsInPlay = 10;
            
            for(int i = 0; i < this->rows; i++) {
                gridRow.clear();
                for(int j = 0; j < this->cols; j++) {
                    gridRow.push_back('-');
                }
                this->grid.push_back(gridRow);
            }
            break;
    }
}

void WordSearchGame::fillWordbank() {
    std::ifstream inFS;
    std::string line;
    
    /* Empty wordbank from previous gameplay */
    this->wordbank.clear();
    
    /* Attempt to establish input file stream connection with data file */
    try {
        switch(this->difficulty) {
            case 1:
                inFS.open(DATA_FILE_EASY, std::ios_base::in);
                break;
            case 2:
                inFS.open(DATA_FILE_MEDIUM, std::ios_base::in);
                break;
            case 3:
                inFS.open(DATA_FILE_HARD, std::ios_base::in);
                break;
            default:
                throw std::runtime_error("Unknown error occurred.");
                break;
        }
        /* Print error if neccesary */
        if(!inFS) {
            /* Missing data file essential for architecture */
            /* Prevents gameplay in this difficulty */
            std::cout << "\033[31m\e[1m\e[4m\nCaught Critical Program Error\e[0m: ";
            throw std::runtime_error(NOFILE.message()+" (\"" + DATA_FILE_EASY + "\").");
        }
        else {
            while(std::getline(inFS, line)) {
                this->wordbank.push_back(line);
            }
        }
    }
    catch(const std::runtime_error &excpt) {
        std::cout << excpt.what() << std::endl;
        std::cout << "\nFailed establishing data contents for selected difficulty ";
        this->printDifficulty();
    }
    inFS.close();
}

bool WordSearchGame::checkWillFit(std::string word, Point point, Orientation direction) {
    
    bool willFit  = false;
    int letterLoc = 0;
    
    /* Function returns false when the word attempting to be pinned is obstructed by a boundary or existing word */
    switch(direction) {
        case NORMAL:
            /* Verify if new word will fit with NORMAL orientation */
            if( point.col + word.size() <= this->cols ) {
                for(int j = point.col; j <= point.col + word.size(); j++) {
                    if(this->grid[point.row][j] != '-') {
                        willFit = false;
                        return willFit;
                    }
                }
                willFit = true;
            }
            /* MARK: Draw word on grid IF AND ONLY IF it will fit */
            if(willFit == true) {
                for(int j = point.col; j < point.col + word.size(); j++) {
                    this->grid[point.row][j] = word.at(letterLoc);
                    letterLoc++;
                }
            }
            break;
        case BACKWARDS:
            if( point.col + word.size() <= this->cols ) {
                for(int j = point.col; j <= point.col + word.size(); j++) {
                    if(this->grid[point.row][j] != '-') {
                        willFit = false;
                        return willFit;
                    }
                }
                willFit = true;
            }
            /* MARK: Draw word on grid IF AND ONLY IF it will fit */
            if(willFit == true) {
                letterLoc = word.size() - 1;
                for(int i = 0; i < word.size(); i++) {
                    this->grid[point.row][point.col + i] = word.at(letterLoc);
                    letterLoc--;
                }
            }
            break;
        case DIAGONAL:
            /* MARK: Attempt to draw diagonal in four directions before returning false */
            /* MARK: Each attempt to draw diagonally will follow these procedures: */
            /*      Step 1 → Determine if word will be cut off by grid boundaries. */
            /*      Step 2 → Parse the elements located on the coordinate grid that lie in new word's path.
                    Step 3 → If an element located in the new word's path has already been assigned a unique character, immediately abort process since the new word would overwrite it. */

            /* MARK: Down Right */
            if( point.row + word.size() <= this->rows &&
                point.col + word.size() <= this->cols ) {
                
                for(int i = point.row; i < point.row + word.size(); i++) {
                    for(int j = point.col; j < point.col + word.size(); j++) {
                        if(this->grid[i][j] != '-') {
                            willFit = false;
                            return willFit;
                }}}
                willFit = true;
            }
            if(willFit == true) {
                for(int i = 0; i < word.size(); i++) {
                    this->grid[point.row + i][point.col + i] = word.at(letterLoc);
                    letterLoc++;
                }
                return true;
            }
            
            /* MARK: Down Left */
            if( point.row + word.size() <= this->rows &&
                point.col - word.size() >= 0 ) {
                for(int i = point.row; i < point.row + word.size(); i++) {
                    for(int j = point.col; j > point.col - word.size(); j--) {
                        if(this->grid[i][j] != '-') {
                            willFit = false;
                            return willFit;
                }}}
                willFit = true;
            }
            if(willFit == true) {
                for(int i = 0; i < word.size(); i++) {
                    this->grid[point.row + i][point.col - i] = word.at(letterLoc);
                    letterLoc++;
                }
                return true;
            }
            
            /* MARK: Up Right */
            if( point.row - word.size() >= 0 &&
                point.col + word.size() <= this->cols ) {
                for(int i = point.row; i > point.row - word.size(); i--) {
                    for(int j = point.col; j < point.col + word.size(); j++) {
                        if(this->grid[i][j] != '-') {
                            willFit = false;
                            return willFit;
                }}}
                willFit = true;
            }
            if(willFit == true) {
                for(int i = 0; i < word.size(); i++) {
                    this->grid[point.row - i][point.col + i] = word.at(letterLoc);
                    letterLoc++;
                }
                return true;
            }
            
            /* MARK: Up Left */
            if( point.row - word.size() >= 0 &&
                point.col - word.size() >= 0 ) {
                for(int i = point.row; i > point.row - word.size(); i--) {
                    for(int j = point.col; j > point.col - word.size(); j--) {
                        if(this->grid[i][j] != '-') {
                            willFit = false;
                            return willFit;
                }}}
                willFit = true;
            }
            if(willFit == true) {
                for(int i = 0; i < word.size(); i++) {
                    this->grid[point.row - i][point.col - i] = word.at(letterLoc);
                    letterLoc++;
                }
                return true;
            }
            break;
        default:
            /* Included to protect from undetermined behavior causing default case scenario */
            return false;
            break;
    }
    return willFit;
}

void WordSearchGame::pinWordsToGrid() {
    int randWords[numWordsInPlay]; // Stores integers that represent the position within the wordbank vector that each random word is being sourced from.
    std::vector<std::string>::iterator iter;
    Point point;
    Orientation direction;
    bool willFit;
    
    /* Randomly generate numWordsInPlay integers from 0 - wordbank to serve as the location(s) within the wordbank vector that each random word will be accessed from. */
    for(int i = 0; i < this->numWordsInPlay; i++) {
        randWords[i] = rand() % this->wordbank.size();
    }
    
    /* Check for duplicate locations set to access */
    for (int i = 0; i < numWordsInPlay - 1; i++) {
        for (int j = i + 1; j < numWordsInPlay; j++) {
            if (randWords[i] == randWords[j]) {
                randWords[i] = rand() % this->wordbank.size();
                i = 0; /* Restart process to ensure no duplicates */
            }
        }
    }
    
    /* Populate temporary vector with the randomly selected words */
    for(int i = 0; i < this->numWordsInPlay; i++) {
        this->activeWords.push_back(this->wordbank.at(randWords[i]));
    }
    
    /* Iterate through temporary vector of words */
    for(iter = this->activeWords.begin(); iter != this->activeWords.end(); iter++) {
        /* MARK: Attempt to locate an open line on gameboard grid for word to be written. */
        /* Repeat loop if any elements on grid that new word would occupy are unsuitable */
        do {
            /* Randomly assign an orientation for the word to be written */
            direction = static_cast<Orientation>(rand() % 3 + 1);
            /* Randomly generate location on 2D vector for word to be written */
            point.row = rand() % this->rows;
            point.col = rand() % this->cols;
            
            //std::cout << std::endl;
            //std::cout << "Word:         " << *iter << std::endl;
            //std::cout << "Direction:    " << direction << std::endl;
            //std::cout << "Start Point: (" << point.row << ',' << point.col << ')' << std::endl;
            
            /* MARK: If checkWillFit() returned true, it wrote the word to grid as well; allowing the do-while loop to terminate and next word in vector (words) to be evaluated. */
            willFit = ( checkWillFit(*iter, point, direction) ) ? true : false;
            
            //if(willFit == true) { std::cout << "Will fit." << std::endl; }
            //else { std::cout << "Won't fit." << std::endl; }
            //std::cout << std::endl;

        }while(willFit == false);
    }
}

void WordSearchGame::finalizeGrid() {
    for(int i = 0; i < this->rows; i++) {
        for(int j = 0; j < this->cols; j++) {
            if(this->grid[i][j] == '-') {
                this->grid[i][j] = static_cast<char>(rand() % 26 + 65);
            }
            else { continue; }
        }
    }
}

bool WordSearchGame::checkForWord(std::string word) {
    std::string temp;
    
    for(int i = 0; i < this->numWordsInPlay - this->wordsFound; i++) {
        if( word == this->activeWords.at(i) ) {
            /* Swap matching word with last element in vector */
            temp = this->activeWords.back();
            this->activeWords.back() = activeWords.at(i);
            this->activeWords.at(i) = temp;
            /* Remove last word in vector */
            this->activeWords.pop_back();
            this->wordsFound++;
            return true;
        }
    }
    return false;
}
