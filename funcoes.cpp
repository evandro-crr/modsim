#include "funcoes.h"
#include <stdexcept>
#include <sstream>
#include <random>
#include <cmath>

namespace func {

  /* Using random device as seed */
    std::random_device rd;
    std::mt19937 generator(rd());

    /* Receives a string in form 'distribution value value' with the name
       of the distribution and its values separeted by spaces
       
       Accepted strings are:
            'expo value';
            'norm value value';
            'tria value value value';
            'unif value value';
     */
    func parse(std::string text) {
        double media, dp, min, moda, max = 0;

        std::stringstream ss(text);
        std::string distribution;
        ss >> distribution;

        if(distribution.compare("expo") == 0) {
            ss >> media;
            return [=]() { return expo(media);};
        }

        if(distribution.compare("norm") == 0) {
            ss >> media;
            ss >> dp;
            return [=]() { return norm(media, dp);};
        }

        if(distribution.compare("tria") == 0) {
            ss >> min;
            ss >> moda;
            ss >> max;
            return [=]() { return tria(min, moda, max);};
        }

        if(distribution.compare("unif") == 0) {
            ss >> min;
            ss >> max;
            return [=]() { return unif(min, max);};
        }

        if (distribution.compare("cons") == 0) {
            ss >> moda;
            return [=]() {  return moda;};
        }

        throw std::invalid_argument("Use 'expo', 'norm', 'tria', 'unif' or 'cons'!");
    }

    double expo(double media) {
        std::exponential_distribution<> exponential(media);
        return exponential(generator);
    }

    double norm(double media, double dp) {
        std::normal_distribution<> normal(media, dp);
        return normal(generator);
    }

    double tria(double min, double moda, double max) {
        double u = unif(0, 1.0);
        if(0 <= u && u <= (moda-min)/(max-min))
            return min + std::sqrt(u * (moda - min) * (max - min));

        if((moda-min)/(max-min) < u && u <= 1)
            return max - std::sqrt((1 - u) * (max - moda) * (max - min));
    }

    double unif(double min, double max) {
        std::uniform_real_distribution<double> uniform(min, max);
        return uniform(generator);
    }

}
