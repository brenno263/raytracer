# Raytracer
A project implementing CPU-based raytracing in C++

authored by Brennan Seymour (brenno263@gmail.com)

# Executing
run.sh should compile and build, then run the executable to produce out.ppm.
anim.sh takes a parameter, which is number of frames to render, and spits out numbered images with the blue dielectric sphere animated.

This is currently the only renderable scene, though feel free to edit the source to make your own. In the future, I plan on creating a way to take an input file which defines the scene to render, making this more flexible without editing source code.

# Implementation

This project implements Spheres and Triangles, which can have Diffuse, Specular, or Dielectric materials which all interact differently with light.

I also went ahead and implemented multithreading. Default is 8 threads. If at least two are available, this will work just fine.
If you need to adjust, the executable is in src/ray, usage is ./ray <filename>. An available option is --cores=<num_threads>.

executable usage looks like ./ray <output.ppm>

Supported options are --cores=<num_threads> and --animate <max_frames> <frame_number>
