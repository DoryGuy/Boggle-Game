//
//  JThreadRAII.hpp
//  BoggleMultiProcess
//
//  Created by Gary Powell on 7/5/19.
//  Copyright © 2019 Guest User. All rights reserved.
//

#ifndef JThreadRAII_hpp
#define JThreadRAII_hpp

#include <thread>
#include <utility> // std::move

// The purpose of this class is to guarantee that threads will always be cleanly
// destructed, either join, or detach will be called.
// code from "Effective Modern C++" by Scott Meyers
// upgraded to use jthread

class JThreadRAII {
public:
    enum class DtorAction { join, detach };
    
    explicit JThreadRAII(std::jthread&& t, DtorAction a) noexcept
    : m_action(a)
    , m_t(std::move(t))
    {}
    
    ~JThreadRAII() {
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
    JThreadRAII(JThreadRAII &) = delete;
    JThreadRAII(JThreadRAII const &) = delete;
    JThreadRAII& operator=(JThreadRAII &) = delete;
    JThreadRAII& operator=(JThreadRAII const &) = delete;
    
    // we can move it though
    JThreadRAII(JThreadRAII&&) noexcept = default;
    [[nodiscard]] JThreadRAII& operator=(JThreadRAII&&) noexcept = default;
    
    [[nodiscard]] bool joinable() { return m_t.joinable(); }
    void join() { m_t.join(); }
    void swap(JThreadRAII& t) noexcept {
        using std::swap;
        swap(m_action, t.m_action);
        swap(m_t, t.m_t);
    }
    
private:
    DtorAction m_action;
    std::jthread m_t;
};


#endif /* JThreadRAII_hpp */
