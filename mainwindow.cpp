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
    ui->setup->setEnabled(false);

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

    state = std::make_unique<mod::Estado>(tec1, ts1, tef1, tf1, tfe1,
                                          tec2, ts2, tef2, tf2, tfe2,
                                          tempo_total);
}



void MainWindow::on_avancar_clicked()
{
    double tempo = ui->t_exec->toPlainText().toDouble();

    // Passa o tempo para o oráculo?
}

void MainWindow::on_terminar_clicked()
{
    // executar com o tempo total

    double tempo_total = ui->t_tempo->toPlainText().toDouble();
}

void MainWindow::on_cancelar_clicked()
{
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

    // Como parar a simulacao se estiver no meio dela?

    ui->setup->setEnabled(true);
}
