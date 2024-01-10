//
//  location.hpp
//  BoggleMultiProcess
//
//  Created by Gary Powell on 7/5/19.
//  Copyright © 2019 Guest User. All rights reserved.
//

#ifndef location_hpp
#define location_hpp

#include <ostream>
#include <vector>

enum class Move_t { MOVE_SUCCESS, MOVE_FAIL };

class Location {
private:
    using LocationsAvailable_t=std::vector<bool>;
    
    int row;
    int col;
    LocationsAvailable_t locations_available;
    
public:
    Location(int r = 0, int c = 0)
    : row(r)
    , col(c)
    {
        init_board();
    }
    
    Location(Location &&) = default;
    Location(Location const &) = default;
    
    constexpr int getRow() const { return row; }
    constexpr int getCol() const { return col; }
    
    void init_board();
    Move_t MoveUp();
    Move_t MoveDown();
    Move_t MoveLeft();
    Move_t MoveRight();
    Move_t MoveUpRight();
    Move_t MoveUpLeft();
    Move_t MoveDownRight();
    Move_t MoveDownLeft();
    void printLocation(std::ostream &os) const;
};

#endif /* location_hpp */
