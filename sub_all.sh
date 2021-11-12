rm ~/PA3.*
./compile.sh
echo ""
for SCRIPT in run_q.sh run_t.sh
do
  echo $SCRIPT
  for K in 0 1667 2000 2500 5000
  do
    for M in 0 1 2
    do
      echo $M $K
      qsub -v k=$K,m=$M $SCRIPT
    done
  done
done
