#include <pthread.h>
#include <string>
#include <vector>
#include <map>
#include "sandbox/zheshan.lx/problem3/include/summary.h"

#ifndef WLY_SANDBOX_ZHESHANLX_PROBLEM3_INCLUDE_TERMCOUNTER_H_
#define WLY_SANDBOX_ZHESHANLX_PROBLEM3_INCLUDE_TERMCOUNTER_H_

namespace wolong {
    
class TermCounter {
    public:
        TermCounter();
        ~TermCounter();

        int add_terms(std::string &query,
                      std::vector<std::string> &terms,
                      std::map<std::string, long> &basic_words,
                      std::map<std::string, long> &statistics);

        inline const Summary &get_summary() const {
            return m_summary;
        }

    private:
        pthread_mutex_t m_mutex;
        Summary m_summary;
};

} // namespace wolong

#endif // WLY_SANDBOX_ZHESHANLX_PROBLEM3_INCLUDE_TERMCOUNTER_H_

