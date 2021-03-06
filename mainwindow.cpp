#include <QMessageBox>
#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_executar_clicked()
{
    std::string tec1 = ui->t_tec1->toPlainText().toStdString();
    std::string ts1 = ui->t_ts1->toPlainText().toStdString();
    unsigned tfe1 = (unsigned)ui->t_tfe1->toPlainText().toInt();
    std::string tef1 = ui->t_tef1->toPlainText().toStdString();
    std::string tf1 = ui->t_tf1->toPlainText().toStdString();

    std::string tec2 = ui->t_tec2->toPlainText().toStdString();
    std::string ts2 = ui->t_ts2->toPlainText().toStdString();
    unsigned tfe2 = (unsigned)ui->t_tfe2->toPlainText().toInt();
    std::string tef2 = ui->t_tef2->toPlainText().toStdString();
    std::string tf2 = ui->t_tf2->toPlainText().toStdString();

    double tempo_total = ui->t_tempo->toPlainText().toDouble();

    try {
      state = std::make_unique<mod::Estado>(tec1, ts1, tef1, tf1, tfe1,
                                            tec2, ts2, tef2, tf2, tfe2,
                                            tempo_total);
      ui->setup->setEnabled(false);
      ui->simulacao->setEnabled(true);
      pause = true;
      ui->cancelar->setEnabled(true);
      clean_relatorio();
    } catch (std::invalid_argument &e) {
      QMessageBox messege{this};
      messege.setText(e.what());
      messege.exec();
    }

}

void MainWindow::update_relatorio()
{
    //QString::fromStdString(std::to_string())

    double tserv1 = state->get_servidor1().get_t_servico();
    double tserv2 = state->get_servidor2().get_t_servico();

    double tempo1 = state->get_saida().get_t1();
    double tempo2 = state->get_saida().get_t2();
    unsigned saida1 = state->get_saida().get_n_t1();
    unsigned saida2 = state->get_saida().get_n_t2();
    ui->t_medio_sistema_t1->setText(QString::fromStdString(std::to_string(tempo1/saida1)));
    ui->t_medio_sistema_t2->setText(QString::fromStdString(std::to_string(tempo2/saida2)));

    double tfila1 = state->get_saida().get_tfila1();
    double tfila2 = state->get_saida().get_tfila2();
    ui->t_medio_fila_t1->setText(QString::fromStdString(std::to_string(tfila1/saida1)));
    ui->t_medio_fila_t2->setText(QString::fromStdString(std::to_string(tfila2/saida2)));

    unsigned perdas1 = state->get_chegada1().get_perdas();
    unsigned entraram1 = state->get_chegada1().get_entradas();
    unsigned entraram_s1 = entraram1-perdas1;
    ui->entraram_t1->setText(QString::fromStdString(std::to_string(entraram_s1)));

    unsigned perdas2 = state->get_chegada2().get_perdas();
    unsigned entraram2 = state->get_chegada2().get_entradas();
    unsigned entraram_s2 = entraram2-perdas2;
    ui->entraram_t2->setText(QString::fromStdString(std::to_string(entraram_s2)));

    ui->sairam_t1->setText(QString::fromStdString(std::to_string(saida1)));
    ui->sairam_t2->setText(QString::fromStdString(std::to_string(saida2)));

    unsigned permanecem1 = entraram_s1-saida1;
    unsigned permanecem2 = entraram_s2-saida2;
    ui->permanecem_t1->setText(QString::fromStdString(std::to_string(permanecem1)));
    ui->permanecem_t2->setText(QString::fromStdString(std::to_string(permanecem2)));

    unsigned nfalhas1 = state->get_servidor1().get_n_falhas();
    unsigned nfalhas2 = state->get_servidor2().get_n_falhas();
    ui->n_falhas_s1->setText(QString::fromStdString(std::to_string(nfalhas1)));
    ui->n_falhas_s2->setText(QString::fromStdString(std::to_string(nfalhas2)));

    double tfalha1 = state->get_servidor1().get_t_falha();
    double tfalha2 = state->get_servidor2().get_t_falha();

    ui->t_falhas_s1->setText(QString::fromStdString(std::to_string(tfalha1)));
    ui->t_falhas_s2->setText(QString::fromStdString(std::to_string(tfalha2)));

    double simulation_time = state->get_oraculo().time();
    double tempo_total = ui->t_tempo->toPlainText().toDouble();
    if (simulation_time > tempo_total) {
        simulation_time = tempo_total;
    }

    ui->t_media_ocupacao_s1->setText(QString::fromStdString(std::to_string((tserv1)/(simulation_time-tfalha1))));
    ui->t_media_ocupacao_s2->setText(QString::fromStdString(std::to_string((tserv2)/(simulation_time-tfalha2))));

    ui->tempo_simulacao->setText(QString::fromStdString(std::to_string(simulation_time)));

    double pfalha1 = (tfalha1)/simulation_time;
    double pfalha2 = (tfalha2)/simulation_time;

    ui->p_falhas_s1->setText(QString::fromStdString(std::to_string(pfalha1)));
    ui->p_falhas_s2->setText(QString::fromStdString(std::to_string(pfalha2)));

    ui->perdidas_t1->setText(QString::fromStdString(std::to_string(perdas1)));
    ui->perdidas_t2->setText(QString::fromStdString(std::to_string(perdas2)));

    QString trocas1 = QString::fromStdString(std::to_string(state->get_chegada1().get_trocas()));
    ui->trocas_t1->setText(trocas1);
    QString trocas2 = QString::fromStdString(std::to_string(state->get_chegada2().get_trocas()));
    ui->trocas_t2->setText(trocas2);


    double mfila1 = state->get_servidor1().get_mfila();
    double mfila2 = state->get_servidor2().get_mfila();
    QString m_fila1 = QString::fromStdString(std::to_string(mfila1));
    QString m_fila2 = QString::fromStdString(std::to_string(mfila2));
    ui->n_medio_filas_s1->setText(m_fila1);
    ui->n_medio_filas_s2->setText(m_fila2);
}

