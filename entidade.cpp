#include "entidade.h"
#include <iostream>

template <class F>
void mod::Oraculo::add_event(F call, double time, std::string text) {
  std::cout << "add: " << text << "(" << time << ")\n";
  events.insert(Event{call, time, text});
}

bool mod::Oraculo::run(double limit) {
  auto end = time_ + limit;

  while(time_ <= end and time_ <= tempo_total ) {
    std::cout << "time: " << time_ << " end " << end << " total " << tempo_total <<  "\n";
    auto event = *(events.begin());

    if (event > end or event > tempo_total) {
      time_ = end;
      break;
    }

    std::cout << "exec: " << (std::string)event << "\n";
    time_ = event;
    event();

    events.erase(events.begin());
  }

  return not (time_ < tempo_total);
}

void mod::Chegada::add_chegada() {
  auto event = [&]() {
    entradas++;
    Entidade entidade{oraculo.time(), tipo};

    if (not primario.add_entidade(entidade)) {
      if (not secundario.add_entidade(entidade)) {
        perdas++;
      } else trocas++;
    }
    add_chegada();
  };

  oraculo.add_event(event, oraculo.time() + tec(), "Evento Chegada");
}

bool mod::Servidor::add_entidade(Entidade entidade) {

    if (tfe == 0 or fila.size() < tfe) {
        entidade.begin_fila(oraculo.time());
        fila.push(entidade);
        executar_proximo(false);
        return true;
    }
    return false;
}

void mod::Servidor::executar_proximo(bool recuperacao) {
  if (em_falha) return;
  if (fila.empty()) {
    t_servico += oraculo.time() - begin_ocupado;
    ocupado = false;
    return;
  } else if (fila.size() == 1 or recuperacao){
    if (not ocupado) {
      ocupado = true;
      begin_ocupado = oraculo.time();
    }

    fila.front().end_fila(oraculo.time());

    auto event = [&]() {
      if (em_falha) return;
      saida.evento_saida(fila.front());
      fila.pop();
      executar_proximo(true);
    };

    oraculo.add_event(event, oraculo.time()+ts(), "Sair do servidor");
  }
}

void mod::Saida::evento_saida(Entidade entidade) {

  if (entidade.tipo() == Entidade::um) {
    um++;
    tempo_um += entidade.tempo_total(oraculo.time());
    tempo_em_fila_um += entidade.tempo_fila();
  } else {
    dois++;
    tempo_dois += entidade.tempo_total(oraculo.time());
    tempo_em_fila_dois += entidade.tempo_fila();
  }
}

void mod::Servidor::programar_falha() {

  auto event = [&] () {
    em_falha = true;
    begin_falha = oraculo.time();
    n_falhas++;

    if(ocupado) {
      ocupado = false;
      t_servico += oraculo.time() - begin_ocupado;
    }

    auto recuperacao = [&] () {
      em_falha = false;
      t_falha += oraculo.time() - begin_falha;

      executar_proximo(true);
      programar_falha();
    };

    oraculo.add_event(recuperacao, oraculo.time() + tf(), "Serdidor voltou");
  };

  oraculo.add_event(event, oraculo.time()+tef(), "Servidor em falha");

}
