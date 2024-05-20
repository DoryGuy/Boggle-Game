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
#include <cassert>


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

    const auto isWord = word.length() == 3 ? isWord_t::yes : isWord_t::no;
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
    const auto triIter = dictionary.find(hashKey);
    
    if (triIter == dictionary.end()) {
        using std::set;
        auto quad_dictionary_ptr = QuadInnerDictionaryPtr_t(new QuadInnerDictionary_t);
        
        if (word.length() == 3) {
            dictionary.emplace(std::move(hashKey), std::move(quad_dictionary_ptr));
        } else if (word.length() == 4) {
            auto quintDictionary_ptr = QuintInnerDictionaryPtr_t(new QuintInnerDictionary_t);
            subDictionaryKeyword<char> quadInnerKeyword(isWord_t::yes, word[3]);
            // connect the quad dictionary to the empty quint dictionary
            quad_dictionary_ptr->emplace(std::move(quadInnerKeyword), std::move(quintDictionary_ptr));
            
            // connect the tri dictionary to the quad dictionary
            dictionary.emplace(std::move(hashKey), std::move(quad_dictionary_ptr));
        } else if (word.length() == 5) {
            auto words_ptr = InnerDictionarySetPtr_t(new setWithMaxLength);
            
            subDictionaryKeyword<char> quintInnerKeyword(isWord_t::yes, word[4]);
            auto quintDictionary_ptr = QuintInnerDictionaryPtr_t(new QuintInnerDictionary_t);
            quintDictionary_ptr->emplace(std::move(quintInnerKeyword), std::move(words_ptr));
            
            subDictionaryKeyword<char> quadInnerKeyword(isWord_t::no, word[3]);
            // connect the quad dictionary to the quint dictionary with one entry.
            quad_dictionary_ptr->emplace(std::move(quadInnerKeyword), std::move(quintDictionary_ptr));
            
            // connect the tri dictionary to the quad dictionary
            dictionary.emplace(std::move(hashKey), std::move(quad_dictionary_ptr));
        }  else {
            auto words_ptr = InnerDictionarySetPtr_t(new setWithMaxLength);
            words_ptr->emplace(word.substr(5,word.length()));
            
            subDictionaryKeyword<char> quintInnerKeyword(isWord_t::no, word[4]);
            auto quint_dictionary_ptr = QuintInnerDictionaryPtr_t(new QuintInnerDictionary_t);
            quint_dictionary_ptr->emplace(std::move(quintInnerKeyword), std::move(words_ptr));
            
            subDictionaryKeyword<char> quadInnerKeyword(isWord_t::no, word[3]);
            // connect the quad dictionary to the quint dictionary with one entry.
            quad_dictionary_ptr->emplace(std::move(quadInnerKeyword), std::move(quint_dictionary_ptr));
    
            // connect the tri dictionary to the quad dictionary
            dictionary.emplace(std::move(hashKey), std::move(quad_dictionary_ptr));
        }
    }
    else {
        if (word.length() == 3) {
            if (triIter->first.isWord() == isWord_t::no) {
                triIter->first.setIsWord();
            }
        } else if (word.length() == 4) {
            auto quad_dictionary_ptr = triIter->second.get();
            subDictionaryKeyword<char> quadInnerKeyword(isWord_t::yes, word[3]);
            auto quad_dictionary_iter = quad_dictionary_ptr->find(quadInnerKeyword);
            if (quad_dictionary_iter == quad_dictionary_ptr->end()) {
                // the four letter word isn't here.
                auto quintDictionary_ptr = QuintInnerDictionaryPtr_t(new QuintInnerDictionary_t);
                quad_dictionary_ptr->emplace(std::move(quadInnerKeyword), std::move(quintDictionary_ptr));

            } else {
                if (quad_dictionary_iter->first.isWord() == isWord_t::no)
                {
                    quad_dictionary_iter->first.setIsWord();
                }
            }
        } else if (word.length() == 5) {
            auto quad_dictionary_ptr = triIter->second.get();
            subDictionaryKeyword<char> quadInnerKeyword(isWord_t::no, word[3]);
            auto quad_dictionary_iter = quad_dictionary_ptr->find(quadInnerKeyword);
            if (quad_dictionary_iter == quad_dictionary_ptr->end()) {
                // the four letter word isn't here. and thus neither is the 5 letter word.
                auto words_ptr = InnerDictionarySetPtr_t(new setWithMaxLength);
                
                auto quint_dictionary_ptr = QuintInnerDictionaryPtr_t(new QuintInnerDictionary_t);
                subDictionaryKeyword<char> quintInnerKeyword(isWord_t::yes, word[4]);
                quint_dictionary_ptr->emplace(std::move(quintInnerKeyword), std::move(words_ptr));
                
                quad_dictionary_ptr->emplace(std::move(quadInnerKeyword), std::move(quint_dictionary_ptr));
            } else {
                auto quint_dictionary_ptr = quad_dictionary_iter->second.get();
                subDictionaryKeyword<char> quintInnerKeyword(isWord_t::yes, word[4]);
                auto quint_dictionary_iter = quint_dictionary_ptr->find(quintInnerKeyword);
                
                if (quint_dictionary_iter == quint_dictionary_ptr->end() ) {
                    // the five letter word isn't here
                    auto words_ptr = InnerDictionarySetPtr_t(new setWithMaxLength);
                    quint_dictionary_ptr->emplace(std::move(quintInnerKeyword), std::move(words_ptr));
                } else {
                    if (quint_dictionary_iter->first.isWord() == isWord_t::no) {
                        quint_dictionary_iter->first.setIsWord();
                    }
                }
            }
        } else {
            auto quad_dictionary_ptr = triIter->second.get();
            subDictionaryKeyword<char> quadInnerKeyword(isWord_t::no, word[3]);
            auto quad_dictionary_iter = quad_dictionary_ptr->find(quadInnerKeyword);
            if (quad_dictionary_iter == quad_dictionary_ptr->end()) {
                // the four letter word isn't here, so neither is the 5 letter root.

                auto words_ptr = InnerDictionarySetPtr_t(new setWithMaxLength);
                auto quint_dictionary_ptr = QuintInnerDictionaryPtr_t(new QuintInnerDictionary_t);
                subDictionaryKeyword<char> quintInnerKeyword(isWord_t::no, word[4]);
                words_ptr->emplace(word.substr(5, word.length()));
                quint_dictionary_ptr->emplace(std::move(quintInnerKeyword), std::move(words_ptr));
                
                quad_dictionary_ptr->emplace(quadInnerKeyword, std::move(quint_dictionary_ptr));
            } else {
                auto quint_dictionary_ptr = quad_dictionary_iter->second.get();
                subDictionaryKeyword<char> quintInnerKeyword(isWord_t::no, word[4]);
                auto quint_dictionary_iter = quint_dictionary_ptr->find(quintInnerKeyword);
                
                if (quint_dictionary_iter == quint_dictionary_ptr->end() ) {
                    // the five letter root isn't here
                    auto words_ptr = InnerDictionarySetPtr_t(new setWithMaxLength);
                    auto stored_word = word.substr(5, word.length());
                    words_ptr->emplace(std::move(stored_word));
                    quint_dictionary_ptr->emplace(std::move(quintInnerKeyword), std::move(words_ptr));
                } else {
                    auto words_ptr = quint_dictionary_iter->second.get();
                    auto stored_word = word.substr(5, word.length());
                    if (words_ptr->find(stored_word) == words_ptr->end()) {
                        words_ptr->emplace(std::move(stored_word));
                    }
                }
            }
        }
    }
}

