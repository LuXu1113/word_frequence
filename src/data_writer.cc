#include <time.h>
#include <string>
#include <vector>
#include <map>
#include "pub/src/base/common/logging.h"
#include "sandbox/zheshan.lx/problem3/include/errno.h"
#include "sandbox/zheshan.lx/problem3/include/data_writer.h"

using std::string;
using std::vector;
using std::map;
using std::pair;
using std::endl;

namespace wolong {

DataWriter::DataWriter() :
    m_fp(NULL),
    m_summary() {
}
    
DataWriter::~DataWriter() {
    if (NULL != m_fp) {
        fclose(m_fp);
    }
}

// Set file that to write
int DataWriter::set_file(const char *path) {
    int ret = SUCCESS;

    do {
        // Invalid parameter
        if (NULL == path) {
            LOG(ERROR) << "Input path is NULL." << endl;
            ret = NULL_POINTER_ERROR;
            break;
        }

        // Change output file
        if (NULL != m_fp) {
            fclose(m_fp);
            LOG(INFO) << "Close file \"" << m_path << "\"." << endl;
            m_fp = NULL;
            m_path = "";
        }

        // Open output file
        m_fp = fopen(path, "w+");

        // Fail to open
        if (NULL == m_fp) {
            LOG(ERROR) << "Encounter an error when open file \"" << path << "\"." << endl;
            ret = OPEN_FILE_ERROR;
            break;
        }

        // Record the success
        m_path = path;
        LOG(INFO) << "Open file \"" << m_path << "\"." << endl;

    } while(0);

    return ret;
}

// Write sorted terms to output file
int DataWriter::write(const vector<pair<string, long> > &results) {
    int ret = SUCCESS;
    struct timespec ts1 = {0, 0};
    struct timespec ts2 = {0, 0};
    double exec_ns = 0.0;

    clock_gettime(CLOCK_MONOTONIC, &ts1);

    for(unsigned int i = 0; i < results.size(); ++i) {
        fprintf(m_fp, "%s\t%ld\n", results[i].first.c_str(), results[i].second);
    }

    clock_gettime(CLOCK_MONOTONIC, &ts2);
    exec_ns = 1e9 * (ts2.tv_sec - ts1.tv_sec) + (ts2.tv_nsec - ts1.tv_nsec);

    m_summary.record(exec_ns, results.size());
    
    return ret;
}

// Write the percentage of basic words to output file
int DataWriter::write(const double results) {
    int ret = SUCCESS;
    struct timespec ts1 = {0, 0};
    struct timespec ts2 = {0, 0};
    double exec_ns = 0.0;

    clock_gettime(CLOCK_MONOTONIC, &ts1);

    fprintf(m_fp, "Percentage: %lf %%\n", results * 100);

    clock_gettime(CLOCK_MONOTONIC, &ts2);
    exec_ns = 1e9 * (ts2.tv_sec - ts1.tv_sec) + (ts2.tv_nsec - ts1.tv_nsec);

    m_summary.record(exec_ns, sizeof(double) + 15);
    
    return ret;
}

// Write the basic words to output file
int DataWriter::write(const map<string, long> &results) {
    int ret = SUCCESS;
    struct timespec ts1 = {0, 0};
    struct timespec ts2 = {0, 0};
    double exec_ns = 0.0;

    clock_gettime(CLOCK_MONOTONIC, &ts1);

    for(map<string, long>::const_iterator iter = results.begin(); results.end() != iter; ++iter) {
        fprintf(m_fp, "%s\n", iter->first.c_str());
    }

    clock_gettime(CLOCK_MONOTONIC, &ts2);
    exec_ns = 1e9 * (ts2.tv_sec - ts1.tv_sec) + (ts2.tv_nsec - ts1.tv_nsec);

    m_summary.record(exec_ns, results.size());
    
    return ret;
}

} // namespace wolong


