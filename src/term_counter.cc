#include <time.h>
#include <string>
#include <map>
#include "pub/src/base/common/logging.h"
#include "sandbox/zheshan.lx/problem3/include/errno.h"
#include "sandbox/zheshan.lx/problem3/include/term_counter.h"

using std::string;
using std::vector;
using std::map;
using std::pair;
using std::endl;

namespace wolong {

TermCounter::TermCounter() :
    m_summary() {
    pthread_mutex_init(&m_mutex, 0);
}
    
TermCounter::~TermCounter() {
    pthread_mutex_destroy(&m_mutex);
}

// Add one term to statistics
int TermCounter::add_terms(string &query,
                           vector<string> &terms,
                           map<string, long> &basic_words,
                           map<string, long> &statistics) {
    int ret = SUCCESS;
    struct timespec ts1 = {0, 0};
    struct timespec ts2 = {0, 0};
    double exec_ns = 0.0;
    map<string, long>::iterator iter;
    
    clock_gettime(CLOCK_MONOTONIC, &ts1);
    pthread_mutex_lock(&(m_mutex));

    // This term is a basic words
    if (terms.size() == 1 && terms[0] == query) {
        iter = basic_words.find(terms[0]);
        if (basic_words.end() != iter) {
            ++(iter->second);
        } else {
            basic_words.insert(pair<string, long>(terms[0], 1));
        }
    }

    // Using a map to count term frequency
    for(unsigned int i = 0; i < terms.size(); ++i) {
        iter = statistics.find(terms[i]);
        if (statistics.end() != iter) {
            ++(iter->second);
        } else {
            statistics.insert(pair<string, long>(terms[i], 1));
        }
    }

    pthread_mutex_unlock(&(m_mutex));
    clock_gettime(CLOCK_MONOTONIC, &ts2);
    exec_ns = 1e9 * (ts2.tv_sec - ts1.tv_sec) + (ts2.tv_nsec - ts1.tv_nsec);

    // Record performence
    m_summary.record(exec_ns, 1);
    
    return ret;
}

} // namespace wolong


