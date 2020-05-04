all:
	g++ main.cpp Bezier.cpp Street.cpp GenerateImage.cpp -Wall -O3 -o generator -fopenmp
run:
	generator 0 0 40 20 60 20 100 0
test_vector:
	g++ ./Testing-script/VectorTesting.cpp -Wall -O3 -o vector_testing
test_bezier:
	g++ ./Testing-script/BezierTesting.cpp Bezier.cpp -Wall -O3 -o bezier_testing
test_generateimage:
	g++ ./Testing-script/GenerateImageTesting.cpp Bezier.cpp GenerateImage.cpp -Wall -O3 -o generateimage_testing
test_perlin:
	g++ ./Testing-script/PerlinNoiseTesting.cpp PerlinNoise.cpp -Wall -O3 -o perlin_testing
clean:
	rm -f bezier_testing vector_testing generateimage_testing output.obj generator perlin_testing