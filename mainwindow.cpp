#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

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
    } catch (std::invalid_argument &e) {
      QMessageBox messege{this};
      messege.setText(e.what());
      messege.exec();
    }

}

void MainWindow::on_avancar_clicked()
{
    ui->simulacao->setEnabled(false);
    double tempo = ui->t_exec->toPlainText().toDouble();

    auto end = state->run(tempo);
    update_relatorio();
    ui->simulacao->setEnabled(true);
    if (end) terminar_sim();
}

void MainWindow::update_relatorio()
{
    //QString::fromStdString(std::to_string())



    unsigned perdas1 = state->get_chegada1().get_perdas();
    unsigned entraram1 = state->get_chegada1().get_entradas();
    unsigned entraram_s1 = entraram1-perdas1;
    ui->entraram_t1->setText(QString::fromStdString(std::to_string(entraram_s1)));

    unsigned perdas2 = state->get_chegada2().get_perdas();
    unsigned entraram2 = state->get_chegada2().get_entradas();
    unsigned entraram_s2 = entraram2-perdas2;
    ui->entraram_t2->setText(QString::fromStdString(std::to_string(entraram_s2)));

    unsigned saida1 = state->get_saida().get_n_t1();
    unsigned saida2 = state->get_saida().get_n_t2();
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

    double tfalha1 = state->get_servidor1().get_t_servico();
    double tfalha2 = state->get_servidor2().get_t_servico();

    ui->t_falhas_s1->setText(QString::fromStdString(std::to_string(tfalha1)));
    ui->t_falhas_s2->setText(QString::fromStdString(std::to_string(tfalha2)));

    double simulation_time = state->get_oraculo().time();
    double tempo_total = ui->t_tempo->toPlainText().toDouble();
    if (simulation_time > tempo_total) {
        simulation_time = tempo_total;
    }

    ui->tempo_simulacao->setText(QString::fromStdString(std::to_string(simulation_time)));

    double pfalha1 = (tfalha1*100)/simulation_time;
    double pfalha2 = (tfalha2*100)/simulation_time;

    ui->p_falhas_s1->setText(QString::fromStdString(std::to_string(pfalha1)));
    ui->p_falhas_s2->setText(QString::fromStdString(std::to_string(pfalha2)));

    ui->perdidas_t1->setText(QString::fromStdString(std::to_string(perdas1)));
    ui->perdidas_t2->setText(QString::fromStdString(std::to_string(perdas2)));

    QString trocas1 = QString::fromStdString(std::to_string(state->get_chegada1().get_trocas()));
    ui->trocas_t1->setText(trocas1);
    QString trocas2 = QString::fromStdString(std::to_string(state->get_chegada2().get_trocas()));
    ui->trocas_t2->setText(trocas2);
}

void MainWindow::terminar_sim()
{
  ui->setup->setEnabled(true);
  ui->simulacao->setEnabled(false);
  state.reset(nullptr);
}

void MainWindow::on_terminar_clicked()
{
    double tempo_total = ui->t_tempo->toPlainText().toDouble();
    state->run(tempo_total);
    update_relatorio();
    terminar_sim();
}

void MainWindow::on_cancelar_clicked()
{
    state.reset(nullptr);
    ui->setup->setEnabled(true);
    ui->simulacao->setEnabled(false);
}
