#include <string>
#include <vector>
#include "sandbox/zheshan.lx/problem3/include/summary.h"

#ifndef WLY_SANDBOX_ZHESHANLX_PROBLEM3_INCLUDE_DATAPROCESSOR_H_
#define WLY_SANDBOX_ZHESHANLX_PROBLEM3_INCLUDE_DATAPROCESSOR_H_

namespace wolong {

class DataProcessor {
    public:
        DataProcessor();
        ~DataProcessor();

        int get_query(std::string &line, std::vector<std::string> &query);

        inline const Summary &get_summary() const {
            return m_summary;
        }

    private:
        Summary m_summary;
};

} // namespace wolong

#endif // WLY_SANDBOX_ZHESHANLX_PROBLEM3_INCLUDE_DATAPROCESSOR_H_

