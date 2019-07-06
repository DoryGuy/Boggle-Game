//
//  GameConstants.h
//  BogleMultiProcess
//
//  Created by Gary Powell on 7/5/19.
//  Copyright © 2019 Guest User. All rights reserved.
//

#ifndef GameConstants_hpp
#define GameConstants_hpp

constexpr int numberOfColumnsInBoard = 4;
constexpr int numberOfRowsInBoard = 4;

inline int indexToMove(int row, int column) {
    // debug check row < numberOfColumns && column < numberOfColums
    return (numberOfColumnsInBoard * row) + column;
}

constexpr int max_board_elements = numberOfRowsInBoard * numberOfColumnsInBoard;

#endif /* GameConstants_hpp */
