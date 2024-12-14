#pragma once

#include <chrono>

/**
 * @brief A timer class to measure elapsed time.
 */
class Timer
{
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint; // Start timepoint of the timer
    float* m_ResultPtr; // Pointer to store the elapsed time result

public:
    /**
     * @brief Constructs a new Timer object.
     *
     * @param ResultPtr Pointer to a float variable where the elapsed time will be stored.
     */
    Timer(float* ResultPtr);

    /**
     * @brief Destroys the Timer object and calculates the elapsed time.
     */
    ~Timer();

    /**
     * @brief Stops the timer and calculates the elapsed time.
     */
    void Stop() const;
};