WordDictionary::WordDictionary() {
    // fill it with words...
    std::ifstream dictionaryFile;
    dictionaryFile.open("/Users/garypowell/QuizQuestions/Boggle-Game/dictionary/dictionary-yawl.txt", std::ios::in);
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
    
    
    constexpr static char const* wordList[] = {
     "test"
    ,"army"
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
    ,"fortify"
    ,"fortifying"
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
        insertWord(word);
    }
}

// check for both forward spelling and reverse spelling.
std::tuple<WordDictionary::FoundWord_t, WordDictionary::LeadingPrefixFound_t, std::string>
WordDictionary::isInDictionary(std::string word) const {
    using std::reverse;

    const auto hashkey = hashTheWord(word);
    const auto triIter = dictionary.find(hashkey);
    if (triIter != dictionary.end()) {
        if (word.length() == 3) {
            const auto more_prefixes = triIter->second->empty() ? LeadingPrefixFound_t::NotFound : LeadingPrefixFound_t::Found;
            if (triIter->first.isWord() == isWord_t::yes) {
            // 3 letter word found.
                return {FoundWord_t::Found, more_prefixes, std::move(word)};
            }
            return {FoundWord_t::NotFound, more_prefixes,std::string()};
        }
        
        subDictionaryKeyword<char> quadInnerKeyword(isWord_t::no, word[3]);
        const auto quad_dictionary_ptr = triIter->second.get();
        const auto quad_dictionary_iter = quad_dictionary_ptr->find(quadInnerKeyword);
        if (quad_dictionary_iter != quad_dictionary_ptr->end()) {
            if (word.length() == 4) {
                const auto more_prefixes = quad_dictionary_iter->second->empty() ? LeadingPrefixFound_t::NotFound : LeadingPrefixFound_t::Found;
                if (quad_dictionary_iter->first.isWord() == isWord_t::yes) {
                    return {FoundWord_t::Found, more_prefixes, std::move(word)};
                }
                
                return {FoundWord_t::NotFound,more_prefixes,std::string()};
            }
            
            if (word.length() == 5) {
                subDictionaryKeyword<char> quintInnerKeyword(isWord_t::yes, word[4]);
                const auto quint_dictionary_ptr = quad_dictionary_iter->second.get();
                const auto quint_dictionary_iter = quint_dictionary_ptr->find(quintInnerKeyword);
                if (quint_dictionary_iter != quint_dictionary_ptr->end() ) {
                    const auto more_prefixes = quad_dictionary_iter->second->empty() ? LeadingPrefixFound_t::NotFound : LeadingPrefixFound_t::Found;
                    if (quint_dictionary_iter->first.isWord() == isWord_t::yes) {
                        return {FoundWord_t::Found, more_prefixes, std::move(word)};
                    }
                    return {FoundWord_t::NotFound, more_prefixes, std::string()};
                }
                return {FoundWord_t::NotFound, LeadingPrefixFound_t::NotFound, std::string()};
            }
            

            const auto quint_dictionary_ptr = quad_dictionary_iter->second.get();
            subDictionaryKeyword<char> quintInnerKeyword(isWord_t::no, word[4]);
            const auto quint_dictionary_iter = quint_dictionary_ptr->find(quintInnerKeyword);
            if (quint_dictionary_iter != quint_dictionary_ptr->end() ) {
                
                const auto endOfWord = word.substr(5,word.length());
                const auto word_list = quint_dictionary_iter->second.get();
                size_t max_wordLength_in_word_list(word_list->getMaxLength());
                
                auto more_prefixes = (word.length() > max_wordLength_in_word_list) ? LeadingPrefixFound_t::NotFound : LeadingPrefixFound_t::Found;
                const auto word_list_iter = word_list->find(endOfWord);
                if (word_list_iter != word_list->end()) {
                    if (word_list->size() == 1) {
                        // this is the only word here, so stop looking.
                        more_prefixes = LeadingPrefixFound_t::NotFound;
                    }
                    return {FoundWord_t::Found, more_prefixes, std::move(word)};
                }

                return {FoundWord_t::NotFound, more_prefixes, std::string()};
            }
            return {FoundWord_t::NotFound, LeadingPrefixFound_t::NotFound, std::string()};

        }

    }

    return {FoundWord_t::NotFound,LeadingPrefixFound_t::NotFound,std::string()};
}

