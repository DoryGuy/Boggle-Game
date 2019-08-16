//
//  bogle_game.hpp
//  BoggleMultiProcess
//
//  Created by Gary Powell on 7/5/19.
//  Copyright © 2019 Guest User. All rights reserved.
//

#ifndef boggle_game_hpp
#define boggle_game_hpp

#include <set>
#include <string>
#include <vector>

#include "location.hpp"
#include "word_dictionary.hpp"
#include <mutex>


class BoggleGame {
private:
    // the real game has a "Qu" so we need to store two letters, or always just insert "u" when we see
    // "Q". Thus we can't use just a single char for the letter. For the moment we'll use std::string.
    // maybe later an optimization will be to use a special short string fn.
    using  BoggleBoard_t=std::vector<std::string>;
    
    // maybe later use a more generic fn pointer here.
    using MoveFunctionPtr_t = Move_t (Location::*)();
    
    // indicator that there are no more possible words following this path.
    enum class KeepTrying_t { keepGoing, stop };
    
    BoggleBoard_t board;
    std::vector<MoveFunctionPtr_t> moveFnPtrs;
    std::string currentWord;
    WordDictionary dictionary;
    
    std::mutex foundWordsMutex;
    std::set<std::string> foundWords;
    
    KeepTrying_t checkWord(std::string word);
    // called recursively until all directions are tried
    // and while it's possible to move.
    void moveNextPostion(std::string currentWord, Location original_location);
    
public:
    BoggleGame();
    void init_board();
    void fill_board_for_testing(std::string const &testData);
    int score() const;
    std::ostream &printTheBoard(std::ostream &);

    std::set<std::string> play_game();
};
#endif /* boggle_game_hpp */
