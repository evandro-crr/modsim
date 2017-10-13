#ifndef FUNCOES_H
#define FUNCOES_H
#include <string>
#include <functional>

namespace func {
    using func = std::function<double()>;

    func parse(std::string text); 
    double expo(double media);
    double norm(double media, double dp);
    double tria(double min, double moda, double max);
    double unif(double min, double max);

}


#endif // FUNCOES_H
