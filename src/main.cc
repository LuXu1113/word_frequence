#include <stdlib.h>
#include <string>

#include "base/common/base.h"
#include "base/common/gflags.h"
#include "base/common/logging.h"

#include "sandbox/zheshan.lx/problem3/include/worker.h"

using std::string;

DEFINE_string(file_in_session_log, "/data2/zheshan.lx/wly/sandbox/zheshan.lx/problem3/data/session_log", "input session log file.");
DEFINE_string(file_out_basic_words, "basic_words.txt", "file for saving basic words.");
DEFINE_string(file_out_term_statistics, "term_statistics.txt", "file for saving output term statistics.");
DEFINE_string(num_counters, "1", "number of threads used to get and count terms.");
DEFINE_string(num_sorters, "1", "number of threads for sorting terms by frequency.");

int main (int argc, char **argv) {
    base::InitApp(&argc, &argv, "");

    string file_in_session_log      = FLAGS_file_in_session_log;
    string file_out_basic_words     = FLAGS_file_out_basic_words;
    string file_out_term_statistics = FLAGS_file_out_term_statistics;
    string num_counters             = FLAGS_num_counters;
    string num_sorters              = FLAGS_num_sorters;

    // Initialize worker, specify the number of counters and the number of sorters.
    wolong::Worker worker(atoi(num_counters.c_str()), atoi(num_sorters.c_str()));

    // Run job
    worker.run(file_in_session_log.c_str(), file_out_term_statistics.c_str(), file_out_basic_words.c_str());

    // Print performence summary
    worker.print_summary();

    return 0;
}

