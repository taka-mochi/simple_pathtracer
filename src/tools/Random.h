#pragma once

#include <climits>

namespace SimpleRenderer {

class Random {
public:
  Random(const unsigned int seed) {
    unsigned int s = seed;
    for (int i = 1; i <= 4; i++){
      m_seed[i-1] = s = 1812433253U * (s^(s>>30)) + i;
    }
  }

  unsigned int next() {
    const unsigned int t = m_seed[0] ^ (m_seed[0] << 11);
		m_seed[0] = m_seed[1]; 
		m_seed[1] = m_seed[2];
		m_seed[2] = m_seed[3];
		return m_seed[3] = (m_seed[3] ^ (m_seed[3] >> 19)) ^ (t ^ (t >> 8)); 
  }

  double nextDouble() {
    return static_cast<double>(next())/UINT_MAX;
  }

private:
  unsigned int m_seed[4];
};

}
