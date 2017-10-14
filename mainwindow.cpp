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

void MainWindow::update_relatorio() {}

void MainWindow::terminar_sim() {
  ui->setup->setEnabled(true);
  ui->simulacao->setEnabled(false);
}

void MainWindow::on_terminar_clicked()
{
    // executar com o tempo total

    double tempo_total = ui->t_tempo->toPlainText().toDouble();
    state->run(tempo_total);
    terminar_sim();
}

void MainWindow::on_cancelar_clicked()
{
    /*
    ui->t_tec1->setText("");
    ui->t_ts1->setText("");
    ui->t_tfe1->setText("");
    ui->t_tef1->setText("");
    ui->t_tf1->setText("");

    ui->t_tec2->setText("");
    ui->t_ts2->setText("");
    ui->t_tfe2->setText("");
    ui->t_tef2->setText("");
    ui->t_tf2->setText("");

    ui->t_tempo->setText("");
    ui->t_path->setText("");
    ui->t_exec->setText("");
    */

    ui->setup->setEnabled(true);
    ui->simulacao->setEnabled(false);
}
