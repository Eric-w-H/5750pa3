
echo "compile tts"
gcc -Wall -Wextra -Werror -Og -m32 -o ubench_tts lock.s ubench.c lock_s.c -lpthread
echo "compile ts"
gcc -Wall -Wextra -Werror -Og -m32 -o ubench_ts ts.s ubench.c lock_s.c -lpthread
echo "compile ticket"
gcc -Wall -Wextra -Werror -Og -m32 -o ubench_t lock.s ubench-t.c ticket.c -lpthread
echo "compile queue"
gcc -Wall -Wextra -Werror -Og -m32 -o ubench_q lock.s ubench-q.c queue.c -lpthread
