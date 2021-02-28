#pragma once

#include "ui_registrationwindow.h"

class QTcpSocket;
class QRegExpValidator;

class RegistrationWindow : public QWidget
{
  Q_OBJECT

  Ui::RegistrationWindow ui_;
  QTcpSocket* socket_;
  QRegExpValidator* validator_;

public:
  explicit RegistrationWindow(QTcpSocket* socket, QWidget *parent = nullptr);

private slots:
  void confirm_clicked();

  void read_response();
};

