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
- [x] **7. Buttons auto-repeat.** **Resolved:** flags are set to 1 on press and reset on release for all four buttons; the left-release typo is fixed. One press = one action.
- [x] **8. Display only ever gets brighter.** **Resolved:** `Game::clear(data)` zeroes the 16-byte frame buffer each frame; `grid` persists as landed-block state; `display()` overlays the piece.
- [x] **12. Piece collides with its own ghost.** **Resolved:** per-frame `draw()` is gone; `display()` overlays the piece into `data` without touching `grid`.

## Missing mechanics — game can't be played

- [x] **9. No gravity.** **Resolved:** `src/main.cpp:100-105` calls `game.handleInput('F')` (the `default:` move-down case) once per second and now resets `last_time = millis()`. Caveat: the piece never *stops* falling — the floor check is non-functional (issue 3), so `y` decrements past 0 into negative indices after ~15 s.
- [x] **10. No lock / line-clear / respawn.** **Resolved:** full landing pipeline works — `land()` (called from both down-paths) → bounds-checked `Grid::lock(const Tetromino&)` → `clearLines()` → write-index `shift()` compaction → `spawn()` → game-over board wipe when the new piece has no room.
- [x] **11. Unseeded RNG.** **Resolved:** `srand(esp_random())` at the top of `Game::Game()` — in the constructor, not `setup()`, because the global `Game game;` is constructed first.

## Fix order

Issues 4–5 route everything through `getView()`/`getGrid()`, so fix 1–3 first (replace the `bool**` returns with a safe accessor like `bool at(row, col) const`, plus bounds checks) or the other fixes just crash faster.

**Status (2026-08-01, final):** All issues (1–12) resolved. Full game loop: input (debounced) → gravity tick → move/rotate/drop validation → land → lock → line clear → compact → respawn → game-over reset, rendered per-frame via clear + grid copy + piece overlay, transmitted as one contiguous I2C frame. Remaining polish (non-issues): delete the duplicated render block in `loop()`, drop the leftover `delay(5)`s and `sendFrame`'s trailing `delay(10)`, make `last_time` `uint32_t`, and check physical display orientation on first upload (fix in `display()` mapping if flipped, not in game logic).
