#include "text.h"

int main()
{
    auto file = "/home/finkrer/CLionProjects/Task_1/2701.txt";
    auto text = getFileContents(file);
    auto start = chrono::steady_clock::now();
    auto sortedStats1 = getStatsWithStandardAllocator(text);
    auto end = chrono::steady_clock::now();
    cout << "Elapsed time with standard allocator: "
         << chrono::duration_cast<chrono::milliseconds>(end - start).count()
         << " ms\n";

    text = getFileContents(file);
    start = chrono::steady_clock::now();
    auto sortedStats2 = getStatsWithCustomAllocator(text);
    end = chrono::steady_clock::now();
    cout << "Elapsed time with custom allocator: "
         << chrono::duration_cast<chrono::milliseconds>(end - start).count()
         << " ms\n";

//    for (auto const& [frequency, word]: sortedStats1)
//        cout << frequency << ": " << word << "\n";
}
