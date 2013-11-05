#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <sstream>
#include "renderer/Color.h"

namespace SimpleRenderer {

class PPM {
private:
	static double Clamp(double x) {
		if (x<0.0) return 0.0;
		if (x>1.0) return 1.0;
		return x;
	}
	static double GammaRev(double x) {
		return pow(Clamp(x), 1.0/2.2);
	}
	static double ToRgb(double v_0_1) {
		return static_cast<int>(v_0_1 * 255 + 0.5);
	}
public:
	static void Save(const std::string &filename, const Color *image, int width, int height) {
		std::ofstream ofs(filename.c_str());
		ofs << "P3" << std::endl;
		ofs << width << " " << height << std::endl;
		ofs << 255 << std::endl;
    int i=0;
		for (int y=0; y<height; y++) {
      std::stringstream ss;
      for (int x=0; x<width; x++) {
  			ss << ToRgb(GammaRev(image[i].x)) << " " << ToRgb(GammaRev(image[i].y)) << " " << ToRgb(GammaRev(image[i].z)) << std::endl;
        i++;
      }
      ofs << ss.str();
		}
	}
};

}
