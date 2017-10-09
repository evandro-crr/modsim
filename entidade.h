#ifndef ENTIDADE_H
#define ENTIDADE_H
#include "funcoes.h"
#include <set>

namespace mod {
  class Entidade {
  public:
    enum Tipo {
      um,
      dois,
    };

    Entidade(double begin, Tipo tipo)
      : begin{begin}, tipo_{tipo} {}

    Tipo tipo() const {
      return tipo;
    }

    double tempo_total(double time) const {
      return time - begin;
    }

  private:
    double begin;
    double fila_begin, file_end;
    Tipo tipo_;

  };

  class Chegada {
  public:
    Chegada(Oraculo oraculo, func::func tec, Entidade::Tipo tipo, Servidor &primario, Servidor &secundario)
      : oraculo{oraculo}, tec{tec}, tipo{tipo}, primario{primario}, secundario{secundario} {}
  private:
    Oraculo &Oraculo;
    func::func tec;
    Entidade::Tipo tipo;
    Servidor &primario, &secundario;
    unsigned entradas{0}, trocas{0}, perdas{0};
  };

  class Servidor{
  public:
    Servidor(Oraculo &oraculo, func::func ts, func::func tef, func::func tf, Saida saida, unsigned tfe = 0)
      : oraculo{oraculo}, ts{ts}, tef{tef}, tf{tf}, saida{saida}, tfe{tfe} {}
  private:
    Oraculo &oraculo;
    func::func ts, tef, tf;
    Saida saida;
    unsigned tfe, fila{0};
    unsigned n_falhas{0};
    double t_servico{0}, t_falha{0};

  };

  class Saida{
  public:
    Saida(Oraculo &oraculo);
  private:
    Oraculo &oraculo;
    unsigned um{0}, dois{0};
    double tempo_um{0}, tempo_dois{0};
    double tempo_em_fila_um{0}, tempo_em_fila_dois{0};

  };

  class Oraculo{
  public:
    using event = void (*)(double);
  private:
    std::set<event> events;
  };
}

#endif // ENTIDADE_H
