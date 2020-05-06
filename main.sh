#!/usr/bin/env bash
#SBATCH -p wacc
#SBATCH -J Generator
#SBATCH --gres=gpu:1 --cpus-per-task=20
#SBATCH -o Generator.out -e Generator.err 
#SBATCH -t 0-00:10:00

./generator 0 0 100 0 400 0 500 0 0 1
./generator 0 0 100 0 400 0 500 0 0 2
./generator 0 0 100 0 400 0 500 0 0 3
./generator 0 0 100 0 400 0 500 0 0 4