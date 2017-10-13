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
  void on_pushButton_clicked();

  void on_run_clicked();

private:
  Ui::MainWindow *ui;
  std::unique_ptr<mod::Estado> state;
};

#endif // MAINWINDOW_H
