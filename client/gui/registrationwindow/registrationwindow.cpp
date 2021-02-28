#include "registrationwindow.h"
#include "definitions/protocol.h"
#include "factory/factory.h"

#include <QMessageBox>
#include <QTcpSocket>
#include <QRegExpValidator>
#include <QRegExp>

using Factory = protocol::Factory<QByteArray>;

RegistrationWindow::RegistrationWindow(QTcpSocket* socket, QWidget *parent)
  : QWidget(parent)
  , ui_()
  , socket_(socket)
  , validator_(new QRegExpValidator(QRegExp{"^[^\\W\\s]+$"}, this))
{
  ui_.setupUi(this);

  ui_.leLogin->setMaxLength(protocol::block::Login);
  ui_.lePassword->setMaxLength(protocol::block::Password);

  ui_.leLogin->setValidator(validator_);
  ui_.lePassword->setValidator(validator_);

  connect(ui_.bConfirm, &QPushButton::clicked,
          this, &RegistrationWindow::confirm_clicked);
}

void RegistrationWindow::confirm_clicked()
{
  const auto login = ui_.leLogin->text();
  const auto password = ui_.lePassword->text();

  if (login.isEmpty() || password.isEmpty()) {
    QMessageBox::critical(this, "Critical", "All fields must been filled");
    return;
  }

  protocol::RegRequest req;
  req.set_login(login.toStdString());
  req.set_password(password.toStdString());

  connect(socket_, &QTcpSocket::readyRead,
          this, &RegistrationWindow::read_response);

  socket_->write(Factory::serialize(req));
}

void RegistrationWindow::read_response()
{
  disconnect(socket_, &QTcpSocket::readyRead,
             this, &RegistrationWindow::read_response);

  const auto bytes = socket_->readAll();
  const auto header = Factory::get_header(bytes);

  if (header.command() != protocol::TypeCommand::RegistrationResponse) {
    QMessageBox::critical(this, "Critical", "Response isn't RegistrationResponse");
    return;
  }

  const auto data = Factory::deserialize<protocol::RegResponse>(bytes);

  if (data.status() != protocol::Status::Ok) {
    QMessageBox::critical(this, "Critical", "User with this login already exists");
    return;
  }

  QMessageBox::information(this, "Info", "Registration completed successfully ");
  close();
}
