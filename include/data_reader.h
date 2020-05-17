#include <pthread.h>
#include <string>
#include "sandbox/zheshan.lx/problem3/include/summary.h"

#ifndef WLY_SANDBOX_ZHESHANLX_PROBLEM3_INCLUDE_DATAREADER_H_
#define WLY_SANDBOX_ZHESHANLX_PROBLEM3_INCLUDE_DATAREADER_H_

namespace wolong {
    
class DataReader {
    public:
        DataReader();
        ~DataReader();

        int set_file(const char *file);
        int get_line(std::string &line);

        inline const Summary &get_summary() const {
            return m_summary;
        }

    private:
        FILE *m_fp;
        std::string m_path;
        pthread_mutex_t m_mutex;
        Summary m_summary;
};

} // namespace wolong

#endif // WLY_SANDBOX_ZHESHANLX_PROBLEM3_INCLUDE_DATAREADER_H_

