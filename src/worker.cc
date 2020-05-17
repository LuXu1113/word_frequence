#include <stdio.h>
#include <time.h>
#include <string>
#include <algorithm>
#include <map>
#include "pub/src/base/common/logging.h"
#include "base/common/closure.h"
#include "base/thread/thread_pool.h"
#include "sandbox/zheshan.lx/problem3/include/errno.h"
#include "sandbox/zheshan.lx/problem3/include/data_reader.h"
#include "sandbox/zheshan.lx/problem3/include/data_processor.h"
#include "sandbox/zheshan.lx/problem3/include/query_segmenter.h"
#include "sandbox/zheshan.lx/problem3/include/term_counter.h"
#include "sandbox/zheshan.lx/problem3/include/send_to_bin.h"
#include "sandbox/zheshan.lx/problem3/include/send_to_bin.h"
#include "sandbox/zheshan.lx/problem3/include/merge.h"
#include "sandbox/zheshan.lx/problem3/include/worker.h"

using std::string;
using std::vector;
using std::map;
using std::pair;
using std::endl;

namespace wolong {

Worker::Worker(int num_counters, int num_sorters) :
    m_num_counters(num_counters),
    m_num_sorters(num_sorters),
    m_thread_pool_1(num_counters),
    m_thread_pool_2(num_sorters),
    m_summary(),
    m_data_reader(),
    m_term_counter(),
    m_send_to_bin(),
    m_merge() {
}
    
Worker::~Worker() {
}

// Count terms
void Worker::process_1(int tid) {
    DataProcessor  data_processor;
    QuerySegmenter query_segmenter;
    string line;
    vector<string> query;
    vector<string> terms;

    while (SUCCESS == m_data_reader.get_line(line)) {
        query.clear();

        data_processor.get_query(line, query);

        for(unsigned int i = 0; i < query.size(); ++i) {
            query_segmenter.split_query(query[i], terms);
            m_term_counter.add_terms(query[i], terms, m_basic_words, m_statistics);
        }
    }

    m_qs_summary[tid] = query_segmenter.get_summary();
    m_qp_summary[tid] = data_processor.get_summary();

    return ;
}

static bool cmp(const pair<string, long> &p1, const pair<string, long> &p2) {  
    return p1.second > p2.second;
}

// Sort terms by frequency
void Worker::process_2(int bin_id) {
    struct timespec ts1 = {0, 0};
    struct timespec ts2 = {0, 0};
    double exec_ns = 0.0;

    clock_gettime(CLOCK_MONOTONIC, &ts1);
    if (bin_id == 0 && m_bins[bin_id].size() != 0) {
        sort(m_bins[bin_id].begin(), m_bins[bin_id].end(), cmp);
    }
    clock_gettime(CLOCK_MONOTONIC, &ts2);
    exec_ns = 1e9 * (ts2.tv_sec - ts1.tv_sec) + (ts2.tv_nsec - ts1.tv_nsec);
    m_sort_summary[bin_id].record(exec_ns, 1);

    return ;
}

// Main procedure
int Worker::run(const char *in_path,
                const char *out_path1,
                const char *out_path2) {
    int ret = SUCCESS;
    struct timespec ts1 = {0, 0};
    struct timespec ts2 = {0, 0};
    double exec_ns = 0.0;

    clock_gettime(CLOCK_MONOTONIC, &ts1);
    do {
        // Fail to set reader
        ret = m_data_reader.set_file(in_path);
        if (SUCCESS != ret) {
            LOG(ERROR) << "DataReader set file failed!" << endl;
            break;
        }

        // Fail to set Writer
        ret = m_data_writer.set_file(out_path1);
        if (SUCCESS != ret) {
            LOG(ERROR) << "DataWriter set file failed!" << endl;
            break;
        }

        // Prepare summay for each thread.
        m_qp_summary.resize(m_num_counters);
        m_qs_summary.resize(m_num_counters);

        // Start threads for counting terms
        for (int i = 0; i < m_num_counters; ++i) {
            Closure *task = NewCallback(this, &Worker::process_1, i);
            m_thread_pool_1.AddTask(task);
        }
        
        // Sync threads
        m_thread_pool_1.JoinAll();

        // Send terms to bins
        ret = m_send_to_bin.run(m_statistics, m_bins, m_num_sorters);
        if (SUCCESS != ret) {
            LOG(ERROR) << "Send to bin failed!" << endl;
            break;
        }

        // Start threads for sorting (one threads per bin)
        m_sort_summary.resize(m_num_sorters);
        for (int i = 0; i < m_num_sorters; ++i) {
            Closure *task = NewCallback(this, &Worker::process_2, i);
            m_thread_pool_2.AddTask(task);
        }

        // Sync threads
        m_thread_pool_2.JoinAll();

        // Merge all bins
        ret = m_merge.run(m_bins, m_results);
        if (SUCCESS != ret) {
            LOG(ERROR) << "Merge results failed!" << endl;
            break;
        }

        // Write term statistics to file
        m_data_writer.write(m_results);

        // Set another file to write
        ret = m_data_writer.set_file(out_path2);
        if (SUCCESS != ret) {
            LOG(ERROR) << "DataWriter set file failed!" << endl;
            break;
        }

        // Write persentage of basic words to file
        m_data_writer.write((double)(m_basic_words.size()) / (double)(m_statistics.size()));

        // Write all words to file
        m_data_writer.write(m_basic_words);

    } while(0);

    clock_gettime(CLOCK_MONOTONIC, &ts2);
    exec_ns = 1e9 * (ts2.tv_sec - ts1.tv_sec) + (ts2.tv_nsec - ts1.tv_nsec);

    // Record performence
    m_summary.record(exec_ns, 1);

    return ret;
}

// Print the performence of each module
void Worker::print_summary () const {
    const Summary &s_reader      = m_data_reader.get_summary();
    const Summary &s_counter     = m_term_counter.get_summary();
    const Summary &s_send_to_bin = m_send_to_bin.get_summary();
    const Summary &s_merge       = m_merge.get_summary();
    const Summary &s_writer      = m_data_writer.get_summary();

    Summary s_processor;
    for (unsigned int i = 0; i < m_qp_summary.size(); ++i) {
        s_processor.add(m_qp_summary[i]);
    }

    Summary s_segmenter;
    for (unsigned int i = 0; i < m_qs_summary.size(); ++i) {
        s_segmenter.add(m_qs_summary[i]);
    }

    Summary s_sort;
    for (unsigned int i = 0; i < m_sort_summary.size(); ++i) {
        s_sort.add(m_sort_summary[i]);
    }

    long num_querys = s_reader.get_count();

    printf("+======================================================================+\n");
    printf("|            Summary (num_counters = %3d, num_sorters = %3d)           |\n", m_num_counters, m_num_sorters);
    printf("|======================================================================|\n");
    printf("|    module   |  time_per_thrd(ms)  |    kilo-query/sec   |    perf    |\n");
    printf("|======================================================================|\n");
    printf("|    reader   |   %15.3lf   |    %13.2f    |            |\n",
           s_reader.get_time_ms() / m_num_counters, s_reader.get_qps() * m_num_counters / 1000);
    printf("|======================================================================|\n");
    printf("|  processor  |   %15.3lf   |    %13.2f    |            |\n",
           s_processor.get_time_ms() / m_num_counters, num_querys / s_processor.get_time_ms() * m_num_counters);
    printf("|======================================================================|\n");
    printf("|  segmenter  |   %15.3lf   |    %13.2f    |            |\n",
           s_segmenter.get_time_ms() / m_num_counters, num_querys / s_segmenter.get_time_ms() * m_num_counters);
    printf("|======================================================================|\n");
    printf("|   counter   |   %15.3lf   |    %13.2f    |            |\n",
           s_counter.get_time_ms() / m_num_counters, num_querys / s_counter.get_time_ms() * m_num_counters);
    printf("|======================================================================|\n");
    printf("| send to bin |   %15.3lf   |    %13.2f    |            |\n",
           s_send_to_bin.get_time_ms(), num_querys / s_send_to_bin.get_time_ms());
    printf("|======================================================================|\n");
    printf("|    sort     |   %15.3lf   |    %13.2f    |            |\n",
           s_sort.get_time_ms(), num_querys / s_sort.get_time_ms());
    printf("|======================================================================|\n");
    printf("|    merge    |   %15.3lf   |    %13.2f    |            |\n",
           s_merge.get_time_ms(), num_querys / s_merge.get_time_ms());
    printf("|======================================================================|\n");
    printf("|   writer    |   %15.3lf   |    %13.2f    |            |\n",
           s_writer.get_time_ms(), num_querys / s_writer.get_time_ms());
    printf("|======================================================================|\n");
    printf("|    total    |   %15.3lf   |    %13.2f    |            |\n",
           m_summary.get_time_ms(), num_querys / m_summary.get_time_ms());
    printf("+======================================================================+\n");

}

} // namespace wolong