void MainWindow::clean_setup()
{
    ui->t_tec1->setText("");
    ui->t_tec2->setText("");
    ui->t_tef1->setText("");
    ui->t_tef2->setText("");
    ui->t_tempo->setText("");
    ui->t_tf1->setText("");
    ui->t_tf2->setText("");
    ui->t_tfe1->setText("");
    ui->t_tfe2->setText("");
    ui->t_ts1->setText("");
    ui->t_ts2->setText("");
}

void MainWindow::clean_relatorio()
{
    ui->t_medio_sistema_t1->setText("--");
    ui->t_medio_sistema_t2->setText("--");

    ui->t_medio_fila_t1->setText("--");
    ui->t_medio_fila_t2->setText("--");

    ui->entraram_t1->setText("--");
    ui->entraram_t2->setText("--");

    ui->sairam_t1->setText("--");
    ui->sairam_t2->setText("--");

    ui->permanecem_t1->setText("--");
    ui->permanecem_t2->setText("--");

    ui->n_falhas_s1->setText("--");
    ui->n_falhas_s2->setText("--");

    ui->t_falhas_s1->setText("--");
    ui->t_falhas_s2->setText("--");

    ui->t_media_ocupacao_s1->setText("--");
    ui->t_media_ocupacao_s2->setText("--");

    ui->tempo_simulacao->setText("--");

    ui->p_falhas_s1->setText("--");
    ui->p_falhas_s2->setText("--");

    ui->perdidas_t1->setText("--");
    ui->perdidas_t2->setText("--");

    ui->trocas_t1->setText("--");
    ui->trocas_t2->setText("--");

    ui->n_medio_filas_s1->setText("--");
    ui->n_medio_filas_s2->setText("--");
}

void MainWindow::on_terminar_clicked()
{
    ui->simulacao->setEnabled(false);
    pause = false;
    double tempo_total = ui->t_tempo->toPlainText().toDouble();
    std::function<void()> update = [&]() {update_relatorio();};
    worker = new Worker(state, ui, update, tempo_total);

    worker->start();
}

void MainWindow::on_avancar_clicked()
{
    ui->simulacao->setEnabled(false);
    pause = false;
    double tempo = ui->t_exec->toPlainText().toDouble();
    std::function<void()> update = [&]() {update_relatorio();};
    worker = new Worker(state, ui, update, tempo);

    worker->start();
    update_relatorio();
}

void MainWindow::on_pausar_cancelar_clicked()
{
    if(!pause) {
        pause = true;
        ui->simulacao->setEnabled(true);
        state->get_oraculo().pause();
        while(!worker->isFinished()) {}
        update_relatorio();
        state->get_oraculo().unpause();
    } else {
        ui->simulacao->setEnabled(false);
        ui->cancelar->setEnabled(false);
        ui->setup->setEnabled(true);
        //clean_setup();
    }
}
