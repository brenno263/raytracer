#include <iostream>
#include <memory>
#include <ctime>
#include <fstream>
#include <pthread.h>
#include <unistd.h>
#include <cstring>

#include "utils.h"
#include "vec3.h"
#include "ray.h"
#include "collision.h"
#include "material.h"
#include "shape.h"
#include "plane.h"
#include "triangle.h"
#include "sphere.h"
#include "camera.h"
#include "tilecounter.h"

void writeImg(int width, int height, Vec3 *imgbuf, char *path)
{
	std::streambuf *buf;
	std::ofstream of;
	if (path == NULL)
	{
		buf = std::cout.rdbuf();
	} else
	{
		of.open(path);
		buf = of.rdbuf();
	}

	std::ostream out(buf);

	out << "P3\n" << width << "\n" << height << "\n255" << std::endl;

	for (int i = 0; i < width * height; i++)
	{
		out << imgbuf[i] << std::endl;
	}

	of.close();
}

Vec3 get_color(const Ray &ray, Shape &shape, int depth)
{
	if (depth <= 0) return Vec3(0, 0, 0);

	Collision coll;
	if (shape.getCollision(ray, 0.001, 1000, coll))
	{
		Ray reflection;
		Vec3 attenuation;

		if (coll.mat->scatter(ray, coll, attenuation, reflection))
		{
			return get_color(reflection, shape, depth - 1).attenuate(attenuation);
		}
	}

	Vec3 unit_direction = ray.direction().normalized();
	auto t = 0.5 * (unit_direction.y() + 1.0);
	return Vec3(1.0, 1.0, 1.0) * (1.0 - t) + Vec3(0.4, 0.7, 1) * t;
}

Vec3 gammaCorrect(const Vec3 &color)
{
	return Vec3(sqrt(color.x()), sqrt(color.y()), sqrt(color.z()));
}

double lerp(double x0, double x1, double y0, double y1, double t)
{
	t = (t - x0) / (x1 - x0);
	return y0 * (1 - t) + y1 * t;
}

typedef struct thread_info
{
	int id;
	Vec3 *imgbuf;
	Camera *camera;
	ShapeList *world;
	int width;
	int height;
	int ss;
	int recursive_depth;

	//should change
	bool done;
	int x_min;
	int x_max;
	int y_min;
	int y_max;
} ThreadInfo;

void *renderLine(void *t_info_struct)
{
	ThreadInfo *t_info = static_cast<ThreadInfo *>(t_info_struct);
	srand(time(NULL) + t_info->id);
	for (int y = t_info->y_min; y < t_info->y_max; y++)
	{
		for (int x = t_info->x_min; x < t_info->x_max; x++)
		{
			Collision coll;
			Vec3 color;

			for (int s = 0; s < t_info->ss; s++)
			{
				Ray ray = t_info->camera->getRay((double) (x + random_double()) / t_info->width,
												 (double) (y + random_double()) / t_info->height);
				color = color + get_color(ray, *t_info->world, t_info->recursive_depth);
			}
			color = color / (double) t_info->ss;

			t_info->imgbuf[y * t_info->width + x] = gammaCorrect(color);
		}
	}

	t_info->done = true;
	return NULL;
}

