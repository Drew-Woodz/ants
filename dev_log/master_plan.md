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

---

## Milestone Roadmap (updated Nov 20, 2025)

1. Sprint 0 – Brainstorm & Docs ✓ (Nov 18–19)
2. Sprint 1 – Pheromone-Driven Foraging MVP (Nov 19–30) – **Core substrate complete Nov 20**
   - Evaporating pheromone grids (food + home) ✓
   - Mouse-paintable trails for instant feedback ✓
   - 1–10 ants with sensing, laying, food pickup/dropoff (in progress – target Nov 25)
   - Visible recruitment → highway formation (target Nov 28)
   - Success = “holy shit, they’re organizing themselves” moment
3. Sprint 2 – Nest Arena + Digging Substrate (Dec 1–15)
   - Second connected arena
   - Queen, nurses, egg chambers
   - Worker excavation triggered by overcrowding pheromone

---

## Milestone Roadmap (updated Nov 19, 2025)

1. Sprint 0 – Brainstorm & Docs ✓ (Nov 18–19)
2. Sprint 1 – Pheromone-Driven Foraging MVP (Nov 19–30)
   - Evaporating pheromone grids (food + home)
   - Mouse-paintable trails for instant feedback
   - 1–10 ants with sensing, laying, food pickup/dropoff
   - Visible recruitment → highway formation
   - Success = “holy shit, they’re organizing themselves” moment
3. Sprint 2 – Nest Arena + Digging Substrate (Dec 1–15)
   - Second connected arena
   - Queen, nurses, egg chambers
   - Worker excavation triggered by overcrowding pheromone
4. Sprint 3 – Task Allocation & Castes (Dec)
5. Sprint 4 – Polish, Animation, Sound, Save/Load
6. Sprint 5 – Raspberry Pi 5 Port + Touch UI
7. Sprint 6+ – Hardware Bring-Up, Kickstarter Prep

---