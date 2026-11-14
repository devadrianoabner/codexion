#!/bin/bash
BIN=./codexion
RUNS=5
FAIL=0

make re > /dev/null 2>&1 || { echo "COMPILATION FAILED"; exit 1; }

# run <name> <expect: burn|noburn> <args...>
run_case()
{
	name=$1; expect=$2; shift 2
	bad=0
	for i in $(seq 1 $RUNS); do
		out=$(timeout 60 $BIN "$@" 2>&1); code=$?
		if [ $code -eq 124 ]; then bad=$((bad+1)); continue; fi
		burned=$(echo "$out" | grep -c "burned out")
		if [ "$expect" = "burn" ]; then
			[ "$burned" -eq 1 ] && [ "$(echo "$out" | tail -1 | grep -c 'burned out')" -eq 1 ] || bad=$((bad+1))
		else
			[ "$burned" -eq 0 ] || bad=$((bad+1))
		fi
		# every "is compiling" must follow two "has taken a dongle" of same coder
		echo "$out" | awk '
			{ id=$2; l2[id]=l1[id]; l1[id]=$0 }
			/is compiling/ { if (l2[id] !~ /has taken a dongle/) err=1 }
			END { exit err }' || bad=$((bad+1))
	done
	if [ $bad -eq 0 ]; then echo "PASS  $name"; else echo "FAIL  $name ($bad/$RUNS runs)"; FAIL=1; fi
}

# invalid: must fail (exit != 0) and print nothing on stdout
run_invalid()
{
	out=$($BIN "$@" 2>/dev/null); code=$?
	if [ $code -ne 0 ] && [ -z "$out" ]; then echo "PASS  invalid: $*"
	else echo "FAIL  invalid: $*"; FAIL=1; fi
}

echo "== Easy =="
run_case "1 coder must burn"          burn   1 800 200 200 200 10 0 fifo
run_case "5 2000 fifo"                noburn 5 2000 200 200 200 10 0 fifo
run_case "5 2000 edf"                 noburn 5 2000 200 200 200 7 0 edf
echo "== Less easy =="
run_case "5 500 must burn"            burn   5 500 200 200 200 10 0 fifo
echo "== Medium =="
run_case "cooldown 400 fifo"          noburn 5 3000 200 200 200 10 400 fifo
run_case "cooldown 800 fifo"          noburn 5 3000 200 200 200 10 800 fifo
run_case "cooldown 800 edf"           noburn 5 3000 200 200 200 10 800 edf
echo "== Invalid input =="
run_invalid 5 -800 200 200 200 5 0 fifo
run_invalid 5 800 abc 200 200 5 0 fifo
run_invalid 5 800 200 200 200 5 0 lifo
run_invalid 5 800 200 200
run_invalid 0 800 200 200 200 5 0 fifo
run_invalid 5 800 200 200 200 5 0 fifo extra

echo "== Memory / threads (slow) =="
vg() { valgrind -q --error-exitcode=42 "$@" > /dev/null 2>&1; }
vg --leak-check=full --errors-for-leak-kinds=all $BIN 5 5000 200 200 200 3 0 fifo \
	&& echo "PASS  valgrind leaks" || { echo "FAIL  valgrind leaks"; FAIL=1; }
vg --tool=helgrind $BIN 5 5000 200 200 200 3 0 fifo \
	&& echo "PASS  helgrind" || { echo "FAIL  helgrind"; FAIL=1; }
vg --tool=drd $BIN 5 5000 200 200 200 3 0 edf \
	&& echo "PASS  drd" || { echo "FAIL  drd"; FAIL=1; }
cc -fsanitize=thread -pthread -Iincludes src/*.c -o /tmp/codexion_tsan \
	&& timeout 60 /tmp/codexion_tsan 5 5000 200 200 200 3 0 edf 2>&1 >/dev/null | grep -q "ThreadSanitizer" \
	&& { echo "FAIL  tsan"; FAIL=1; } || echo "PASS  tsan"

norminette src includes > /dev/null 2>&1 && echo "PASS  norminette" || { echo "FAIL  norminette"; FAIL=1; }

[ $FAIL -eq 0 ] && echo "ALL OK" || echo "SOME TESTS FAILED"