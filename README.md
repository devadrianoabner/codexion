## Tests

Arguments order: number_of_coders time_to_burnout time_to_compile
time_to_debug time_to_refactor number_of_compiles_required
dongle_cooldown scheduler

### 1. Basic behaviour
| Command | Expected |
|---|---|
| `./codexion 1 800 200 200 200 5 0 fifo` | Single coder, single dongle: must burn out around 800 ms. |
| `./codexion 5 2000 200 200 200 8 0 fifo` | No burnout. Stops when every coder has compiled 8 times. |
| `./codexion 5 2000 200 200 200 8 0 edf` | Same as above with edf. |

### 2. Burnout detection and logs
| Command | Expected |
|---|---|
| `./codexion 5 500 200 200 200 8 0 fifo` | Infeasible (cycle is 600 > 500). Must burn out near 500 ms. "burned out" is the last line, printed within 10 ms of the deadline. |

On feasible runs, check that:
- every "is compiling" is preceded by exactly two "has taken a dongle" from the same coder;
- no two lines are interleaved;
- no dongle is held by two coders at the same time.

### 3. Cooldown
| Command | Expected |
|---|---|
| `./codexion 5 3000 200 200 200 8 400 fifo` | No dongle is taken again within 400 ms of its release. No burnout. |

### 4. Scheduler comparison
| Command | Expected |
|---|---|
| `./codexion 5 3000 200 200 200 8 800 fifo` | Run once with fifo and once with edf, same parameters. Grant order may differ: edf favors the coder closest to its deadline. No burnout in either. |

### 5. Timing
Each coder spends about time_to_compile / time_to_debug / time_to_refactor
in each state (small tolerance), even under heavy contention.

### 6. Concurrency tools
valgrind --leak-check=full ./codexion 5 2000 200 200 200 5 0 fifo
valgrind --tool=helgrind ./codexion 5 2000 200 200 200 5 0 fifo
valgrind --tool=drd ./codexion 5 2000 200 200 200 5 0 edf
cc -fsanitize=thread -pthread src/*.c -Iincludes -o codexion_tsan
./codexion_tsan 5 2000 200 200 200 5 0 edf

Expected: no leaks, no data races, no lock-order warnings.

### 7. Input validation (must print an error and exit)
./codexion 5 -800 200 200 200 5 0 fifo
./codexion 5 800 abc 200 200 5 0 fifo
./codexion 5 800 200 200 200 5 0 lifo
./codexion 5 800 200 200