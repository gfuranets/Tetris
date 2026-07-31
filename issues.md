# Known Issues

Runtime errors and missing mechanics, in the order they will occur.
(Compile errors already fixed: duplicate `Tetromino` default constructor, missing `<cstdint>` include in `Game.h`.)

## Crashes — won't get past the first frame

- [ ] **1. Instant panic on first `loop()` pass.** `game.display(data)` (`src/main.cpp:98`) calls `Grid::getGrid()` (`lib/Grid/Grid.cpp:3`), which writes through the uninitialized pointer `bool** g`. On ESP32 this is a `StoreProhibited` Guru Meditation error → boot-loop. Happens with no buttons pressed.
- [ ] **2. Same crash on any button press.** `Tetromino::getView()` (`lib/Tetromino/Tetromino.cpp:95`) has the identical bug (`bool** res`, never allocated). Called by `Grid::isValid`/`collision` on every input.
- [ ] **3. Out-of-bounds array accesses** (surface once 1–2 are fixed):
  - `Grid::isValid` (`lib/Grid/Grid.cpp:25`) reads `grid[y + i][x + j]`; pieces spawn at `y = 15`, so with `i` up to 3 it reads `grid[18]` on a 16-row array.
  - `Grid::collision` (`lib/Grid/Grid.cpp:36`) starts its loop at `i = getSize()`, so an I-piece reads `view[4][j]` — past the 4×4 array. `grid[y - i - 1]` (`lib/Grid/Grid.cpp:40`) goes negative near row 0.
  - `isValid` never checks walls or floor, so `x` can reach −1 or 8 → `grid[...][-1]`.

## Silent failures — code runs but nothing happens

- [ ] **4. No input changes game state.** `Game::handleInput` (`lib/Game/Game.cpp:32-33`) copies the members into locals `test`/`current` that shadow them; every `current = test;` writes to the discarded local. Left/right/drop are all no-ops.
- [ ] **5. Rotate is a no-op twice over.** `src/main.cpp:81-82` gets `game.getCurrent()` by value and rotates the copy. Also shadows the `bool c` button variable, and `Game::handleInput` has no rotate case at all.
- [ ] **6. Drop logic inverted.** `lib/Game/Game.cpp:58`: `while (!grid.isValid(test) && !grid.collision(test))` — false immediately when the current position is valid, so the piece never moves.
- [ ] **7. Buttons auto-repeat.** Debounce flags (`r_flag` etc.) are never set to 1 (lines deleted in refactor), so a held button fires every ~100 ms loop. Typo at `src/main.cpp:75`: left-button release check tests `r` instead of `l`.
- [ ] **8. Display only ever gets brighter.** `Game::display` (`lib/Game/Game.cpp:81`) ORs bits into `data` without clearing it, and the old per-frame `data[i] = 0` reset was removed from `src/main.cpp`. Any lit pixel stays lit forever.

## Missing mechanics — game can't be played

- [ ] **9. No gravity.** The "move down one step" path (`default:` case in `handleInput`) is never called; no tick timer in `loop()`.
- [ ] **10. No lock / line-clear / respawn.** Nothing writes a landed piece into `grid`, checks full rows, or spawns a next piece. `Game`'s constructor picks exactly one tetromino per boot.
- [ ] **11. Fixed RNG seed.** `srand(2)` (`lib/Game/Game.cpp:6`) → same piece every boot. Replace with e.g. `esp_random()` later.

## Fix order

Issues 4–5 route everything through `getView()`/`getGrid()`, so fix 1–3 first (replace the `bool**` returns with a safe accessor like `bool at(row, col) const`, plus bounds checks) or the other fixes just crash faster.
