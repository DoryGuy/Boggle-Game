//
//  dictionary.hpp
//  BoggleMultiProcess
//
//  Created by Gary Powell on 7/5/19.
//  Copyright © 2019 Guest User. All rights reserved.
//

#ifndef dictionary_hpp
#define dictionary_hpp

#include <memory>
#include <ostream>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>


class WordDictionary {
private:
    std::unordered_map<unsigned long, std::unique_ptr<std::set<std::string>>> dictionary;
    
    void insertWord(std::string word);
public:
    enum class FoundWord_t { Found, NotFound };
    enum class LeadingPrefixFound_t { Found, NotFound };
    
    WordDictionary();
    // check for both forward spelling and reverse spelling.
    std::tuple<FoundWord_t, LeadingPrefixFound_t, std::string> isInDictionary(std::string word) const;
    void printDictionary(std::ostream &os) const;
};
#endif /* dictionary_hpp */
