#pragma once

#include <chrono>

typedef std::chrono::high_resolution_clock chr_clock;

#define INTERVAL(start, end) ((end - start) / CLOCKS_PER_SEC)

#define START_TIMER() chr_clock::now()

#define ELAPSED_SECONDS(start) std::chrono::duration_cast<std::chrono::seconds>(chr_clock::now() - start).count()

#define ELAPSED_MSECNDS(start) std::chrono::duration_cast<std::chrono::milliseconds>(chr_clock::now() - start).count()