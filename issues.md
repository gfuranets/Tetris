# Known Issues

Runtime errors and missing mechanics, in the order they will occur.
(Compile errors already fixed: duplicate `Tetromino` default constructor, missing `<cstdint>` include in `Game.h`.)

## Crashes — won't get past the first frame

- [x] **1. Instant panic on first `loop()` pass.** ~~`game.display(data)` calls `Grid::getGrid()`, which writes through the uninitialized pointer `bool** g`.~~ **Resolved:** replaced with `Grid::at(row, col)` accessor over a statically allocated `bool grid[17][10]` member.
- [x] **2. Same crash on any button press.** ~~`Tetromino::getView()` has the identical bug (`bool** res`, never allocated).~~ **Resolved:** replaced with `Tetromino::at(i, j)` accessor over a static `bool view[4][4]` member.
- [x] **3. Out-of-bounds accesses / broken validation.** **Resolved:** `isValid` now does correct explicit bounds checks (`row < 0 || col < 0 || col > 7`, overlap guarded by `row < 16 &&`); dead `collision()` deleted; `clear()` bounds fixed. (New OOB risk to avoid in the upcoming `lock()`: bounds-check before writing.)
- [x] **~~3b. draw()-every-frame OOB write~~** — resolved: per-frame `draw()` is gone; `display()` overlays the piece with bounds guards.

## Silent failures — code runs but nothing happens

- [x] **4. No input changes game state.** **Resolved:** `Game::handleInput` now uses copy semantics — `*test = *current`, move the copy, then `*current = *test` only if `grid.isValid` passes. Trial-and-rollback works as intended.
- [x] **5. Rotate is a no-op twice over.** **Fully resolved:** rotation now goes through `handleInput('C')` with copy-test-commit and an `isValid` check.
- [x] **6. Drop logic inverted.** **Resolved:** the drop case now moves the copy down and commits while `isValid` holds — correct hard-drop. (It still needs `land()` after the loop — tracked in issue 10.)
- [ ] **7. Buttons auto-repeat.** *Half fixed:* the left-release typo is corrected (`l == 0` at `src/main.cpp:78`). But the flags are still never set to 1 on press, so the release branches are dead code and a held button still fires every ~105 ms.
- [x] **8. Display only ever gets brighter.** **Resolved:** `Game::clear(data)` zeroes the 16-byte frame buffer each frame; `grid` persists as landed-block state; `display()` overlays the piece.
- [x] **12. Piece collides with its own ghost.** **Resolved:** per-frame `draw()` is gone; `display()` overlays the piece into `data` without touching `grid`.

## Missing mechanics — game can't be played

- [x] **9. No gravity.** **Resolved:** `src/main.cpp:100-105` calls `game.handleInput('F')` (the `default:` move-down case) once per second and now resets `last_time = millis()`. Caveat: the piece never *stops* falling — the floor check is non-functional (issue 3), so `y` decrements past 0 into negative indices after ~15 s.
- [ ] **10. No lock / line-clear / respawn.** *In progress — helpers exist, wiring and two bodies missing:*
  - `Grid::isFull` / `Grid::isEmpty` / `Grid::clearLines` — correct. ✓
  - ~~`Grid::lock()`~~ — **done** ✓: takes `const Tetromino&`, bounds-checked stamping.
  - ~~`Game::land()` wiring~~ — **done** ✓: called after the `'D'` while-loop and in the `default:` else. Landing pipeline complete.
  - `Grid::shift()` (`lib/Grid/Grid.cpp:46-59`) — no OOB anymore, but **the whole stack levitates**: scanning top-down and pulling `grid[i-1]` up into every empty row `i` makes the emptiness bubble *down* through the stack, carrying every filled row **up by one** — and `land()` calls `shift()` on every lock, so the stack rises one row per landed piece and cleared gaps never close. Needs bottom-up downward compaction (the write-index version).
  - ~~`Game::spawn()` arg bug~~ — fixed ✓ (cosmetic: constructor still duplicates `spawn()` line-for-line).
  - *New note for issue 11:* `Game game;` is a **global** — its constructor (and first `rand()`) runs *before* `setup()`, so seeding in `setup()` is too late for the first piece. Put `srand(esp_random())` at the top of `Game::Game()` instead.
- [ ] **11. Unseeded RNG.** `srand(2)` was removed but nothing replaced it — unseeded `rand()` uses seed 1: still the same piece sequence every boot. Seed once with `srand(esp_random())`.

## Fix order

Issues 4–5 route everything through `getView()`/`getGrid()`, so fix 1–3 first (replace the `bool**` returns with a safe accessor like `bool at(row, col) const`, plus bounds checks) or the other fixes just crash faster.

**Status (2026-08-01, ninth pass):** 1–6, 8, 9, 12 resolved; `land()` wired into both down-paths — landing pipeline works end to end. One lib bug left: **`shift()` levitates the stack one row per landing and never closes gaps** (issue 10) — use write-index compaction. Then main.cpp: debounce flags (7), loop restructure, and `srand(esp_random())` **in the Game constructor**, not `setup()` (11).
