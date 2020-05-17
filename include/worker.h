#include <pthread.h>
#include <string>
#include <vector>
#include <map>
#include "base/thread/thread_pool.h"
#include "sandbox/zheshan.lx/problem3/include/summary.h"
#include "sandbox/zheshan.lx/problem3/include/data_reader.h"
#include "sandbox/zheshan.lx/problem3/include/data_writer.h"
#include "sandbox/zheshan.lx/problem3/include/data_processor.h"
#include "sandbox/zheshan.lx/problem3/include/query_segmenter.h"
#include "sandbox/zheshan.lx/problem3/include/term_counter.h"
#include "sandbox/zheshan.lx/problem3/include/send_to_bin.h"
#include "sandbox/zheshan.lx/problem3/include/send_to_bin.h"
#include "sandbox/zheshan.lx/problem3/include/merge.h"


#ifndef WLY_SANDBOX_ZHESHANLX_PROBLEM3_INCLUDE_WORKER_H_
#define WLY_SANDBOX_ZHESHANLX_PROBLEM3_INCLUDE_WORKER_H_

namespace wolong {
    
class Worker {
    public:
        Worker(int num_counters, int num_sorters);
        ~Worker();

        int run(const char *in_path, const char *out_path1, const char *out_path2);
        void print_summary() const;

        inline const Summary &get_summary() const {
            return m_summary;
        }

    private:
        void process_1(int tid);
        void process_2(int bin_id);

        int m_num_counters;
        int m_num_sorters;
        thread::ThreadPool m_thread_pool_1;
        thread::ThreadPool m_thread_pool_2;
        Summary m_summary;
        std::vector<Summary> m_sort_summary;
        std::vector<Summary> m_qp_summary;
        std::vector<Summary> m_qs_summary;

        DataReader     m_data_reader;
        DataWriter     m_data_writer;
        TermCounter    m_term_counter;
        SendToBin      m_send_to_bin;
        Merge          m_merge;

        std::map<std::string, long> m_basic_words;
        std::map<std::string, long> m_statistics;
        std::vector<std::vector<std::pair<std::string, long> > > m_bins;
        std::vector<std::pair<std::string, long> > m_results;
};

} // namespace wolong

#endif // WLY_SANDBOX_ZHESHANLX_PROBLEM3_INCLUDE_WORKER_H_

