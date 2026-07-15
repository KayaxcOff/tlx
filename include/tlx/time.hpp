//
// Created by muham on 15.07.2026.
//

#ifndef TLX_TIME_HPP
#define TLX_TIME_HPP

#include <chrono>

namespace tlx {
    using std::chrono::milliseconds;
    using std::chrono::microseconds;
    using std::chrono::nanoseconds;
    using std::chrono::seconds;
    using std::chrono::minutes;
    using std::chrono::hours;

    /**
     * @brief A simple high-resolution stopwatch for measuring elapsed time.
     *
     * Uses `std::chrono::steady_clock` for monotonic and precise timing.
     */
    class StopWatch {
    public:
        /**
         * @brief Constructs and starts the stopwatch.
         */
        StopWatch();
        ~StopWatch();

        /**
         * @brief Restarts the stopwatch, setting the start time to now.
         */
        void restart() noexcept;

        /**
         * @brief Returns the elapsed time since start (or last restart).
         *
         * @tparam Duration The desired duration type (e.g. `std::chrono::milliseconds`,
         *                  `std::chrono::seconds`, `std::chrono::microseconds`, etc.)
         * @return Elapsed time as the count of the specified duration.
         */
        template<class Duration>
        [[nodiscard]]
        auto elapsed() const noexcept {
            return std::chrono::duration_cast<Duration>(
                std::chrono::steady_clock::now() - this->m_start
            ).count();
        }
    private:

        std::chrono::steady_clock::time_point m_start;
    };
} //namespace tlx

#endif //TLX_TIME_HPP