int main(int argc, char **argv)
{
	srand(time(NULL));

	double param = 0;
	int num_threads = 1;

	for (int i = 1; i < argc; i++)
	{
		std::string arg(argv[i]);
		if (arg.substr(0, 8) == std::string("--cores="))
		{
			num_threads = std::stoi(arg.substr(8));
		} else if (argc > i + 2 && arg == std::string("--animate"))
		{
			int count = std::stoi(std::string(argv[i + 1]));
			param = std::stoi(std::string(argv[i + 2]));
			param = lerp(0, count - 1, -1.2, 1.2, param);
		}
	}


	int width = 512;
	int height = 512;
	int tile_width = 64;
	int tile_height = 64;
	TileCounter tile_counter(ceil(width / tile_width) * ceil(height / tile_height));
	double fov = 85 * M_PI / 180;
	int ss = 24;
	int recursive_depth = 32;

	Camera camera(Vec3(0, 1, 0), Vec3(0, -0.2, 0.9), Vec3(0.5, 0.8, 0), fov, (double) width / height);

	auto diffuse_grey = std::make_shared<Diffuse>(Vec3(0.5, 0.5, 0.5));
	auto diffuse_green = std::make_shared<Diffuse>(Vec3(0.2, 0.6, 0.3));
	auto specular_gold = std::make_shared<Specular>(Vec3(0.9, 0.9, 0.2), 0.3);
	auto specular_mirror = std::make_shared<Specular>(Vec3(0.9, 0.9, 0.9), 0.02);
	auto specular_blue = std::make_shared<Specular>(Vec3(0.6, 0.7, 0.9), 0.05);
	auto dielectric_teal = std::make_shared<Dielectric>(Vec3(0.2, 0.9, 0.9), 1.3);


	ShapeList world;
	world.add(std::make_shared<Sphere>(Vec3(0, 0.5, 3), 0.5, diffuse_grey));
	world.add(std::make_shared<Sphere>(Vec3(0, -0.5, 3), 0.5, diffuse_green));
	world.add(std::make_shared<Sphere>(Vec3(0, 1.5, 3), 0.5, specular_gold));
	world.add(std::make_shared<Sphere>(Vec3(param, 0.5, 1.8), 0.6, dielectric_teal));
	world.add(std::make_shared<Sphere>(Vec3(-2, 2, 5), 2, specular_mirror));

	//floor
	world.add(std::make_shared<Triangle>(Vec3(-4, -1, 0), Vec3(4, -1, 0), Vec3(-4, -1, 5), diffuse_grey));
	world.add(std::make_shared<Triangle>(Vec3(4, -1, 0), Vec3(4, -1, 5), Vec3(-4, -1, 5), diffuse_grey));


	Vec3 *imgbuf = (Vec3 *) malloc(sizeof(Vec3) * width * height);

	pthread_t threads[num_threads];
	ThreadInfo *thread_structs[num_threads];

	//initialize thread structs, but they won't be rendering yet.
	for (int i = 0; i < num_threads; i++)
	{
		thread_structs[i] = new ThreadInfo;
		thread_structs[i]->id = i;
		thread_structs[i]->imgbuf = imgbuf;
		thread_structs[i]->camera = &camera;
		thread_structs[i]->world = &world;
		thread_structs[i]->width = width;
		thread_structs[i]->height = height;
		thread_structs[i]->ss = ss;
		thread_structs[i]->recursive_depth = recursive_depth;

		thread_structs[i]->done = true;
		thread_structs[i]->x_min = 0;
		thread_structs[i]->x_max = 0;
		thread_structs[i]->y_min = 0;
		thread_structs[i]->y_max = 0;

		pthread_create(&threads[i], NULL, renderLine, thread_structs[i]);
	}

	int thread_index = 0;
	for (int y = 0; y < height; y += tile_height)
	{
		for (int x = 0; x < width; x += tile_width, thread_index++)
		{
			int thread_ready = -1;
			while (thread_ready == -1)
			{
				for (int i = 0; i < num_threads; i++)
				{
					if (thread_structs[i]->done)
					{
						thread_ready = i;
					}
				}
				usleep(1);
			}
			pthread_join(threads[thread_ready], NULL);
			//set up values for the new thread
			ThreadInfo *t_info = thread_structs[thread_ready];
			t_info->done = false;
			t_info->y_min = y;
			t_info->y_max = std::min(y + tile_height, height);
			t_info->x_min = x;
			t_info->x_max = std::min(x + tile_width, width);

			//print that we've finished a tile
			tile_counter.increment();

			//send 'er off to work
			pthread_create(&threads[thread_ready], NULL, renderLine, thread_structs[thread_ready]);
		}
	}

	//clear threads
	for (int i = 0; i < num_threads; i++)
	{
		pthread_join(threads[i], NULL);
		delete thread_structs[i];
	}

	//export
	char *path = argc > 1 ? argv[1] : NULL;
	writeImg(width, height, imgbuf, path);

	free(imgbuf);
}
