main.exe: main.cpp config.h vec3.h ray.h hittable.h sphere.h color.h camera.h hittable_list.h
	g++ main.cpp -O3 -pthread -o main.exe
run: main.exe
	./main.exe > image.ppm
