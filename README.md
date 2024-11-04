# Guild Manager

Console app for managing game guild. C++17 practice project.

Add/remove players, sort by gearscore, rank system (GM/Officer/Member), basic stats.

File save works, load doesn't (todo).

## Build

Windows (Clang):
```bash
clang++ -std=c++17 -Wall -Wextra main.cpp -o main.exe
./main.exe
```

Linux/macOS (GCC):
```bash
g++ -std=c++17 -Wall -Wextra main.cpp -o guild_manager
./guild_manager
```

Requires C++17 (Clang 5+, GCC 7+, MSVC 2017+)
