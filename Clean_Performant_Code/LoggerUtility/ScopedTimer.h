// ===========================================================================
// ScopedTimer.h
// ===========================================================================

#pragma once

#include <iostream>
#include <chrono>

#include "Logger.h"

class ScopedTimer
{
public:
    enum class Resolution { Milli, Micro, Nano };

private:
    enum class Resolution m_resolution;

public:
    ScopedTimer() : ScopedTimer{ Resolution::Milli }
    {
        Logger::startWatch();
    }

    ScopedTimer(enum class Resolution resolution) : m_resolution{ resolution }
    {
        Logger::startWatch();
    }

    ~ScopedTimer() {

        switch (m_resolution)
        {
        case Resolution::Milli:
            Logger::stopWatchMilli(std::cout);
            break;
        case Resolution::Micro:
            Logger::stopWatchMicro(std::cout);
            break;
        case Resolution::Nano:
            Logger::stopWatchNano(std::cout);
            break;
        }
    }

    // no copying or moving
    ScopedTimer(const ScopedTimer&) = delete;
    ScopedTimer& operator=(const ScopedTimer&) = delete;

    ScopedTimer(ScopedTimer&&) = delete;
    ScopedTimer& operator=(ScopedTimer&&) = delete;
};

// ===========================================================================
// End-of-File
// ===========================================================================
