#!/usr/bin/env bash
#SBATCH -p wacc
#SBATCH -J Perlin
#SBATCH --gres=gpu:1 -c 1
#SBATCH -o Perlin.out -e Perlin.err 
#SBATCH -t 0-00:30:00

./perlin_testing