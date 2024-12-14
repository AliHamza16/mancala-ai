#include "timer.h"

#include <iostream>

/**
 * @brief Constructs a new Timer object and starts the timer.
 *
 * @param ResultPtr Pointer to a float variable where the elapsed time will be stored.
 */
Timer::Timer(float* ResultPtr) : m_ResultPtr(ResultPtr)
{
	m_StartTimepoint = std::chrono::high_resolution_clock::now(); // Record the current time
}


/**
 * @brief Destroys the Timer object and calculates the elapsed time.
 */
Timer::~Timer()
{
	Stop(); // Stop the timer and calculate the elapsed time
}


/**
 * @brief Stops the timer and calculates the elapsed time.
 */
void Timer::Stop() const
{
    std::chrono::high_resolution_clock::time_point endTimepoint = std::chrono::high_resolution_clock::now(); // Get the current time

    // Calculate the duration in microseconds
    long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
    long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();
    long long duration = (end - start);

    // Convert duration to milliseconds
    float ms = duration * 0.001;

    *m_ResultPtr = ms; // Store the elapsed time in the result pointer
}
