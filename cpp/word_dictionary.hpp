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

enum isWord_t { no, yes};

template<typename KeyWord_t>
class subDictionaryKeyword {
public:
    subDictionaryKeyword(isWord_t isWord = isWord_t::no, KeyWord_t keyWord = {})
    : m_isWord(isWord)
    , m_keyWord(keyWord)
    {}
    
    subDictionaryKeyword(subDictionaryKeyword const & rhs) = default;
    subDictionaryKeyword(subDictionaryKeyword &&) = default;
    subDictionaryKeyword & operator=(subDictionaryKeyword &&) = default;
    subDictionaryKeyword & operator=(subDictionaryKeyword const &) = default;
    
    constexpr isWord_t isWord() const { return m_isWord;}
    void setIsWord() const { m_isWord = yes; };
    constexpr KeyWord_t const & keyWord() const { return m_keyWord; }
    // hash fn
    size_t operator()(const subDictionaryKeyword& k) const;
    // comparison fn as the compiler can't seem to find operator()==
    bool operator()(const subDictionaryKeyword& lhs, const subDictionaryKeyword& rhs) const
        { return lhs == rhs; }
    constexpr bool operator==(subDictionaryKeyword const &rhs) const { return m_keyWord == rhs.m_keyWord; }
private:
    mutable isWord_t m_isWord; // doesn't change the hash value or the comparison fn result.
    KeyWord_t m_keyWord;
};

class setWithMaxLength : public std::set<std::string>
{
public:
    using inherited = std::set<std::string>;
    
    setWithMaxLength()
    : m_max_length(0)
    {}
    
    // overload emplace so that we track the max length word in the set.
    void emplace(std::string &&rhs) {
        newLength(rhs.length());
        inherited::emplace(std::move(rhs));
    }
    
    constexpr size_t getMaxLength() const { return 5 + m_max_length;}
private:
    constexpr void newLength(size_t x) {
        if (x > m_max_length ) {
            m_max_length = x;
        }
    }
    
    size_t m_max_length;
};



class WordDictionary {
private:
    using subDictionaryKeywordChar_t = subDictionaryKeyword<char>;
    using InnerDictionarySetPtr_t = std::unique_ptr<setWithMaxLength>;
    
    using QuintInnerDictionary_t = std::unordered_map<subDictionaryKeywordChar_t, InnerDictionarySetPtr_t,subDictionaryKeywordChar_t, subDictionaryKeywordChar_t>;
    using QuintInnerDictionaryPtr_t = std::unique_ptr<QuintInnerDictionary_t>;
    
    using QuadInnerDictionary_t = std::unordered_map<subDictionaryKeywordChar_t, QuintInnerDictionaryPtr_t,subDictionaryKeywordChar_t, subDictionaryKeywordChar_t>;
    
    using QuadInnerDictionaryPtr_t = std::unique_ptr<QuadInnerDictionary_t>;
    using subDictionaryKeywordString_t = subDictionaryKeyword<std::string>;
    using Dictionary_t = std::unordered_map<subDictionaryKeywordString_t, QuadInnerDictionaryPtr_t, subDictionaryKeywordString_t,subDictionaryKeywordString_t>;
 
    Dictionary_t dictionary;
    
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
