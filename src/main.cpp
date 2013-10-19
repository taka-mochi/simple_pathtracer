#include <iostream>
#include <sstream>
#include <ctime>

#include "renderer.h"
#include "cornell_box_scene.h"
#include "vector.h"
#include "ppm.h"

using namespace std;

static const int supersampling = 4;

class SavePPM_callback : public PathTracer::RenderingFinishCallback {
  int w,h;
public:
  SavePPM_callback(int width, int height):w(width),h(height){};
  void operator()(int samples, const Color *img) {
  	cerr << "save ppm file for sample " << samples << " ..." << endl;
    char name[1024];
    sprintf(name, "result_%04d_%dx%d.ppm", samples, supersampling, supersampling);
    clock_t begin,end;
    begin = clock();
    PPM::save(name, img, w, h);
    end = clock();
    cerr << "saving time = " << (double)(end - begin)/CLOCKS_PER_SEC << endl;
  };
};

int main(int argc, char *argv[]) {

	int width = 320, height = 240;
	//PathTracer renderer(width, height, 16, 4);
  SavePPM_callback callback(width, height);
  PathTracer renderer(width, height, 16, 16, supersampling, &callback);
	CornellBoxScene scene;

	cerr << "begin rendering..." << endl;
	renderer.RenderScene(scene);


	return 0;
}
