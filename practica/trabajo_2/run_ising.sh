for (( amount = 0; amount < 10; amount++ )); do
  progress $amount 10;
  exec/antiferro 1000 32 150000 3000 >> valores/antiferro.dat;
done
