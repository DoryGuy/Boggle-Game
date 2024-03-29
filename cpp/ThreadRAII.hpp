//
//  ThreadRAII.hpp
//  BoggleMultiProcess
//
//  Created by Gary Powell on 7/5/19.
//  Copyright © 2019 Guest User. All rights reserved.
//

#ifndef ThreadRAII_hpp
#define ThreadRAII_hpp

#include <thread>

// The purpose of this class is to guarantee that threads will always be cleanly
// destructed, either join, or detach will be called.
// code from "Effective Modern C++" by Scott Meyers

class ThreadRAII {
public:
    enum class DtorAction { join, detach };
    
    explicit ThreadRAII(std::thread&& t, DtorAction a)
    : m_action(a)
    , m_t(std::move(t))
    {}
    
    ~ThreadRAII() {
        try {
            if (m_t.joinable() ) {
                if (m_action == DtorAction::join) {
                    m_t.join();
                } else {
                    m_t.detach();
                }
            }
        } catch(...) {
            // don't crash
        }
    }
    
    // no copies, as we can't copy a thread object.
    ThreadRAII(ThreadRAII &) = delete;
    ThreadRAII(ThreadRAII const &) = delete;
    ThreadRAII& operator=(ThreadRAII &) = delete;
    ThreadRAII& operator=(ThreadRAII const &) = delete;
    
    // we can move it though
    ThreadRAII(ThreadRAII&&) = default;
    ThreadRAII& operator=(ThreadRAII&&) = default;
    
    [[nodiscard]] bool joinable() { return m_t.joinable(); }
    void join() { m_t.join(); }
    
private:
    DtorAction m_action;
    std::thread m_t;
};


#endif /* ThreadRAII_hpp */
