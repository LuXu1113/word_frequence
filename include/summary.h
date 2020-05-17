#ifndef WLY_SANDBOX_ZHESHANLX_PROBLEM3_INCLUDE_SUMMARY_H_
#define WLY_SANDBOX_ZHESHANLX_PROBLEM3_INCLUDE_SUMMARY_H_

namespace wolong {

class Summary {
    public:
        Summary();
        ~Summary();

        inline void record(double exec_time, double exec_volumn) {
            ++m_total_exec_count;
            m_total_exec_time   += exec_time;
            m_total_exec_volumn += exec_volumn;
        }

        inline double get_time_ns() const {
            return m_total_exec_time;
        }

        inline double get_time_us() const {
            return m_total_exec_time / 1e3;
        }

        inline double get_time_ms() const {
            return m_total_exec_time / 1e6;
        }

        inline double get_time_s() const {
            return m_total_exec_time / 1e9;
        }

        inline double get_volumn() const {
            return m_total_exec_volumn;
        }

        inline long get_count() const {
            return m_total_exec_count;
        }

        inline double get_qps() const {
            return m_total_exec_count / m_total_exec_time * 1e9;
        }

        inline void add(const Summary &other) {
            m_total_exec_count  += other.get_count();
            m_total_exec_time   += other.get_time_ns();
            m_total_exec_volumn += other.get_volumn();
        }

    private:
        long m_total_exec_count;
        double m_total_exec_time;
        double m_total_exec_volumn;
};

} // namespace wolong

#endif // WLY_SANDBOX_ZHESHANLX_PROBLEM3_INCLUDE_SUMMARY_H_

