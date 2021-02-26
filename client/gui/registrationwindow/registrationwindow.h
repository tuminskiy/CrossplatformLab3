#pragma once

#include "ui_registrationwindow.h"

class QTcpSocket;

class RegistrationWindow : public QWidget
{
  Q_OBJECT

  Ui::RegistrationWindow ui_;
  QTcpSocket* socket_;

public:
  explicit RegistrationWindow(QTcpSocket* socket, QWidget *parent = nullptr);


private slots:
  void confirm_clicked();

  void read_response();
};

