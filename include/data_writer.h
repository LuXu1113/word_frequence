#include <string>
#include <vector>
#include <map>
#include "sandbox/zheshan.lx/problem3/include/summary.h"

#ifndef WLY_SANDBOX_ZHESHANLX_PROBLEM3_INCLUDE_DATAWRITER_H_
#define WLY_SANDBOX_ZHESHANLX_PROBLEM3_INCLUDE_DATAWRITER_H_

namespace wolong {
    
class DataWriter {
    public:
        DataWriter();
        ~DataWriter();

        int set_file(const char *path);
        int write(const std::vector<std::pair<std::string, long> > &results);
        int write(const double results);
        int write(const std::map<std::string, long> &results);

        inline const Summary &get_summary() const {
            return m_summary;
        }

    private:
        FILE *m_fp;
        std::string m_path;
        Summary m_summary;
};

} // namespace wolong

#endif // WLY_SANDBOX_ZHESHANLX_PROBLEM3_INCLUDE_DataWriter_H_

