#include <stdlib.h>
#include <string>
#include <vector>
#include "pub/src/base/testing/gtest.h"
#include "base/common/base.h"
#include "base/common/gflags.h"
#include "base/common/logging.h"

#include "sandbox/zheshan.lx/problem3/include/errno.h"
#include "sandbox/zheshan.lx/problem3/include/data_reader.h"
#include "sandbox/zheshan.lx/problem3/include/data_writer.h"
#include "sandbox/zheshan.lx/problem3/include/data_processor.h"
#include "sandbox/zheshan.lx/problem3/include/query_segmenter.h"
#include "sandbox/zheshan.lx/problem3/include/term_counter.h"
#include "sandbox/zheshan.lx/problem3/include/send_to_bin.h"
#include "sandbox/zheshan.lx/problem3/include/merge.h"
#include "sandbox/zheshan.lx/problem3/include/worker.h"

using namespace std;
  
TEST(data_reader, set_file) {
    int ret = SUCCESS;
    wolong::DataReader reader;
    

    ret = reader.set_file(NULL);
    EXPECT_EQ(NULL_POINTER_ERROR, ret);

    static char empty_path[] = "";
    ret = reader.set_file(empty_path);
    EXPECT_EQ(OPEN_FILE_ERROR, ret);

    static char non_exists_path[] = ".../this_is_not_a_file";
    ret = reader.set_file(non_exists_path);
    EXPECT_EQ(OPEN_FILE_ERROR, ret);

    system("touch input.txt");
    static char path[] = "input.txt";
    ret = reader.set_file(path);
    EXPECT_EQ(SUCCESS, ret);
    system("rm -f input.txt");
}

TEST(data_reader, get_line) {
    int ret = SUCCESS;
    wolong::DataReader reader;
    string line;

    system("touch input.txt");
    static char path[] = "input.txt";
    ret = reader.set_file(path);
    EXPECT_EQ(SUCCESS, ret);

    ret = reader.get_line(line);
    EXPECT_EQ(EOF_ERROR, ret);

    system("echo -n 'this is the 1-st line' >> input.txt");
    ret = reader.set_file(path);
    EXPECT_EQ(SUCCESS, ret);

    ret = reader.get_line(line);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_STREQ("this is the 1-st line", line.c_str());

    ret = reader.get_line(line);
    EXPECT_EQ(EOF_ERROR, ret);

    system("echo -n '\n\n' >> input.txt");
    ret = reader.set_file(path);
    EXPECT_EQ(SUCCESS, ret);

    ret = reader.get_line(line);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_STREQ("this is the 1-st line", line.c_str());

    ret = reader.get_line(line);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_STREQ("", line.c_str());

    ret = reader.get_line(line);
    EXPECT_EQ(EOF_ERROR, ret);

    system("rm -f input.txt");
}

TEST(DataProcessor, get_query) {
    int ret = SUCCESS;
    wolong::DataProcessor processor;

    string line;
    vector<string> query;

    processor.get_query(line, query);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(0, (int)query.size());
}

TEST(QuerySegmenter, split_query) {
    int ret = SUCCESS;
    wolong::QuerySegmenter segmenter;

    string query;
    vector<string> terms;
    segmenter.split_query(query, terms);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(0, (int)terms.size());
}


int main(int argc, char **argv) {
    base::InitApp(&argc, &argv, "");
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

