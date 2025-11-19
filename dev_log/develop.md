# Development Guidelines – Myrmidon

## Tech Stack Decision (to be finalized this week)
- Language: C++20 (performance critical)
- Graphics: Raylib (single-file, dear imgui built-in, works great on Pi) or SFML 3
- Entity system: Custom lightweight ECS or enl::entt
- Build: CMake + conan or vcpkg
- Testing: Catch2 + GitHub Actions CI
- Code style: clang-format + clang-tidy

## Best Practices We Enforce From Day 1
- Git branching: GitFlow or trunk-based with feature flags
- Every new class/system → header + unit tests before merge
- No magic numbers – all ant parameters in data-driven JSON/TOML
- Error handling: RAII everywhere, std::expected where appropriate
- Logging: spdlog with rotating file for long-term colony autopsy
- Security: Even though offline, sandbox file access, sign savegames

## First Technical Deep Dive Questions for You
1. Raylib vs SFML vs SDL2+Dear ImGui – which ecosystem are you most comfortable with?
2. How many ants do you personally want to see on screen before it feels “alive”? (this drives grid size vs detail trade-off)
3. Real-time vs turn-based: do we advance simulation every frame (true real-time) or in fixed 0.1 s ticks for determinism?

## Sprint 0 Retrospective – Decisions Locked (Nov 19, 2025)

- Compiler: LLVM Clang 21.1.5 (primary) + MSVC fallback for faster Windows builds
- Build: CMake + Ninja (preferred) / Visual Studio generator (current working)
- Graphics: Raylib 5.6-dev confirmed — zero dependencies, perfect Pi5 support
- Asset pipeline: Post-build copy via $<TARGET_FILE_DIR> — works across generators
- Branching: GitFlow (main = golden releases, develop = integration)
- First green executable achieved with real ant.png sprite
- Lesson learned: Windows toolchain is spicy — hybrid MSVC/Clang workflow adopted