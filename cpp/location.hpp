//
//  location.hpp
//  BoggleMultiProcess
//
//  Created by Gary Powell on 7/5/19.
//  Copyright © 2019 Guest User. All rights reserved.
//

#ifndef location_hpp
#define location_hpp

#include "GameConstants.hpp"
#include <array>
#include <algorithm>
#include <ostream>


enum class Move_t { MOVE_SUCCESS, MOVE_FAIL };

class Location {
private:
    using LocationsAvailable_t=std::array<bool, max_board_elements>;
    
    int row;
    int col;
    LocationsAvailable_t locations_available;
    
public:
    constexpr explicit Location(int r = 0, int c = 0)
    : row(r)
    , col(c)
    {
        std::fill(locations_available.begin(), locations_available.end(), true);
        locations_available[indexInBoard(row, col)] = false; // we start at the initial position.
    }
    
    Location(Location &&) = default;
    Location(Location const &) = default;
    
    [[nodiscard]] constexpr int getRow() const { return row; }
    [[nodiscard]] constexpr int getCol() const { return col; }
    
    [[nodiscard]] Move_t MoveUp();
    [[nodiscard]] Move_t MoveDown();
    [[nodiscard]] Move_t MoveLeft();
    [[nodiscard]] Move_t MoveRight();
    [[nodiscard]] Move_t MoveUpRight();
    [[nodiscard]] Move_t MoveUpLeft();
    [[nodiscard]] Move_t MoveDownRight();
    [[nodiscard]] Move_t MoveDownLeft();
    std::ostream & printLocation(std::ostream &os) const;
};

#endif /* location_hpp */
