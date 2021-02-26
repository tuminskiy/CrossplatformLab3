#pragma once

#include <QMainWindow>
#include <QStringListModel>

#include "ui_mainwindow.h"

class QTcpSocket;

class MainWindow : public QMainWindow
{
  Q_OBJECT

  Ui::MainWindow ui_;
  QTcpSocket* socket_;
  QString login_;
  QStringListModel model_;

public:
  explicit MainWindow(QTcpSocket* socket, QWidget *parent = nullptr);

  void set_login(const QString& login);

private slots:
  void msg_change();

  void send_click();

  void read_response();

private:
  void append_msg(const QString& msg);
};
