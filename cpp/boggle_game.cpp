//
//  bogle_game.cpp
//  BoggleMultiProcess
//
//  Created by Gary Powell on 7/5/19.
//  Copyright © 2019 Guest User. All rights reserved.
//

#include "boggle_game.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <thread>

#include "GameConstants.hpp"
#include "JThreadRAII.hpp"

// the real game has a "Qu" so we need to store two letters, or always just insert "u" when we see "Q".

BoggleGame::BoggleGame()
{
    init_board();
    
    dictionary.printDictionary(std::cout);
}
    
void BoggleGame::init_board() {
    using std::string;
    foundWords.clear();
    // stupid but easy way to fill board.
    board.clear();
    board.reserve(max_board_elements);
    for (auto _ = 0; _ < max_board_elements; ++_) {
        const auto c = static_cast<char>((rand() % 25) + 'a');
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
    
void BoggleGame::fill_board_for_testing(std::string_view const &testData)
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
    
BoggleGame::KeepTrying_t BoggleGame::checkWord(std::string word) {
    using std::transform;
    using std::get;
    transform(word.begin(), word.end(), word.begin(), ::tolower);
    auto result = dictionary.isInDictionary(word);
    if (get<0>(result) == WordDictionary::FoundWord_t::Found) {
        std::lock_guard<std::mutex> lockFoundWords(foundWordsMutex);
#ifdef DEBUG_SEARCH
        std::cout << " Found " << get<2>(result) << std::endl;
#endif
        foundWords.insert(std::move(get<2>(result)));
    }
    return get<1>(result) == WordDictionary::LeadingPrefixFound_t::Found ?
           BoggleGame::KeepTrying_t::keepGoing : BoggleGame::KeepTrying_t::stop;
}

// called recursively until all directions are tried
// and while it's possible to move, and find a new word.
void BoggleGame::moveNextPostion(std::string currentWord, Location original_location) {
    for (auto fn: moveFnPtrs) {
        Location location(original_location);
        std::string newWord(currentWord);
        KeepTrying_t keepOnTrucking = KeepTrying_t::keepGoing;
        if ((location.*fn)() == Move_t::MOVE_SUCCESS) {
            //location.printLocation(); std::cout << std::endl;
            newWord += board[indexInBoard(location.getRow(), location.getCol())];
            if (newWord.length() > 2) {
                keepOnTrucking = checkWord(newWord);            }
            if (keepOnTrucking == KeepTrying_t::keepGoing) {
                moveNextPostion(std::move(newWord), std::move(location));
            }
        }
    }
}
    
    // return the unique words found
std::set<std::string> BoggleGame::play_game(){
    using std::string;
    using std::vector;
    using std::jthread;
    
    // we are going to use one thread for each location on the board.
    vector<JThreadRAII> processes;
    // preallocate enough space
    processes.reserve(numberOfColumnsInBoard*numberOfRowsInBoard);
    for (int row = 0; row < numberOfRowsInBoard; ++row) {
        for (int col = 0; col < numberOfColumnsInBoard ; ++col) {
            string currentWord;
            Location location(row, col);
            currentWord += board[indexInBoard(row,col)]; // start word at this location.
            auto moveFn = [&,location=std::move(location),currentWord=std::move(currentWord)]() -> void {
                moveNextPostion(currentWord, location);
            };
            processes.emplace_back(jthread(moveFn), JThreadRAII::DtorAction::join);
        }
    }
    
    for (auto& t: processes) {
        if (t.joinable() ) {
            t.join();
        }
    }
    return foundWords;
}

int scoreWord(const std::string_view &word) {
    int result = 0;
    switch(word.length()) {
        case 0:
        case 1:
        case 2:
            // result = 0;
            break;
        case 3:
        case 4:
            result = 1;
            break;
        case 5:
            result = 2;
            break;
        case 6:
            result = 3;
            break;
        case 7:
            result = 5;
            break;
        default:
            result = 11;
    }
    return result;
}

int BoggleGame::score() const {
    int result = 0;
    for (auto word: foundWords) {
        result += scoreWord(word);
    }
    return result;
}

std::ostream &BoggleGame::printTheBoard(std::ostream &os) {
    
    for (int row = 0; row < numberOfRowsInBoard; ++row) {
        for (int col = 0; col < numberOfColumnsInBoard; ++col) {
            const auto c = board[indexInBoard(row, col)];
            os << c;
            if (c.length() == 2 ) {
                os << " ";
            } else {
                os << "  ";
            }
        }
        os << "\n";
    }
    
    return os;
}
