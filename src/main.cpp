#include <iostream>
#include <sstream>
#include <ctime>

#include "renderer/Renderer.h"
#include "scenes/CornellBoxScene.h"
#include "scenes/TestScene.h"
#include "tools/Vector.h"
#include "tools/PPM.h"

using namespace std;
using namespace SimpleRenderer;

static const int supersampling = 4;

static const int startSample = 2;
static const int endSample = 1024;
static const int stepSample = 2;

class SavePPM_callback : public PathTracer::RenderingFinishCallback {
  int w,h;
public:
  SavePPM_callback(int width, int height):w(width),h(height){};
  void operator()(int samples, const Color *img) {
  	cerr << "save ppm file for sample " << samples << " ..." << endl;
    char name[1024];
    sprintf(name, "result_qbvh_torii_%04d_%dx%d.ppm", samples, supersampling, supersampling);
    clock_t begin,end;
    begin = clock();
    PPM::Save(name, img, w, h);
    end = clock();
    cerr << "saving time = " << (double)(end - begin)/CLOCKS_PER_SEC << endl;
  };
};

int main(int argc, char *argv[]) {

	int width = 640, height = 480;
	//PathTracer renderer(width, height, 16, 4);
  SavePPM_callback callback(width, height);
  PathTracer renderer(width, height, startSample, endSample, stepSample, supersampling, &callback);
	TestScene scene;
  //CornellBoxScene scene;

	cerr << "begin rendering..." << endl;
  clock_t t1 = clock();
	renderer.RenderScene(scene);
  cerr << "total time = "  << (1.0/60*(clock()-t1)/CLOCKS_PER_SEC) << " (min)." << endl;

	return 0;
}
