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
#include <utility> // std::move, std::swap

// The purpose of this class is to guarantee that threads will always be cleanly
// destructed, either join, or detach will be called.
// code from "Effective Modern C++" by Scott Meyers

class ThreadRAII {
public:
    enum class DtorAction { join, detach };
    
    explicit ThreadRAII(std::thread&& t, DtorAction a) noexcept
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
    ThreadRAII(ThreadRAII&&) noexcept = default;
    [[nodiscard]] ThreadRAII& operator=(ThreadRAII&&) noexcept = default;
    
    [[nodiscard]] bool joinable() { return m_t.joinable(); }
    void join() { m_t.join(); }
    void swap(ThreadRAII& t) noexcept {
        using std::swap;
        swap(m_action, t.m_action);
        swap(m_t, t.m_t);
    }
    
private:
    DtorAction m_action;
    std::thread m_t;
};


#endif /* ThreadRAII_hpp */
