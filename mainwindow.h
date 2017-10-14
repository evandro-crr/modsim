#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "entidade.h"
#include <memory>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:

  void on_executar_clicked();

  void on_avancar_clicked();

  void on_terminar_clicked();

  void on_cancelar_clicked();

private:

  void update_relatorio();
  void terminar_sim();

  Ui::MainWindow *ui;
  std::unique_ptr<mod::Estado> state;
};

#endif // MAINWINDOW_H
