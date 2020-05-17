#include <time.h>
#include <string>
#include "pub/src/base/common/logging.h"
#include "wolong/proto/ad_log.pb.h"
#include "base/strings/string_util.h"
#include "base/encoding/line_escape.h"
#include "sandbox/zheshan.lx/problem3/include/errno.h"
#include "sandbox/zheshan.lx/problem3/include/summary.h"
#include "sandbox/zheshan.lx/problem3/include/data_processor.h"

using std::string;
using std::endl;
using std::vector;

namespace wolong {

DataProcessor::DataProcessor() :
    m_summary() {
}

DataProcessor::~DataProcessor() {
}

// Get query field string from one piece of log
int DataProcessor::get_query(string &line, vector<string> &query) {
    int ret = SUCCESS;
    struct timespec ts1 = {0, 0};
    struct timespec ts2 = {0, 0};
    double exec_ns = 0.0;

    clock_gettime(CLOCK_MONOTONIC, &ts1);
    do {
        string unescaped_line;

        // Unescape line
        base::TrimTrailingWhitespaces(&line);
        if (false == base::LineUnescape(line, &unescaped_line)) {
            LOG(ERROR) << "Unexcape line error." << endl;
            ret = LINE_UNESCAPE_ERROR;
            break;
        }

        query.clear();

        // Convert log to proto-buffer structure
        SessionLog session_log;
        session_log.ParseFromString(unescaped_line);

        // Find and save contents in query filed
        int search_log_size = session_log.search_log_size();
        for (int i = 0; i < search_log_size; ++i) {
            const SearchLog &search_log = session_log.search_log(i);

            if (search_log.has_query_info()) {
                const QueryInfo &query_info = search_log.query_info();

                if (query_info.has_query()) {
                    query.push_back(query_info.query());
                }
            }
        }
    } while(0);
    clock_gettime(CLOCK_MONOTONIC, &ts2);
    exec_ns = 1e9 * (ts2.tv_sec - ts1.tv_sec) + (ts2.tv_nsec - ts1.tv_nsec);

    // Reord performence
    m_summary.record(exec_ns, 1);

    return ret;
}

} // namespace wolong

