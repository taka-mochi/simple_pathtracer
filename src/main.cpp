#include <iostream>

#include "renderer.h"
#include "cornell_box_scene.h"
#include "vector.h"
#include "ppm.h"

using namespace std;

int main(int argc, char *argv[]) {

	int width = 640, height = 480;
	PathTracer renderer(width, height, 4, 2);
	CornellBoxScene scene;

	cerr << "begin rendering..." << endl;
	renderer.RenderScene(scene);

	cerr << "save ppm file..." << endl;
	PPM::save("result.ppm", renderer.GetResult(), width, height);

	return 0;
}
