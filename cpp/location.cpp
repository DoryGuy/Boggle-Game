//
//  location.cpp
//  BoggleMultiProcess
//
//  Created by Gary Powell on 7/5/19.
//  Copyright © 2019 Guest User. All rights reserved.
//

#include "location.hpp"

namespace {
    // do less computation while running, a bit more while compiling.
    constexpr int numberOfColumnsInBoardLessOne = numberOfColumnsInBoard - 1;
    constexpr int numberOfRowsInBoardLessOne = numberOfRowsInBoard - 1;
}
    
Move_t Location::MoveUp() {
    if (row > 0) {
        auto index(indexInBoard(--row, col));
        if (locations_available[index]) {
            locations_available[index] = false;
            return Move_t::MOVE_SUCCESS;
        }
        ++row;
    }
    return Move_t::MOVE_FAIL;
}

Move_t Location::MoveDown() {
    if (row < numberOfRowsInBoardLessOne) {
        auto index(indexInBoard(++row, col));
        if (locations_available[index]) {
            locations_available[index] = false;
            return Move_t::MOVE_SUCCESS;
        }
        --row;
    }
    return Move_t::MOVE_FAIL;
}

Move_t Location::MoveLeft() {
    if (col > 0) {
        auto index(indexInBoard(row, --col));
        if (locations_available[index]) {
            locations_available[index] = false;
            return Move_t::MOVE_SUCCESS;
        }
        ++col;
    }
    return Move_t::MOVE_FAIL;
}

Move_t Location::MoveRight() {
    if (col < numberOfColumnsInBoardLessOne) {
        auto index(indexInBoard(row, ++col));
        if (locations_available[index]) {
            locations_available[index] = false;
            return Move_t::MOVE_SUCCESS;
        }
        --col;
    }
    return Move_t::MOVE_FAIL;
}

Move_t Location::MoveUpRight() {
    if (row > 0) {
        auto new_col = col + 1;
        if (new_col < numberOfColumnsInBoard) {
            auto index(indexInBoard(--row, new_col));
            if (locations_available[index]) {
                col = new_col;
                locations_available[index] = false;
                return Move_t::MOVE_SUCCESS;
            }
            ++row;
        }
    }
    return Move_t::MOVE_FAIL;
}

Move_t Location::MoveUpLeft() {
    if (row > 0) {
        if (col > 0) {
            auto index(indexInBoard(--row, --col));
            if (locations_available[index]) {
                locations_available[index] = false;
                return Move_t::MOVE_SUCCESS;
            }
            ++row;
            ++col;
        }
    }
    return Move_t::MOVE_FAIL;
}

Move_t Location::MoveDownRight() {
    if (row < numberOfRowsInBoardLessOne) {
        if (col < numberOfColumnsInBoardLessOne) {
            auto index(indexInBoard(++row, ++col));
            if (locations_available[index]) {
                locations_available[index] = false;
                return Move_t::MOVE_SUCCESS;
            }
            --row;
            --col;
        }
    }
    return Move_t::MOVE_FAIL;
}

Move_t Location::MoveDownLeft() {
    if (row < numberOfRowsInBoardLessOne) {
        if (col > 0) {
            auto index(indexInBoard(++row, --col));
            if (locations_available[index]) {
                locations_available[index] = false;
                return Move_t::MOVE_SUCCESS;
            }
            --row;
            ++col;
        }
    }
    return Move_t::MOVE_FAIL;
}

std::ostream& Location::printLocation(std::ostream &os) const {
    for (auto row = 0; row < numberOfRowsInBoard; ++row) {
        for (auto col = 0; col < numberOfColumnsInBoard; ++col) {
            os << (locations_available[indexInBoard(row,col)] ? "0 " : "X ");
        }
        os << std::endl;
    }
    return os;
}