std::ostream & WordDictionary::printDictionary(std::ostream &os) const {
    using std::endl;

    std::size_t word_count = 0;
    os << "Dictionary contains: " << dictionary.size() << " tri's" << endl;
    std::size_t quad_count = 0;
    std::size_t quint_count = 0;
    for (auto &triList: dictionary) {
        if (triList.first.isWord() == isWord_t::yes) {
            ++word_count;
        }
        quad_count += triList.second->size();
        for (auto &quad_list: *triList.second.get()) {
            if (quad_list.first.isWord() == isWord_t::yes) {
                ++word_count;
            }
            
            quint_count += quad_list.second->size();
            for (auto &word_list: *quad_list.second.get()) {
                if (word_list.first.isWord() == isWord_t::yes) {
                    ++word_count;
                }
                word_count += word_list.second.get()->size();
            }
        }
    }
    os << "Dictionary contains: " << quad_count << " quad's" << endl;
    os << "Dictionary contains: " << quint_count << " quint's" << endl;

    os << "Dictionary contains: " << word_count << " words" << endl;

#define FDEBUG_DICTIONARY
#ifdef DEBUG_DICTIONARY
    for (auto &triList: dictionary) {
        if (triList.first.isWord()) {
            os << triList.first.keyWord() << endl;
        }
        for (auto &quad_list: *triList.second.get()) {
            if (quad_list.first.isWord()) {
                os << triList.first.keyWord() << quad_list.first.keyWord() << endl;
            }
            for (auto &word_list: *quad_list.second.get()) {
                if (word_list.first.isWord()) {
                    os << triList.first.keyWord() << quad_list.first.keyWord() <<word_list.first.keyWord() << endl;
                }
                for (auto &word: *word_list.second.get()) {
                    os << triList.first.keyWord() << quad_list.first.keyWord() << word_list.first.keyWord() << word << endl;
                }
            }
        }
    }
    os << "----------" << endl;
#endif
    return os;
}
