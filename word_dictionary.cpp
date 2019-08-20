//
//  dictionary.cpp
//  BoggleMultiProcess
//
//  Created by Gary Powell on 7/5/19.
//  Copyright © 2019 Guest User. All rights reserved.
//

#include "word_dictionary.hpp"
#include "GameConstants.hpp"
#include <algorithm>
#include <iostream>
#include <fstream>


//subDictionaryKeyword<std::string> foo1;
//subDictionaryKeyword<char> foo2;

//#define REVERSE_SEARCH 1

// a hash for our dictionary.
// specializations for the hash of the two types we are using, char and std::string
template<>
size_t subDictionaryKeyword<char>::operator()(const subDictionaryKeyword<char>& k) const { return static_cast<size_t>(k.keyWord()); }

template<>
size_t subDictionaryKeyword<std::string>::operator()(const subDictionaryKeyword<std::string>& k) const { return std::hash<std::string>{}(k.keyWord()); }

// return just the first three letters of the word.
subDictionaryKeyword<std::string> hashTheWord(std::string word) {
    auto isWord = word.length() == 3 ? isWord_t::yes : isWord_t::no;
    return subDictionaryKeyword<std::string>(isWord, word.substr(0, 3));
}

void WordDictionary::insertWord(std::string word) {
    using std::transform;
    using std::string;
    using std::unique_ptr;
    using std::set;

    // no point in having a dictionary with words that won't score.
    if (word.length() < 3 || word.length() > max_board_elements) {
        return;
    }
    transform(word.begin(), word.end(), word.begin(), ::tolower);
    auto hashKey = hashTheWord(word);
    auto triIter = dictionary.find(hashKey);
    
    if (triIter == dictionary.end()) {
        using std::set;
        auto inner_dictionary_ptr = InnerDictionaryPtr_t(new InnerDictionary_t);
        
        if (word.length() == 3) {
            dictionary.emplace(hashKey, std::move(inner_dictionary_ptr));
        } else if (word.length() == 4) {
            auto words_ptr = InnerDictionarySetPtr_t(new set<string>);
            subDictionaryKeyword<char> innerKeyword(isWord_t::yes, word[3]);
            inner_dictionary_ptr->emplace(innerKeyword, std::move(words_ptr));
            dictionary.emplace(hashKey, std::move(inner_dictionary_ptr));
        } else {
            auto words_ptr = InnerDictionarySetPtr_t(new set<string>);
            subDictionaryKeyword<char> innerKeyword(isWord_t::no, word[3]);
            words_ptr->insert(word.substr(4,word.length()));
            inner_dictionary_ptr->emplace(innerKeyword, std::move(words_ptr));
            dictionary.emplace(hashKey, std::move(inner_dictionary_ptr));
        }
    }
    else {
        if (word.length() == 3) {
            if (triIter->first.isWord() == isWord_t::no) {
                triIter->first.setIsWord();
            }
        } else if (word.length() == 4) {
            auto four_letter_words_ptr = triIter->second.get();
            subDictionaryKeyword<char> innerKeyword(isWord_t::yes, word[3]);
            auto inner_dictionary_iter = four_letter_words_ptr->find(innerKeyword);
            if (inner_dictionary_iter == four_letter_words_ptr->end()) {
                // the four letter word isn't here.
                auto words_ptr = InnerDictionarySetPtr_t(new set<string>);
                four_letter_words_ptr->emplace(innerKeyword, std::move(words_ptr));

            } else {
                if (inner_dictionary_iter->first.isWord() == isWord_t::no)
                {
                    inner_dictionary_iter->first.setIsWord();
                }
            }
        } else {
            auto four_letter_words_ptr = triIter->second.get();
            subDictionaryKeyword<char> innerKeyword(isWord_t::no, word[3]);
            auto inner_dictionary_iter = four_letter_words_ptr->find(innerKeyword);
            if (inner_dictionary_iter == four_letter_words_ptr->end()) {
                // the four letter word isn't here.
                subDictionaryKeyword<char> innerKeyword(isWord_t::no, word[3]);
                auto words_ptr = InnerDictionarySetPtr_t(new set<string>);
                words_ptr->emplace(word.substr(4, word.length()));
                four_letter_words_ptr->emplace(innerKeyword, std::move(words_ptr));
            } else {
                auto words_ptr = inner_dictionary_iter->second.get();
                auto stored_word = word.substr(4, word.length());
                if (words_ptr->end() == words_ptr->find(stored_word)) {
                    words_ptr->emplace(std::move(stored_word));
                }
            }
        }
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
std::tuple<WordDictionary::FoundWord_t, WordDictionary::LeadingPrefixFound_t, std::string>
WordDictionary::isInDictionary(std::string word) const {
    using std::reverse;

    auto hashkey = hashTheWord(word);
    auto triIter = dictionary.find(hashkey);
    if (triIter != dictionary.end()) {
        if (word.length() == 3) {
            if (triIter->first.isWord()) {
            // 3 letter word found.
                return {FoundWord_t::Found, LeadingPrefixFound_t::Found, std::move(word)};
            }
            return std::make_tuple<WordDictionary::FoundWord_t, WordDictionary::LeadingPrefixFound_t, std::string>(FoundWord_t::NotFound,LeadingPrefixFound_t::Found,{});
        }
        
        subDictionaryKeyword<char> innerKeyword(isWord_t::no, word[3]);
        auto four_letter_words_ptr = triIter->second.get();
        auto inner_dictionary_iter = four_letter_words_ptr->find(innerKeyword);
        if (inner_dictionary_iter != four_letter_words_ptr->end()) {
            if (word.length() == 4) {
                if (inner_dictionary_iter->first.isWord()) {
                    return {FoundWord_t::Found, LeadingPrefixFound_t::Found, std::move(word)};
                }
                
                return std::make_tuple<WordDictionary::FoundWord_t, WordDictionary::LeadingPrefixFound_t, std::string>(FoundWord_t::NotFound,LeadingPrefixFound_t::Found,{});
            }
            
            auto endOfWord = word.substr(4,word.length());
            auto set_iter = inner_dictionary_iter->second->find(endOfWord);
            if (set_iter != inner_dictionary_iter->second->end()) {
                return {FoundWord_t::Found, LeadingPrefixFound_t::Found, std::move(word)};
            }
        
            return std::make_tuple<WordDictionary::FoundWord_t, WordDictionary::LeadingPrefixFound_t, std::string>(FoundWord_t::NotFound, LeadingPrefixFound_t::Found,{});
        }

    }

    return std::make_tuple<WordDictionary::FoundWord_t, WordDictionary::LeadingPrefixFound_t, std::string>(FoundWord_t::NotFound,LeadingPrefixFound_t::NotFound,{});
}

void WordDictionary::printDictionary(std::ostream &os) const {
    using std::endl;

    int wordCount = 0;
    os << "Dictionary contains: " << dictionary.size() << " tri's " << endl;
    int quadCount = 0;
    for (auto &triList: dictionary) {
        if (triList.first.isWord()) {
            ++wordCount;
        }
        quadCount += triList.second->size();
        for (auto &wordList: *triList.second.get()) {
            if (wordList.first.isWord()) {
                ++wordCount;
            }
            wordCount += wordList.second->size();
        }
    }
    os << "Dictionary contains: " << quadCount << " quad's " << endl;

    os << "Dictionary contains: " << wordCount << " words " << endl;

#ifdef DEBUG_DICTIONARY
    for (auto &triList: dictionary) {
        if (triList.first.isWord()) {
            os << triList.first.keyWord() << endl;
        }
        for (auto &wordList: *triList.second.get()) {
            if (wordList.first.isWord()) {
                os << triList.first.keyWord() << wordList.first.keyWord() << endl;
            }
            for (auto &word: *wordList.second.get()) {
                os << triList.first.keyWord() << wordList.first.keyWord() << word << endl;
            }

        }
    }
    os << "----------" << endl;
#endif
}
