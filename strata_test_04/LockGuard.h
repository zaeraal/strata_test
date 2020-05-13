#ifndef LOCKGUARD_H
#define LOCKGUARD_H

#include <mutex>

namespace strata {

struct adopt_lock_t { explicit adopt_lock_t() = default; };

template <class Mutex>
class lock_guard {
public:
    explicit lock_guard(Mutex& m): m_mutex(m)      { m_mutex.lock(); }
    lock_guard(Mutex& m, adopt_lock_t): m_mutex(m) { }
    ~lock_guard()                                  { m_mutex.unlock(); }
    lock_guard(lock_guard const&) = delete;
    lock_guard& operator=(lock_guard const&) = delete;
private:
    Mutex& m_mutex;
};

} // namespace

#endif // LOCKGUARD_H
