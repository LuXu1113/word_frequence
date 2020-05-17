#include <pthread.h>
#include <string>
#include "nlp/segment/segmenter.h"
#include "sandbox/zheshan.lx/problem3/include/summary.h"

#ifndef WLY_SANDBOX_ZHESHANLX_PROBLEM3_INCLUDE_QUERYSEGMENTER_H_
#define WLY_SANDBOX_ZHESHANLX_PROBLEM3_INCLUDE_QUERYSEGMENTER_H_

namespace wolong {
    
class QuerySegmenter {
    public:
        QuerySegmenter();
        ~QuerySegmenter();

        int split_query(const std::string &query, std::vector<std::string> &terms);

        inline const Summary &get_summary() const {
            return m_summary;
        }

    private:
        nlp::segment::Segmenter m_segmenter;
        pthread_mutex_t m_mutex;
        Summary m_summary;
};

} // namespace wolong

#endif // WLY_SANDBOX_ZHESHANLX_PROBLEM3_INCLUDE_QUERYSEGMENTER_H_

