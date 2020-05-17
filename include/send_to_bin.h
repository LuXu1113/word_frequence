#include <string>
#include <vector>
#include <map>
#include "sandbox/zheshan.lx/problem3/include/summary.h"

#ifndef WLY_SANDBOX_ZHESHANLX_PROBLEM3_INCLUDE_SENDTOBIN_H_
#define WLY_SANDBOX_ZHESHANLX_PROBLEM3_INCLUDE_SENDTOBIN_H_

namespace wolong {
    
class SendToBin {
    public:
        SendToBin();
        ~SendToBin();

        int run(std::map<std::string, long> &statistics,
                std::vector<std::vector<std::pair<std::string, long> > > &bins,
                int num_bins);

        const Summary &get_summary() const {
            return m_summary;
        }

    private:
        Summary m_summary;
};

} // namespace wolong

#endif // WLY_SANDBOX_ZHESHANLX_PROBLEM3_INCLUDE_SENDTOBIN_H_

