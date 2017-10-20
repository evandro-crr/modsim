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
    double fila_begin{0}, fila_end{};
    Tipo tipo_;

  };

  class Saida{
  public:
    Saida(Oraculo &oraculo) : oraculo{oraculo} {}

    void evento_saida(Entidade entidade);

    unsigned get_n_t1() {
        return um;
    }

    unsigned get_n_t2() {
        return dois;
    }

    double get_t1() {
        return tempo_um;
    }

    double get_t2() {
        return tempo_dois;
    }

    double get_tfila1() {
        return tempo_em_fila_um;
    }

    double get_tfila2() {
        return tempo_em_fila_dois;
    }

  private:
    Oraculo &oraculo;
    unsigned um{0}, dois{0};
    double tempo_um{0}, tempo_dois{0};
    double tempo_em_fila_um{0}, tempo_em_fila_dois{0};

  };

  class Servidor{
  public:
    Servidor(Oraculo &oraculo, func::func ts, func::func tef, func::func tf, Saida &saida, unsigned tfe = 0)
      : oraculo{oraculo}, ts{ts}, tef{tef}, tf{tf}, saida{saida}, tfe{tfe} {programar_falha();}

    bool add_entidade(Entidade entidade);
    void executar_proximo(bool recuperacao);
    void programar_falha();

    unsigned get_n_falhas() {
        return n_falhas;
    }

    double get_t_falha() {
      if (em_falha)
        return t_falha + (oraculo.time() - begin_falha);
      else return t_falha;
    }

    double get_t_servico() {
      if (ocupado)
        return t_servico+(oraculo.time() - begin_ocupado);
      else return t_servico;
    }

    double get_mfila() {
      return mfila;
    }

  private:
    Oraculo &oraculo;
    func::func ts, tef, tf;
    Saida &saida;
    std::queue<Entidade> fila{};
    unsigned tfe;
    bool em_falha{false};
    unsigned n_falhas{0};
    double t_servico{0}, t_falha{0};
    double begin_ocupado{0}, begin_falha{0};
    double last_time{0}, mfila{0}, ponderacao;
    bool ocupado{false};
    bool init{false};

  };

  class Chegada {
  public:
    Chegada(Oraculo &oraculo, func::func tec, Entidade::Tipo tipo, Servidor &primario, Servidor &secundario)
      : oraculo{oraculo}, tec{tec}, tipo{tipo}, primario{primario}, secundario{secundario} {}

    void add_chegada();

    unsigned get_entradas() {
        return entradas;
    }

    unsigned get_trocas() {
        return trocas;
    }

    unsigned get_perdas() {
        return perdas;
    }

  private:
    Oraculo &oraculo;
    func::func tec;
    Entidade::Tipo tipo;
    Servidor &primario, &secundario;
    unsigned entradas{0}, trocas{0};
    unsigned perdas{0};
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

    Oraculo &get_oraculo() {
        return oraculo;
    }

    Saida &get_saida() {
        return saida;
    }

    Servidor &get_servidor1() {
        return servidor1;
    }

    Servidor &get_servidor2() {
        return servidor2;
    }

    Chegada &get_chegada1() {
        return chegada1;
    }

    Chegada &get_chegada2() {
        return chegada2;
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
