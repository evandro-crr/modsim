#include <string>
#include <stdexcept>
#include <sstream>
#include <random>
#include "funcoes.h"

namespace func {
  using func = double (*)(void);

  double media, dp, min, moda, max = 0;

  func parse(std::string text) {

    std::stringstream ss(text);
    std::string distribuition;
    ss >> distribuition;

    if(distribuition.compare("expo") == 0) {
      ss >> media;
      return []() { return expo(media);};
    }

    if(distribuition.compare("norm") == 0) {
      ss >> media;
      ss >> dp;
      return []() { return norm(media, dp);};
    }

    if(distribuition.compare("tria") == 0) {
      ss >> min;
      ss >> moda;
      ss >> max;
      return []() { return tria(min, moda, max);};
    }

    if(distribuition.compare("unif") == 0) {
      ss >> min;
      ss >> max;
      return []() { return unif(min, max);};
    }

    throw std::invalid_argument("Use 'expo', 'norm', 'tria' or 'unif'!");
  }

  double expo(double media) {
  	return media;
  }

  double norm(double media, double dp) {
    return media+dp;
  }

  double tria(double min, double moda, double max) {
    return min+moda+max;
  }

  double unif(double min, double max) {
    return min+max;
  }

}
