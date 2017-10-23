#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <memory>
#include "entidade.h"
#include "ui_mainwindow.h"

namespace Ui {
  class MainWindow;
}

class Worker : public QThread
{
    Q_OBJECT
    std::unique_ptr<mod::Estado> &state;
    double &tempo;
    Ui::MainWindow *ui;
    std::function<void()> update;

    void run() override {
        auto end = state->run(tempo);
        if(end) {
            ui->setup->setEnabled(true);
            ui->simulacao->setEnabled(false);
            ui->cancelar->setEnabled(false);
            update();
        } else {
            ui->simulacao->setEnabled(true);
            ui->cancelar->setEnabled(true);
        }
        emit resultReady("Done");
    }

public:
    Worker(std::unique_ptr<mod::Estado> &state, Ui::MainWindow *ui, std::function<void()> update, double &tempo) :
        state{state}, ui{ui}, update{update}, tempo{tempo} {}

signals:
    void resultReady(const QString &s);
};

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  void update_relatorio();

private slots:

  void on_executar_clicked();

  void on_avancar_clicked();

  void on_terminar_clicked();

  void on_pausar_cancelar_clicked();

private:

  void clean_setup();
  void clean_relatorio();

  Ui::MainWindow *ui;
  std::unique_ptr<mod::Estado> state;
  Worker *worker;
  bool pause{false};
};

#endif // MAINWINDOW_H
