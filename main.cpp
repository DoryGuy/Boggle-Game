//
//  main.cpp
//  Boggle
//
//  Created by Gary Powell on 6/18/19.
//  Copyright © 2019 Guest User. All rights reserved.
//

#include <iostream>

#include <cassert>

#include "boggle_game.hpp"

int main(int argc, const char * argv[]) {
    // insert code here...
    BoggleGame game;
    
    
    game.fill_board_for_testing("Qitexxxxxxxxxxxx"); // horizontal
    auto result = game.play_game();
    std::cout << "Horizonatal Words Found = " << result.size() << "!\n";
    for (auto word: result) {
        std::cout << word << std::endl;
    }
    assert(result.size() == 1);
    
    game.fill_board_for_testing("Testxxxxxxxxxxxx"); // horizontal
    result = game.play_game();
    std::cout << "Horizonatal Words Found = " << result.size() << "!\n";
    for (auto word: result) {
        std::cout << word << std::endl;
    }
    assert(result.size() == 1);
    
    game.fill_board_for_testing("TseTxxxxxxxxxxxx"); // horizontal reversed
    result = game.play_game();
    std::cout << "Horizontal reversed Words Found = " << result.size() << "!\n";
    for (auto word: result) {
        std::cout << word << std::endl;
    }
    assert(result.size() == 1);
    
    game.fill_board_for_testing("Txxxxexxxxsxxxxt"); // diagonal
    std::cout << "Diagonal Words Found = " << result.size() << "!\n";
    for (auto word: result) {
        std::cout << word << std::endl;
    }
    assert(result.size() == 1);
    
    game.fill_board_for_testing("txxxxsxxxxexxxxT"); // diagonal reversed
    std::cout << "Diagonal reversed Words Found = " << result.size() << "!\n";
    for (auto word: result) {
        std::cout << word << std::endl;
    }
    std::cout << std::endl;
    assert(result.size() == 1);
    
    game.fill_board_for_testing("Txxxexxxsxxxtxxx"); // vertical
    result = game.play_game();
    std::cout << "Vertical Words Found = " << result.size() << "!\n";
    for (auto word: result) {
        std::cout << word << std::endl;
    }
    std::cout << std::endl;
    assert(result.size() == 1);
    
    game.fill_board_for_testing("txxxsxxxexxxTxxx"); // vertical reversed
    result = game.play_game();
    std::cout << "Vertical reversed Words Found = " << result.size() << "!\n";
    for (auto word: result) {
        std::cout << word << std::endl;
    }
    std::cout << std::endl;
    assert(result.size() == 1);
    
    game.fill_board_for_testing("Ttxxesxxxxxxxxxx"); // quad
    result = game.play_game();
    std::cout << "Quad Words Found = " << result.size() << "!\n";
    for (auto word: result) {
        std::cout << word << std::endl;
    }
    std::cout << std::endl;
    assert(result.size() == 1);
    
    game.fill_board_for_testing("tTxxsexxxxxxxxxx"); // quad reversed
    result = game.play_game();
    std::cout << "Quad reversed Words Found = " << result.size() << "!\n";
    for (auto word: result) {
        std::cout << word << std::endl;
    }
    std::cout << std::endl;
    assert(result.size() == 1);
    
    game.fill_board_for_testing("Texxstxxxxxxxxxx"); // Z
    result = game.play_game();
    std::cout << "Z Words Found = " << result.size() << "!\n";
    for (auto word: result) {
        std::cout << word << std::endl;
    }
    std::cout << std::endl;
    assert(result.size() == 1);
    
    game.fill_board_for_testing("tsxxeTxxxxxxxxxx"); // Z reversed
    result = game.play_game();
    std::cout << "Z reversed Words Found = " << result.size() << "!\n";
    for (auto word: result) {
        std::cout << word << std::endl;
    }
    std::cout << std::endl;
    assert(result.size() == 1);
    
    game.fill_board_for_testing("TestDataForMomit");
    result = game.play_game();
    std::cout << "Words Found = " << result.size() << "!\n";
    for (auto word: result) {
        std::cout << word << std::endl;
    }
    std::cout << std::endl;
    
    game.fill_board_for_testing("TDFMeaoostrmtatt");
    result = game.play_game();
    std::cout << "Words Found = " << result.size() << "!\n";
    for (auto word: result) {
        std::cout << word << std::endl;
    }
    return 0;
}
