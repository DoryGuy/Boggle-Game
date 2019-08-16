//
//  location.cpp
//  BoggleMultiProcess
//
//  Created by Gary Powell on 7/5/19.
//  Copyright © 2019 Guest User. All rights reserved.
//

#include "location.hpp"
#include "GameConstants.hpp"
#include <algorithm>

namespace {
    // do less computation while running, a bit more while compiling.
    constexpr int numberOfColumnsInBoardLessOne = numberOfColumnsInBoard - 1;
    constexpr int numberOfRowsInBoardLessOne = numberOfRowsInBoard - 1;
}
    
void Location::init_board() {
    
    if (locations_available.size() == max_board_elements) {
        using std::fill;
        fill(locations_available.begin(), locations_available.end(), true);
    } else {
        locations_available.reserve(max_board_elements);
        for (auto i = 0; i < max_board_elements; ++i) {
            locations_available.push_back(true);
        }
    }
    locations_available[indexInBoard(row, col)] = false; // we start at the initial position.
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
            int index(indexInBoard(--row, new_col));
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

void Location::printLocation(std::ostream &os) const {
    for (auto row = 0; row < numberOfRowsInBoard; ++row) {
        for (auto col = 0; col < numberOfColumnsInBoard; ++col) {
            os << (locations_available[indexInBoard(row,col)] ? "0 " : "X ");
        }
        os << std::endl;
    }
}

