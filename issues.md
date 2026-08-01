# Known Issues

Runtime errors and missing mechanics, in the order they will occur.
(Compile errors already fixed: duplicate `Tetromino` default constructor, missing `<cstdint>` include in `Game.h`.)

## Crashes — won't get past the first frame

- [x] **1. Instant panic on first `loop()` pass.** ~~`game.display(data)` calls `Grid::getGrid()`, which writes through the uninitialized pointer `bool** g`.~~ **Resolved:** replaced with `Grid::at(row, col)` accessor over a statically allocated `bool grid[17][10]` member.
- [x] **2. Same crash on any button press.** ~~`Tetromino::getView()` has the identical bug (`bool** res`, never allocated).~~ **Resolved:** replaced with `Tetromino::at(i, j)` accessor over a static `bool view[4][4]` member.
- [ ] **3. Out-of-bounds accesses / broken validation** (rewritten with explicit bounds — close, but two typos break it):
  - `Grid::isValid` (`lib/Grid/Grid.cpp:17`): `row > 7` should be **`col > 7`** — as written the right wall is unchecked and the entire top half of the board (rows 8–15, including spawn) is "invalid".
  - `Grid::isValid` (`lib/Grid/Grid.cpp:19`): `if (row < 16 || grid[row][col])` should be **`&&`** — as written every occupied cell (all rows < 16) is "invalid". Net effect of both: `isValid` always returns false → piece frozen at spawn, and the drop `while` loops forever → watchdog reset on the D button.
  - `Grid::collision` (`lib/Grid/Grid.cpp:26-38`): still starts at `i = getSize()` (reads `view[4][j]`) and `grid[y - i - 1]` goes negative — but it is now **unused**; delete it from `Grid.h`/`Grid.cpp`.
  - **New — `Game::clear()` OOB writes** (`lib/Game/Game.cpp:84-90`): loops `i < 17`, `j < 10` over the now-16×8 array → writes past `grid` every frame. (It also clears the wrong thing — see issue 8.)
- [x] **~~3b. draw()-every-frame OOB write~~** — resolved: per-frame `draw()` is gone; `display()` overlays the piece with bounds guards.

## Silent failures — code runs but nothing happens

- [x] **4. No input changes game state.** **Resolved:** `Game::handleInput` now uses copy semantics — `*test = *current`, move the copy, then `*current = *test` only if `grid.isValid` passes. Trial-and-rollback works as intended.
- [x] **5. Rotate is a no-op twice over.** **Fully resolved:** rotation now goes through `handleInput('C')` with copy-test-commit and an `isValid` check.
- [ ] **6. Drop logic inverted.** *Still open:* `lib/Game/Game.cpp:67`: `while (!grid.isValid(*test)) test->move('D');` — moves while INVALID instead of while valid. From a valid position it does nothing; and with issue 3's `isValid` always returning false, it currently loops forever (watchdog reset). Needs try-step-commit: move the copy down first, commit `*current = *test` while `isValid` holds.
- [ ] **7. Buttons auto-repeat.** *Half fixed:* the left-release typo is corrected (`l == 0` at `src/main.cpp:78`). But the flags are still never set to 1 on press, so the release branches are dead code and a held button still fires every ~105 ms.
- [ ] **8. Display only ever gets brighter.** *Regressed — clear flipped to the wrong buffer again:* `Game::clear()` (`lib/Game/Game.cpp:82`) now zeroes **`grid`** (which must persist — it's the landed-blocks state, and once lock exists this would erase the stack every frame) while **`data` is never reset**, so `display()`'s `data[i] |=` accumulates old piece positions on the LED forever. Fix: `clear()` should zero the 16 bytes of `data`; `grid` is only ever cleared by line-clears (or game over).
- [x] **12. Piece collides with its own ghost.** **Resolved:** per-frame `draw()` is gone; `display()` overlays the piece into `data` without touching `grid`.

## Missing mechanics — game can't be played

- [x] **9. No gravity.** **Resolved:** `src/main.cpp:100-105` calls `game.handleInput('F')` (the `default:` move-down case) once per second and now resets `last_time = millis()`. Caveat: the piece never *stops* falling — the floor check is non-functional (issue 3), so `y` decrements past 0 into negative indices after ~15 s.
- [ ] **10. No lock / line-clear / respawn.** Nothing writes a landed piece into `grid`, checks full rows, or spawns a next piece. `Game`'s constructor picks exactly one tetromino per boot.
- [ ] **11. Fixed RNG seed.** `srand(2)` (`lib/Game/Game.cpp:6`) → same piece every boot. Replace with e.g. `esp_random()` later.

## Fix order

Issues 4–5 route everything through `getView()`/`getGrid()`, so fix 1–3 first (replace the `bool**` returns with a safe accessor like `bool at(row, col) const`, plus bounds checks) or the other fixes just crash faster.

**Status (2026-08-01, fifth pass):** 1, 2, 4, 5, 9, 12 resolved. Render/state separation is in (overlay `display()`, 16×8 grid, validated rotate). **Current blockers: two typos in `isValid` (`row > 7` → `col > 7`; `||` → `&&`) make every position invalid — piece frozen, drop = infinite loop/WDT reset (issue 3); `clear()` wipes `grid` instead of `data` and overruns the array with 17×10 bounds (issues 3/8); drop `while` still inverted (issue 6).** Then: debounce flags (7), lock/line-clear/respawn (10), RNG seed (11), delete dead `collision()`.
