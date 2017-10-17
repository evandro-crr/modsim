#ifndef ENTIDADE_H
#define ENTIDADE_H
#include "funcoes.h"
#include <set>
#include <sstream>
#include <functional>
#include <queue>

namespace mod {

  class Event{
  public:
    template<class F>
    Event(F call, double time, std::string text) : call{call}, time_{time}, text{text} {}

    void operator()() const{
      call();
    }

    operator double() const {
      return time_;

    }

    operator std::string () const {
      std::stringstream aux;
      aux << text << " time(" << time_ << ")";
      return aux.str();
    }

    bool operator<(const Event &other) const {
      return time_ < (double) other;

    }

  private:
    std::function<void()> call;
    const double time_;
    const std::string text;

  };

  class Oraculo{
  public:
    Oraculo (double tempo_total) : tempo_total{tempo_total} {}

    template<class F>
    void add_event(F call, double time, std::string text);

    bool run(double limit);

    double time() const {
      return time_;
    }

  private:
    double time_{0};
    const double tempo_total;
    std::multiset<Event> events{};

  };

  class Entidade {
  public:
    enum Tipo {
      um,
      dois,
    };

    Entidade(double begin, Tipo tipo)
      : begin{begin}, tipo_{tipo} {}

    Tipo tipo() const {
      return tipo_;
    }

    double tempo_total(double time) const {
      return time - begin;
    }

    double tempo_fila() const {
      return fila_end  - fila_begin;
    }

    void begin_fila(double time) {
      fila_begin = time;
    }
    void end_fila(double time) {
      fila_end = time;
    }

  private:
    double begin;
    double fila_begin, fila_end;
    Tipo tipo_;

  };

  class Saida{
  public:
    Saida(Oraculo &oraculo) : oraculo{oraculo} {}

    void evento_saida(Entidade entidade);

  private:
    Oraculo &oraculo;
    unsigned um{0}, dois{0};
    double tempo_um{0}, tempo_dois{0};
    double tempo_em_fila_um{0}, tempo_em_fila_dois{0};

  };

  class Servidor{
  public:
    Servidor(Oraculo &oraculo, func::func ts, func::func tef, func::func tf, Saida saida, unsigned tfe = 0)
      : oraculo{oraculo}, ts{ts}, tef{tef}, tf{tf}, saida{saida}, tfe{tfe} {programar_falha();}

    bool add_entidade(Entidade entidade);
    void executar_proximo(bool recuperacao);
    void programar_falha();

  private:
    Oraculo &oraculo;
    func::func ts, tef, tf;
    Saida saida;
    std::queue<Entidade> fila;
    unsigned tfe;
    bool em_falha{false};
    unsigned n_falhas{0};
    double t_servico{0}, t_falha{0};
    double begin_ocupado, begin_falha;
    bool ocupado{false};

  };

  class Chegada {
  public:
    Chegada(Oraculo &oraculo, func::func tec, Entidade::Tipo tipo, Servidor &primario, Servidor &secundario)
      : oraculo{oraculo}, tec{tec}, tipo{tipo}, primario{primario}, secundario{secundario} {}

    void add_chegada();

  private:
    Oraculo &oraculo;
    func::func tec;
    Entidade::Tipo tipo;
    Servidor &primario, &secundario;
    unsigned entradas{0}, trocas{0};
    inline static unsigned perdas{0};
  };

  class Estado {
  public:

    Estado(std::string tec1, std::string ts1, std::string tef1, std::string tf1, unsigned tfe1,
           std::string tec2, std::string ts2, std::string tef2, std::string tf2, unsigned tfe2,
           double tempo_total)
      : oraculo{tempo_total}, saida{oraculo},
        servidor1{oraculo, func::parse(ts1), func::parse(tef1), func::parse(tf1), saida, tfe1},
        servidor2{oraculo, func::parse(ts2), func::parse(tef2), func::parse(tf2), saida, tfe2},
        chegada1{oraculo, func::parse(tec1), Entidade::um, servidor1, servidor2},
        chegada2{oraculo, func::parse(tec2), Entidade::dois, servidor2, servidor1}
    {
      chegada1.add_chegada();
      chegada2.add_chegada();
    }

    bool run(double limit) {
      return oraculo.run(limit);
    }

   private:
    Oraculo oraculo;
    Saida saida;
    Servidor servidor1;
    Servidor servidor2;
    Chegada chegada1;
    Chegada chegada2;

  };

}

#endif // ENTIDADE_H
