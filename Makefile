all: PerlinNoise.o
	g++ main.cpp Bezier.cpp Street.cpp GenerateImage.cpp PerlinNoise.o -Wall -O3 -o generator -fopenmp -L/usr/local/cuda/lib64 -lcuda -lcudart
	sbatch main.sh
all_notCUDA:
	g++ main_withoutCUDA.cpp Bezier.cpp Street.cpp GenerateImage.cpp PerlinNoise.cpp -Wall -O3 -o generator -fopenmp
	sbatch main.sh
test_vector:
	g++ ./Testing-script/VectorTesting.cpp -Wall -O3 -o vector_testing
test_bezier:
	g++ ./Testing-script/BezierTesting.cpp Bezier.cpp -Wall -O3 -o bezier_testing
test_generateimage:
	g++ ./Testing-script/GenerateImageTesting.cpp Bezier.cpp GenerateImage.cpp -Wall -O3 -o generateimage_testing
test_perlin: PerlinNoise.o
	g++ ./Testing-script/PerlinNoiseTesting.cpp PerlinNoise.o -Wall -O3 -o perlin_testing -L/usr/local/cuda/lib64 -lcuda -lcudart
	sbatch PerlinRun.sh
clean:
	rm -f bezier_testing vector_testing generateimage_testing output.obj generator perlin_testing *.err *.out PerlinNoise.o PerlinTest.obj

PerlinNoise.o: PerlinNoise.cu 
	nvcc PerlinNoise.cu -Xcompiler -O3 -Xcompiler -Wall -Xptxas -O3 -c