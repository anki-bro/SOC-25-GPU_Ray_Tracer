main.exe: main.cpp config.h vec3.h ray.h hittable.h sphere.h render_kernel.h

	icpx -fsycl main.cpp -o main.exe
	main.exe > image.ppm