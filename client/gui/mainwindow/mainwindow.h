#pragma once

#include <QMainWindow>

#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() = default;

private:
  Ui::MainWindow ui_;
};
