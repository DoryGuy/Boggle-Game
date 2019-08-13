//
//  dictionary.cpp
//  BoggleMultiProcess
//
//  Created by Gary Powell on 7/5/19.
//  Copyright © 2019 Guest User. All rights reserved.
//

#include "word_dictionary.hpp"
#include <algorithm>
#include <iostream>

    
void WordDictionary::insertWord(std::string word) {
    //using std::transform;
    //transform(word.begin(), word.end(), word.begin(), ::tolower);
    auto hashKey = hashTheWord(word);
    auto words{std::move(dictionary[hashKey])};
    words.insert(std::move(word));
    dictionary[hashKey] = words;
}

// a hash for our dictionary.
unsigned long WordDictionary::hashTheWord(std::string word) const {
    using std::sort;
    sort(word.begin(), word.end());
    return std::hash<std::string>{}(word);
}

WordDictionary::WordDictionary() {
    // fill it with words...
    
    std::string wordList[] = {
     "test"
    ,"arm"
    ,"art"
    ,"ate"
    ,"atom"
    ,"bar"
    ,"dart"
    ,"darts"
    ,"data"
    ,"date"
    ,"dates"
    ,"doom"
    ,"door"
    ,"dorm"
    ,"dote"
    ,"eat"
    ,"eats"
    ,"east"
    ,"fade"
    ,"far"
    ,"farm"
    ,"fart"
    ,"farts"
    ,"fate"
    ,"fates"
    ,"fast"
    ,"foo"
    ,"food"
    ,"foot"
    ,"for"
    ,"form"
    ,"format"
    ,"formats"
    ,"formatted"
    ,"fort"
    ,"forts"
    ,"mat"
    ,"mate"
    ,"mated"
    ,"matted"
    ,"mats"
    ,"moat"
    ,"moats"
    ,"mode"
    ,"modes"
    ,"mom"
    ,"moot"
    ,"motar"
    ,"ode"
    ,"off"
    ,"omit"
    ,"quit"
    ,"quite"
    ,"ram"
    ,"ram"
    ,"rat"
    ,"rats"
    ,"rate"
    ,"rates"
    ,"rim"
    ,"road"
    ,"rod"
    ,"rode"
    ,"roof"
    ,"room"
    ,"root"
    ,"rooted"
    ,"roots"
    ,"rote"
    ,"sat"
    ,"sea"
    ,"sear"
    ,"seat"
    ,"start"
    ,"stat"
    ,"state"
    ,"stated"
    ,"storm"
    ,"tad"
    ,"tar"
    ,"tart"
    ,"tat"
    ,"tear"
    ,"teat"
    ,"tetra"
    ,"too"
    ,"tram"
    ,"trim"
    ,"trod"
    };
    for (auto& word: wordList) {
        insertWord(std::move(word));
    }
}

// check for both forward spelling and reverse spelling.
std::pair<WordDictionary::FoundWord_t, std::string> WordDictionary::isInDictionary(std::string word) const {
    using std::reverse;
    
    auto wordsListIter = dictionary.find(hashTheWord(word));
    if (wordsListIter != dictionary.end()) {
        auto iter = wordsListIter->second.find(word);
        if (iter != wordsListIter->second.end()) {
            return {FoundWord_t::Found,std::move(word)};
        }

        // useful if we can figure out how to eliminate searching
        // from every node position.
        reverse(word.begin(), word.end());
        iter = wordsListIter->second.find(word);
        if (iter != wordsListIter->second.end()) {
            return {FoundWord_t::Found,std::move(word)};
        }
    }
    return {FoundWord_t::NotFound,{}};
}

void WordDictionary::printDictionary(std::ostream &os) const {
    os << "Dictionary contains: " << dictionary.size() << " words " << std::endl;
    for (auto wordList: dictionary) {
        for (auto word : wordList.second) {
            std::cout << word << std::endl;
        }
    }
    os << "----------" << std::endl;
}
