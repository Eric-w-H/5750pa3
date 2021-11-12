rm ~/PA3.*
./compile.sh
echo ""
for SCRIPT in run_tts.sh run_ts.sh run_t.sh run_q.sh
do
  echo ""
  for K in 0 1000 5000 10000 15000 20000
  do
    echo $SCRIPT $K
    qsub -v k=$K $SCRIPT
  done
done
