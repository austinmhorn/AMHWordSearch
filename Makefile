# Usage:
# make			# compile all binary
# make run		# run executable
# make clean		# remove ALL binaries, objects, and executable

# compiler
CC := g++ -std=c++11

# compiler flags
CFLAGS := -Wall -c

# build target
TARGET := AMHWordSearch

$(TARGET): main.o WordSearchGame.o
	@echo "Creating target..."
	@$(CC) main.o WordSearchGame.o -o $(TARGET)
	@echo "Target created successfully."

main.o: main.cpp WordSearchGame.hpp
	@echo "Creating main.o..."
	@$(CC) $(CFLAGS) main.cpp
	@echo "Complete."

WordSearchGame.o: WordSearchGame.cpp WordSearchGame.hpp
	@echo "Creating WordSearchGame.o..."
	@$(CC) $(CFLAGS) WordSearchGame.cpp
	@echo "Complete."

clean:
	@rm *.o
	@rm AMHWordSearch

run:
	./AMHWordSearch
