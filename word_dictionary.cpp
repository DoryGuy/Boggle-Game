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
    using std::transform;
    transform(word.begin(), word.end(), word.begin(), ::tolower);
    auto hashKey = hashTheWord(word);
    auto words = dictionary[hashKey];
    words.emplace(std::move(word));
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
    insertWord("test");
    insertWord("foo");
    insertWord("bar");
    insertWord("mom");
    insertWord("quit");
    insertWord("form");
    insertWord("for");
    insertWord("omit");
    insertWord("data");
    insertWord("teat");
    insertWord("fort");
    insertWord("tat");
    insertWord("atom");
    insertWord("ate");
    insertWord("Storm");
    insertWord("dart");
    insertWord("tart");
    insertWord("tar");
    insertWord("fart");
    insertWord("farm");
    insertWord("far");
    insertWord("rat");
    insertWord("sat");
    insertWord("dorm");
    insertWord("mat");
    insertWord("quit");
    insertWord("quite");
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
