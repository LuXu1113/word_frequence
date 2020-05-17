#include <time.h>
#include <string>
#include <vector>
#include <map>
#include "pub/src/base/common/logging.h"
#include "sandbox/zheshan.lx/problem3/include/errno.h"
#include "sandbox/zheshan.lx/problem3/include/merge.h"

using std::string;
using std::vector;
using std::pair;
using std::endl;

namespace wolong {

Merge::Merge() :
    m_summary() {
}
    
Merge::~Merge() {
}

int Merge::run(vector<vector<pair<string, long> > > &bins,
               vector<pair<string, long> > &results) {
    int ret = SUCCESS;
    struct timespec ts1 = {0, 0};
    struct timespec ts2 = {0, 0};
    double exec_ns = 0.0;

    clock_gettime(CLOCK_MONOTONIC, &ts1);
    results.clear();

    // Merge all vectors in bins to one vector one by one
    for(unsigned int i = 0; i < bins.size(); ++i) {
        results.insert(results.end(), bins[i].begin(), bins[i].end());
    }

    clock_gettime(CLOCK_MONOTONIC, &ts2);
    exec_ns = 1e9 * (ts2.tv_sec - ts1.tv_sec) + (ts2.tv_nsec - ts1.tv_nsec);

    m_summary.record(exec_ns, results.size());
    
    return ret;
}

} // namespace wolong

