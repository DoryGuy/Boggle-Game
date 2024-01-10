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
#include <string_view>
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
    

    
    BoggleBoard_t board;
    std::vector<MoveFunctionPtr_t> moveFnPtrs;
    std::string currentWord;
    WordDictionary dictionary;
    
    std::mutex foundWordsMutex;
    std::set<std::string> foundWords;
    
    // called recursively until all directions are tried
    // and while it's possible to move.
    void moveNextPostion(std::string currentWord, Location original_location);
    
public:
    // indicator that there are no more possible words following this path.
    enum class KeepTrying_t { keepGoing, stop };
    
    BoggleGame();
    void init_board();
    void fill_board_for_testing(std::string_view const &testData);
    int score() const;
    
    // for debugging
    KeepTrying_t checkWord(std::string word);
    std::ostream &printTheBoard(std::ostream &);

    std::set<std::string> play_game();
};
#endif /* boggle_game_hpp */
