#include <time.h>
#include <string>
#include <vector>
#include <map>
#include "pub/src/base/common/logging.h"
#include "sandbox/zheshan.lx/problem3/include/errno.h"
#include "sandbox/zheshan.lx/problem3/include/send_to_bin.h"

using std::string;
using std::vector;
using std::map;
using std::pair;
using std::endl;

namespace wolong {

SendToBin::SendToBin() :
    m_summary() {
}
    
SendToBin::~SendToBin() {
}

int SendToBin::run(map<string, long> &statistics,
                   vector<vector<pair<string, long> > > &bins,
                   int num_bins) {
    int ret = SUCCESS;
    struct timespec ts1 = {0, 0};
    struct timespec ts2 = {0, 0};
    double exec_ns = 0.0;
    
    clock_gettime(CLOCK_MONOTONIC, &ts1);

    // Make sure no less than 1 bins will be used.
    if (num_bins < 1) {
        LOG(WARNING) << "Less than 1(" << num_bins << ") bins required, set 1 instead." << endl;
        num_bins = 1;
    }

    // Perpare enough bins
    bins.resize(num_bins);

    // Send data to bins, terms in bins[i] always has greater frequency than terms in bins[j] when i < j;
    // Assume all terms's frequency is greater than 0
    map<string, long>::iterator iter;
    for (iter = statistics.begin(); iter != statistics.end(); ++iter) {
        if (iter->second < num_bins) {
            bins[num_bins - iter->second].push_back(*iter);
        } else {
            bins[0].push_back(*iter);
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &ts2);
    exec_ns = 1e9 * (ts2.tv_sec - ts1.tv_sec) + (ts2.tv_nsec - ts1.tv_nsec);

    // Record performence
    m_summary.record(exec_ns, statistics.size());
    
    return ret;
}

} // namespace wolong

