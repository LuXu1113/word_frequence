#include <time.h>
#include <string>
#include "pub/src/base/common/logging.h"
#include "sandbox/zheshan.lx/problem3/include/errno.h"
#include "sandbox/zheshan.lx/problem3/include/data_reader.h"

using std::string;
using std::endl;

namespace wolong {

DataReader::DataReader() :
    m_fp(NULL),
    m_summary() {
    pthread_mutex_init(&m_mutex, 0);
}
    
DataReader::~DataReader() {
    pthread_mutex_destroy(&m_mutex);
    if (NULL != m_fp) {
        fclose(m_fp);
    }
}

// Set file from which to read
int DataReader::set_file(const char *path) {
    int ret = SUCCESS;
    
    pthread_mutex_lock(&(m_mutex));
    do {
        // Invalid parameter
        if (NULL == path) {
            LOG(ERROR) << "Input path is NULL." << endl;
            ret = NULL_POINTER_ERROR;
            break;
        }

        // Change input file
        if (NULL != m_fp) {
            fclose(m_fp);
            LOG(INFO) << "Close file \"" << m_path << "\"." << endl;
            m_fp = NULL;
            m_path = "";
        }

        // Open file.
        m_fp = fopen(path, "r");

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
    pthread_mutex_unlock(&(m_mutex));
    
    return ret;
}

// Read one line from input file
int DataReader::get_line(string &line) {
    int ret = EOF_ERROR;
    struct timespec ts1 = {0, 0};
    struct timespec ts2 = {0, 0};
    double exec_ns = 0.0;

    clock_gettime(CLOCK_MONOTONIC, &ts1);
    pthread_mutex_lock(&(m_mutex));
    
    line = "";
    static char buffer[262500];

    // Read data until meeting the End-of-File
    while(fgets(buffer, 262500, m_fp) != NULL) {
        // If c-style string in buffer is end up with '\n', break
        int len = strlen(buffer);
        if ('\n' == buffer[len - 1]) {
            buffer[len - 1] = '\0';
            --len;
            line = line + buffer;

            ret = SUCCESS;
            break;
        }

        // Continue to read
        line = line + buffer;
    }

    if (line.size() > 0) {
        ret = SUCCESS;
    }

    pthread_mutex_unlock(&(m_mutex));
    clock_gettime(CLOCK_MONOTONIC, &ts2);

    exec_ns = 1e9 * (ts2.tv_sec - ts1.tv_sec) + (ts2.tv_nsec - ts1.tv_nsec);

    // Record read performence
    m_summary.record(exec_ns, line.length() + 1);

    return ret;
}

} // namespace wolong

