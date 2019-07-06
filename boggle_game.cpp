//
//  bogle_game.cpp
//  BogleMultiProcess
//
//  Created by Gary Powell on 7/5/19.
//  Copyright © 2019 Guest User. All rights reserved.
//

#include "boggle_game.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <thread>

#include "GameConstants.hpp"
#include "ThreadRAII.hpp"

// the real game has a "Qu" so we need to store two letters, or always just insert "u" when we see "Q".



BogleGame::BogleGame()
{
    init_board();
    moveFnPtrs.reserve(8); // eight possible moves
    moveFnPtrs.emplace_back(&Location::MoveUp);
    moveFnPtrs.emplace_back(&Location::MoveDown);
    moveFnPtrs.emplace_back(&Location::MoveLeft);
    moveFnPtrs.emplace_back(&Location::MoveRight);
    moveFnPtrs.emplace_back(&Location::MoveUpRight);
    moveFnPtrs.emplace_back(&Location::MoveUpLeft);
    moveFnPtrs.emplace_back(&Location::MoveDownRight);
    moveFnPtrs.emplace_back(&Location::MoveDownLeft);
    
    dictionary.printDictionary(std::cout);
}
    
void BogleGame::init_board() {
    using std::string;
    foundWords.clear();
    // stupid but easy way to fill board.
    board.clear();
    board.reserve(max_board_elements);
    for (auto i = 0; i < max_board_elements; ++i) {
        auto c = (rand() % 25) + 'a';
        string letter;
        if (c == 'q') {
            letter += "qu";
        }
        else {
            letter += c;
        }
        board.emplace_back(std::move(letter));
    }
}
    
void BogleGame::fill_board_for_testing(std::string testData)
{
    using std::string;
    board.clear();
    foundWords.clear();
    for (auto c: testData){
        string letter;
        letter += c;
        if (c == 'q' || c == 'Q') { // Q automaticlly becomes "Qu";
            letter += 'u';
        }

        board.emplace_back(letter);
    }
}
    
void BogleGame::checkWord(std::string word) {
    using std::transform;
    transform(word.begin(), word.end(), word.begin(), ::tolower);
    auto result = dictionary.isInDictionary(word);
    if (result.first) {
        std::lock_guard<std::mutex> lockFoundWords(foundWordsMutex);
        std::cout << " Found " << result.second << std::endl;
        foundWords.emplace(std::move(result.second));
    }
}
    
// called recursively until all directions are tried
// and while it's possible to move.
void BogleGame::moveNextPostion(std::string currentWord, Location original_location) {
    for (auto fn: moveFnPtrs) {
        Location location(original_location);
        std::string newWord(currentWord);
        if ((location.*fn)() == Move_t::MOVE_SUCCESS) {
            //location.printLocation(); std::cout << std::endl;
            newWord += board[indexToMove(location.getRow(), location.getCol())];
            checkWord(newWord);
            // debug only look for <= 4 letter words.
            if (newWord.length() < 4 ) {
                moveNextPostion(newWord, location);
            }
        }
    }
}
    
    // return the unique words found
std::set<std::string> BogleGame::play_game(){
    using std::string;
    using std::vector;
    using std::thread;
    
    vector<ThreadRAII> processes;
    // preallocate some space
    processes.reserve(numberOfColumnsInBoard*numberOfRowsInBoard);
    for (int row = 0; row < numberOfRowsInBoard; ++row) {
        for (int col = 0; col < numberOfColumnsInBoard; ++col) {
            string currentWord;
            Location location(row, col);
            currentWord += board[indexToMove(row,col)]; // start word at this location.
            auto moveFn = [&,location=std::move(location),currentWord=std::move(currentWord)]() -> void {
                moveNextPostion(currentWord, location);
            };
            processes.emplace_back(ThreadRAII(thread(moveFn), ThreadRAII::DtorAction::join));
            //moveFn();
        }
    }
    
    for (auto& t: processes) {
        t.get().join();
    }
    return foundWords;
}
