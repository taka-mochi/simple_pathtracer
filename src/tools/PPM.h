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
	static double clamp(double x) {
		if (x<0.0) return 0.0;
		if (x>1.0) return 1.0;
		return x;
	}
	static double gammaRev(double x) {
		return pow(clamp(x), 1.0/2.2);
	}
	static double to_rgb(double v_0_1) {
		return static_cast<int>(v_0_1 * 255 + 0.5);
	}
public:
	static void save(const std::string &filename, const Color *image, int width, int height) {
		std::ofstream ofs(filename.c_str());
		ofs << "P3" << std::endl;
		ofs << width << " " << height << std::endl;
		ofs << 255 << std::endl;
    int i=0;
		for (int y=0; y<height; y++) {
      std::stringstream ss;
      for (int x=0; x<width; x++) {
  			ss << to_rgb(gammaRev(image[i].x)) << " " << to_rgb(gammaRev(image[i].y)) << " " << to_rgb(gammaRev(image[i].z)) << std::endl;
        i++;
      }
      ofs << ss.str();
		}
	}
};

}
