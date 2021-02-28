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
  bool scrolled_to_end_;

public:
  explicit MainWindow(QTcpSocket* socket, QWidget *parent = nullptr);

  void set_login(const QString& login);

private slots:
  void msg_change();

  void send_click();

  void read_response();

  void chat_scroll(int value);

private:
  void append_msg(const QString& login, const QString& msg);
};
