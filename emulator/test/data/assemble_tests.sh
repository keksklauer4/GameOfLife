for i in *.asm; do
    as31 -Fbin $i
done