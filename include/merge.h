#include <string>
#include <vector>
#include <map>
#include "sandbox/zheshan.lx/problem3/include/summary.h"

#ifndef WLY_SANDBOX_ZHESHANLX_PROBLEM3_INCLUDE_MERGE_H_
#define WLY_SANDBOX_ZHESHANLX_PROBLEM3_INCLUDE_MERGE_H_

namespace wolong {
    
class Merge {
    public:
        Merge();
        ~Merge();

        int run(std::vector<std::vector<std::pair<std::string, long> > > &bins,
                std::vector<std::pair<std::string, long> > &results);

        inline const Summary &get_summary() const {
            return m_summary;
        }

    private:
        Summary m_summary;
};

} // namespace wolong

#endif // WLY_SANDBOX_ZHESHANLX_PROBLEM3_INCLUDE_MERGE_H_

