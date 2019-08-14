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
#include <fstream>

// a hash for our dictionary.
unsigned long hashTheWord(std::string const &word) {
#ifdef REVERSE_SEARCH
    using std::sort;
    sort(word.begin(), word.end());
#endif
    std::string leadingChars;
    // I'm not going to check the minimum length of the word here, as we do that
    // before this fn is called.
    leadingChars += word[0];
    leadingChars += word[1];
    leadingChars += word[2];
    return std::hash<std::string>{}(leadingChars);
}
    
void WordDictionary::insertWord(std::string word) {
    using std::transform;
    using std::string;
    //using std::make_unique;
    
    // no point in using a dictionary with words that won't score.
    if (word.length() < 3 || word.length() > 16) {
        return;
    }
    transform(word.begin(), word.end(), word.begin(), ::tolower);
    auto hashKey = hashTheWord(word);
    auto iter = dictionary.find(hashKey);
    
    if (iter == dictionary.end()) {
        using std::set;
        //auto words = std::make_unique<set<string>>>(new set<string>);
        auto words_ptr = std::unique_ptr<set<string>>(new set<string>);
        words_ptr->insert(std::move(word));
        dictionary[hashKey] = std::move(words_ptr);
    }
    else {
        auto words_ptr = iter->second.get();
        words_ptr->insert(std::move(word));
    }
}

WordDictionary::WordDictionary() {
    // fill it with words...
    std::ifstream dictionaryFile;
    dictionaryFile.open("/Users/garypowell/Desktop/QuizQuestions/Boggle-Game/Boggle-Game/dictionary-yawl.txt", std::ios::in);
    std::string word;
    if (dictionaryFile.is_open())
    {
        while (getline (dictionaryFile,word) )
        {
            insertWord(std::move(word));
        }
        dictionaryFile.close();
        return;
    }
    
    std::cout << "Unable to open file dictionary-yawl.txt" << std::endl;
    std::cout << "using default dictionary" << std::endl;
    
    
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
std::tuple<WordDictionary::FoundWord_t, WordDictionary::LeadingPrefixFound_t, std::string> WordDictionary::isInDictionary(std::string word) const {
    using std::reverse;
    
    auto wordsListIter = dictionary.find(hashTheWord(word));
    if (wordsListIter != dictionary.end()) {
        auto iter = wordsListIter->second->find(word);
        if (iter != wordsListIter->second->end()) {
            return {FoundWord_t::Found, LeadingPrefixFound_t::Found, std::move(word)};
        }
        else {
            return std::make_tuple<WordDictionary::FoundWord_t, WordDictionary::LeadingPrefixFound_t, std::string>(FoundWord_t::NotFound, LeadingPrefixFound_t::Found,{});
        }

#ifdef REVERSE_SEARCH
        // useful if we can figure out how to eliminate searching
        // from every node position.
        reverse(word.begin(), word.end());
        iter = wordsListIter->second.find(word);
        if (iter != wordsListIter->second.end()) {
            return {FoundWord_t::Found, LeadingPrefixFound_t::Found, std::move(word)};
        } else {
            return std::make_tuple<WordDictionary::FoundWord_t, WordDictionary::LeadingPrefixFound_t, std::string>(FoundWord_t::NotFound, LeadingPrefixFound_t::Found,{});    
        }
#endif // REVERSE_SEARCH
    }
    return std::make_tuple<WordDictionary::FoundWord_t, WordDictionary::LeadingPrefixFound_t, std::string>(FoundWord_t::NotFound,LeadingPrefixFound_t::NotFound,{});
}

void WordDictionary::printDictionary(std::ostream &os) const {
    os << "Dictionary contains: " << dictionary.size() << " words " << std::endl;
#ifdef DEBUG_DICTIONARY
    for (auto &wordList: dictionary) {
        for (auto word : *wordList.second) {
            std::cout << word << std::endl;
        }
    }
    os << "----------" << std::endl;
#endif
}
