#include <time.h>
#include <string>
#include "pub/src/base/common/logging.h"
#include "nlp/segment/segmenter.h"
#include "sandbox/zheshan.lx/problem3/include/errno.h"
#include "sandbox/zheshan.lx/problem3/include/summary.h"
#include "sandbox/zheshan.lx/problem3/include/query_segmenter.h"

using std::string;
using std::vector;
using std::endl;

namespace wolong {

QuerySegmenter::QuerySegmenter() :
    m_segmenter(),
    m_summary() {
    pthread_mutex_init(&m_mutex, 0);
}
    
QuerySegmenter::~QuerySegmenter() {
    pthread_mutex_destroy(&m_mutex);
}

// Split query to terms:
int QuerySegmenter::split_query(const string &query, vector<string> &terms) {
    int ret = SUCCESS;
    struct timespec ts1 = {0, 0};
    struct timespec ts2 = {0, 0};
    double exec_ns = 0.0;
    
    terms.clear();

    clock_gettime(CLOCK_MONOTONIC, &ts1);
    pthread_mutex_lock(&(m_mutex));
    if (false == m_segmenter.SegmentS(query, &terms)) {
        LOG(ERROR) << "Split query error." << endl;
        ret = SEGMENTER_ERROR;
    }

    pthread_mutex_unlock(&(m_mutex));
    clock_gettime(CLOCK_MONOTONIC, &ts2);

    exec_ns = 1e9 * (ts2.tv_sec - ts1.tv_sec) + (ts2.tv_nsec - ts1.tv_nsec);
    m_summary.record(exec_ns, 1);

    return ret;
}

} // namespace wolong

