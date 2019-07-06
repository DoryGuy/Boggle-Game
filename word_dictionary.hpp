//
//  dictionary.hpp
//  BogleMultiProcess
//
//  Created by Gary Powell on 7/5/19.
//  Copyright © 2019 Guest User. All rights reserved.
//

#ifndef dictionary_hpp
#define dictionary_hpp

#include <ostream>
#include <set>
#include <string>
#include <unordered_map>


class WordDictionary {
private:
    std::unordered_map<unsigned long, std::set<std::string>> dictionary;
    
    void insertWord(std::string word);
    // a hash for our dictionary.
    unsigned long hashTheWord(std::string word) const;
public:
    WordDictionary();
    // check for both forward spelling and reverse spelling.
    std::pair<bool,std::string> isInDictionary(std::string word) const;
    void printDictionary(std::ostream &os) const;
};
#endif /* dictionary_hpp */
