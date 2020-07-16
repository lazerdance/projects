# object files to build from source
OBJ=main.o simple_ray/ray_math.o simple_ray/ray.o simple_ray/bmp.o simple_ray/ray_default_scene.o

# dependencies
DEP=simple_ray/ray_math.h simple_ray/ray.h simple_ray/bmp.h simple_ray/ray_settings.h simple_ray/ray_default_scene.h

# options
FLAGS=-g

all: raytracer

raytracer: $(OBJ)
	gcc -o $@ $^ -lm 

%.o: %.c $(DEP)
	gcc $(FLAGS) -Wall -std=c11 -c $< -o $@

.PHONY: clean

clean:
	rm -f raytracer *.o simple_ray/*.o *.bmp
