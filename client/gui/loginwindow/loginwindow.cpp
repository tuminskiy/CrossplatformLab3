#include "loginwindow.h"
#include "definitions/protocol.h"
#include "factory/factory.h"

#include <QMessageBox>
#include <QTcpSocket>

using Factory = protocol::Factory<QByteArray>;

LoginWindow::LoginWindow(QTcpSocket* socket, QWidget *parent)
  : QWidget(parent)
  , ui_()
  , socket_(socket)
{
  ui_.setupUi(this);

  ui_.leLogin->setMaxLength(protocol::block::Login);
  ui_.lePassword->setMaxLength(protocol::block::Password);

  connect(ui_.bAuth, &QPushButton::clicked,
          this, &LoginWindow::auth_click);

  connect(ui_.bReg, &QPushButton::clicked,
          this, &LoginWindow::reg_clicked);
}

void LoginWindow::auth_click()
{
  const auto login = ui_.leLogin->text();
  const auto password = ui_.lePassword->text();

  if (login.isEmpty() || password.isEmpty()) {
    QMessageBox::critical(this, "Critical", "All fields must been filled");
    return;
  }

  login_ = login;

  protocol::AuthRequest request;
  request.set_login(login.toStdString());
  request.set_password(password.toStdString());

  connect(socket_, &QTcpSocket::readyRead,
          this, &LoginWindow::read_response);

  socket_->write(Factory::serialize(request));
}

void LoginWindow::read_response()
{
  disconnect(socket_, &QTcpSocket::readyRead,
             this, &LoginWindow::read_response);

  const auto bytes = socket_->readAll();
  const auto header = Factory::get_header(bytes);

  if (header.command() != protocol::TypeCommand::AuthorisationResponse) {
    QMessageBox::critical(this, "Critical", "Response isn't AuthorisationResponse");
    return;
  }

  const auto data = Factory::deserialize<protocol::AuthResponse>(bytes);

  if (data.status() != protocol::Status::Ok) {
    QMessageBox::critical(this, "Critical", "Wrong login or password");
    return;
  }

  emit authorized(login_);
  close();
}
