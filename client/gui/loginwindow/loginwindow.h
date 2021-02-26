#pragma once

#include <QWidget>
#include "ui_loginwindow.h"

class QTcpSocket;

class LoginWindow : public QWidget
{
  Q_OBJECT

  Ui::LoginWindow ui_;
  QTcpSocket* socket_;
  QString login_;

public:
  explicit LoginWindow(QTcpSocket* socket, QWidget *parent = nullptr);

signals:
  void authorized(const QString& login);
  void reg_clicked();

private slots:
  void auth_click();

  void read_response();
};

