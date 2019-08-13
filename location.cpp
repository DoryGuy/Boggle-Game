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
    locations_available[indexToMove(row, col)] = false; // we start at the initial position.
}

Move_t Location::MoveUp() {
    if (row > 0) {
        auto new_row = row - 1;
        auto index(indexToMove(new_row, col));
        if (locations_available[index]) {
            row = new_row;
            locations_available[index] = false;
            return Move_t::MOVE_SUCCESS;
        }
    }
    return Move_t::MOVE_FAIL;
}

Move_t Location::MoveDown() {
    auto new_row = row + 1;
    if (new_row < numberOfRowsInBoard) {
        auto index(indexToMove(new_row, col));
        if (locations_available[index]) {
            row = new_row;
            locations_available[index] = false;
            return Move_t::MOVE_SUCCESS;
        }
    }
    return Move_t::MOVE_FAIL;
}

Move_t Location::MoveLeft() {
    if (col > 0) {
        auto new_col = col - 1;
        auto index(indexToMove(row, new_col));
        if (locations_available[index]) {
            col = new_col;
            locations_available[index] = false;
            return Move_t::MOVE_SUCCESS;
        }
    }
    return Move_t::MOVE_FAIL;
}

Move_t Location::MoveRight() {
    auto new_col = col + 1;
    if (new_col < numberOfColumnsInBoard) {
        auto index(indexToMove(row, new_col));
        if (locations_available[index]) {
            col = new_col;
            locations_available[index] = false;
            return Move_t::MOVE_SUCCESS;
        }
    }
    return Move_t::MOVE_FAIL;
}

Move_t Location::MoveUpRight() {
    if (row > 0) {
        auto new_row = row - 1;
        auto new_col = col + 1;
        if (new_col < numberOfColumnsInBoard) {
            int index(indexToMove(new_row, new_col));
            if (locations_available[index]) {
                row = new_row;
                col = new_col;
                locations_available[index] = false;
                return Move_t::MOVE_SUCCESS;
            }
        }
    }
    return Move_t::MOVE_FAIL;
}

Move_t Location::MoveUpLeft() {
    if (row > 0) {
        auto new_row = row - 1;
        if (col > 0) {
            auto new_col = col - 1;
            auto index(indexToMove(new_row, new_col));
            if (locations_available[index]) {
                row = new_row;
                col = new_col;
                locations_available[index] = false;
                return Move_t::MOVE_SUCCESS;
            }
        }
    }
    return Move_t::MOVE_FAIL;
}

Move_t Location::MoveDownRight() {
    auto new_row = row + 1;
    if (new_row < numberOfRowsInBoard) {
        auto new_col = col + 1;
        if (new_col < numberOfColumnsInBoard) {
            auto index(indexToMove(new_row, new_col));
            if (locations_available[index]) {
                row = new_row;
                col = new_col;
                locations_available[index] = false;
                return Move_t::MOVE_SUCCESS;
            }
        }
    }
    return Move_t::MOVE_FAIL;
}

Move_t Location::MoveDownLeft() {
    auto new_row = row + 1;
    if (new_row < numberOfRowsInBoard) {
        if (col > 0) {
            auto new_col = col - 1;
            auto index(indexToMove(new_row, new_col));
            if (locations_available[index]) {
                row = new_row;
                col = new_col;
                locations_available[index] = false;
                return Move_t::MOVE_SUCCESS;
            }
        }
    }
    return Move_t::MOVE_FAIL;
}

void Location::printLocation(std::ostream &os) const {
    for (auto row = 0; row < numberOfRowsInBoard; ++row) {
        for (auto col = 0; col < numberOfColumnsInBoard; ++col) {
            os << (locations_available[indexToMove(row,col)] ? "0 " : "X ");
        }
        os << std::endl;
    }
}

