#include <iostream>
#include <sstream>

#include "renderer.h"
#include "cornell_box_scene.h"
#include "vector.h"
#include "ppm.h"

using namespace std;

class SavePPM_callback : public PathTracer::RenderingFinishCallback {
  int w,h;
public:
  SavePPM_callback(int width, int height):w(width),h(height){};
  void operator()(int samples, const Color *img) {
  	cerr << "save ppm file for sample " << samples << " ..." << endl;
    char name[1024];
    sprintf(name, "result_%04d.ppm", samples);
    PPM::save(name, img, w, h);
  };
};

int main(int argc, char *argv[]) {

	int width = 640, height = 480;
	//PathTracer renderer(width, height, 16, 4);
  SavePPM_callback callback(width, height);
  PathTracer renderer(width, height, 1, 1000, 4, &callback);
	CornellBoxScene scene;

	cerr << "begin rendering..." << endl;
	renderer.RenderScene(scene);


	return 0;
}
