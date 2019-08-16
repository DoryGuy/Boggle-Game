//
//  main.cpp
//  Boggle
//
//  Created by Gary Powell on 6/18/19.
//  Copyright © 2019 Guest User. All rights reserved.
//

#include <chrono>
#include <iostream>

#include <cassert>

#include "boggle_game.hpp"

namespace { // anonymous namespace
    
void runTest(BoggleGame &game
             , std::string testName
             , std::string testBoardInput
             , int expectedWordCount) {
    using std::cout;
    
    cout << "Starting test " << testName << "\n";
    game.fill_board_for_testing(std::move(testBoardInput));
    game.printTheBoard(cout);
    auto now = std::chrono::high_resolution_clock::now();
    auto result = game.play_game();
    auto run_time = std::chrono::high_resolution_clock::now() - now;
    cout << "Took " << std::chrono::duration_cast<std::chrono::milliseconds>(run_time).count()
    << " millieconds \n";
    cout << "Words Found = " << result.size() << "! Score = " << game.score() << "\n";
    for (auto word: result) {
        cout << word << std::endl;
    }
    assert(result.size() == expectedWordCount);
}
}

int main(int argc, const char * argv[]) {
    using std::cout;
    // insert code here...
    BoggleGame game;
  
    if (false) // basic test cases.
    {
        runTest(game, "horizontal Q", "Qitexxxxxxxxxxxx", 3);

        runTest(game, "horizontal", "Testxxxxxxxxxxxx", 8);

        runTest(game, "horizontal reversed", "TseTxxxxxxxxxxxx", 8);
    
        runTest(game, "diagonal", "Txxxxexxxxsxxxxt", 6);
    
        runTest(game, "diagonal reversed", "txxxxsxxxxexxxxT", 6);
    
        runTest(game, "vertical", "Txxxexxxsxxxtxxx", 8);

        runTest(game, "vertical reversed", "txxxsxxxexxxTxxx", 8);
 
        runTest(game, "quad", "Ttxxesxxxxxxxxxx", 9);
        
        runTest(game, "quad reversed", "tTxxsexxxxxxxxxx", 12);
        
        runTest(game, "Z pattern", "Texxstxxxxxxxxxx", 12);
    
        runTest(game, "Z pattern reversed", "tsxxeTxxxxxxxxxx", 12);
    }
    
    //runTest(game, "Just some test data", "TestDataForMomit", 379);
    
    //runTest(game, "Just some test data vertical", "TDFMeaoostrmtatt", 304);
    
    runTest(game, "Best known board", "GNESSRIPETALTSEB", 1351);


    return 0;
}
