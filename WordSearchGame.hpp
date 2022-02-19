#include <cerrno>   /* std::error_condition               */
#include <ctime>    /* srand, rand,                       */
#include <iostream> /* std::cin, std::cout                */
#include <fstream>  /* std::ifstream, std::ofstream       */
#include <string>   /* std::string, std::getline          */
#include <vector>   /* std::vector, std::vector::iterator */

#ifndef WordSearchGame_hpp
#define WordSearchGame_hpp

/* Thrown by exception block when data file for game difficulty is missing or unreadable. */
static std::error_condition NOFILE (ENOENT, std::generic_category() );
/* Controls how the words are written and orientated on grid */
enum Orientation {NORMAL = 1, BACKWARDS = 2, DIAGONAL = 3};
/* Structure to simplify coordinate location handling */
typedef struct Point { int row = -1; int col = -1; } point;
/* Filenames of data files containing each difficulty setting's unique contents. */
/* Each game difficulty requires its respective data file's contents for architecture. */
const std::string DATA_FILE_EASY   = "wordbank_easy.txt";
const std::string DATA_FILE_MEDIUM = "wordbank_medium.txt";
const std::string DATA_FILE_HARD   = "wordbank_hard.txt";

class WordSearchGame {
public:
    /* Constructor */
    WordSearchGame() {
        this->difficulty     = 0;
        this->rows           = 0;
        this->cols           = 0;
        this->numWordsInPlay = 0;
        this->wordsFound     = 0;
    }
    
    /* Mutators */
    void setDifficulty(int newDifficulty) { this->difficulty = newDifficulty; }
    void addWordFound() { this->wordsFound += 1; }
    
    /* Accessors */
    unsigned int getDifficulty()     const { return this->difficulty;      }
    unsigned int getRows()           const { return this->rows;            }
    unsigned int getCols()           const { return this->cols;            }
    unsigned int getNumWordsInPlay() const { return this->numWordsInPlay;  }
    unsigned int getWordbankSize()   const { return this->wordbank.size(); }
    unsigned int getWordsFound()     const { return this->wordsFound;      }
    
    /* Overloaded Operators */
    friend std::ostream & operator << (std::ostream &os, const WordSearchGame& gameboard);
    
    /* Architecture & Gameplay Functions */
    void printDifficulty();
    void initGrid();
    void fillWordbank();
    bool checkWillFit(std::string word, Point point, Orientation direction);
    void pinWordsToGrid();
    void finalizeGrid();
    bool checkForWord(std::string word);
    
private:
    std::vector<std::vector<char> > grid;
    unsigned int difficulty;
    unsigned int rows;
    unsigned int cols;
    unsigned int numWordsInPlay;
    std::vector<std::string> activeWords;
    unsigned int wordsFound;
    std::vector<std::string> wordbank;
};

#endif /* WordSearchGame_hpp */
