#!/usr/bin/env bash
#SBATCH -p wacc
#SBATCH -J Generator
#SBATCH --cpus-per-task=10
#SBATCH -o Generator.out -e Generator.err 
#SBATCH -t 0-00:10:00
cd $SLURM_SUBMIT_DIR
echo "Generator"
for e in {1..10} 
do
    # let v=2**$e
    echo "========$e Time ========"
    ./generator 0 0 100 0 400 0 500 0 0 $e

done

# ./generator 0 0 100 0 400 0 500 0 0 2
# ./generator 0 0 100 0 400 0 500 0 0 3
# ./generator 0 0 100 0 400 0 500 0 0 4