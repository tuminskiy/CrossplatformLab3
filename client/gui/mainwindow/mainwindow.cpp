#include "mainwindow.h"
#include "definitions/protocol.h"
#include "factory/factory.h"

#include <QTcpSocket>
#include <QMessageBox>
#include <QTextStream>

using Factory = protocol::Factory<QByteArray>;

MainWindow::MainWindow(QTcpSocket* socket, QWidget *parent)
  : QMainWindow(parent)
  , ui_()
  , socket_(socket)
{
  ui_.setupUi(this);
  ui_.lvChat->setModel(&model_);

  connect(ui_.teMsg, &QTextEdit::textChanged,
          this, &MainWindow::msg_change);

  connect(ui_.bSend, &QPushButton::clicked,
          this, &MainWindow::send_click);
}

void MainWindow::set_login(const QString& login)
{
  connect(socket_, &QTcpSocket::readyRead,
          this, &MainWindow::read_response);

  login_ = login;
}

void MainWindow::msg_change()
{
  if (protocol::block::Message < static_cast<uint32_t>(ui_.teMsg->toPlainText().size()))
    ui_.teMsg->textCursor().deletePreviousChar();
}

void MainWindow::send_click()
{
  const auto msg = ui_.teMsg->toPlainText();

  if (msg.isEmpty())
    return;

  protocol::MsgRequest req;
  req.set_login(login_.toStdString());
  req.set_msg(msg.toStdString());

  socket_->write(Factory::serialize(req));

  append_msg(login_ + ": " + msg);

  ui_.teMsg->clear();
}

void MainWindow::read_response()
{
  const auto bytes = socket_->readAll();
  const auto header = Factory::get_header(bytes);

  if (header.command() != protocol::TypeCommand::MessageResponse) {
    QMessageBox::critical(this, "Critical", "Response isn't MessageResponse");
    return;
  }

  const auto data = Factory::deserialize<protocol::MsgResponse>(bytes);
  const auto msg = QString{data.login()} + ": " + data.msg();

  append_msg(msg);
}

void MainWindow::append_msg(const QString& msg)
{
  model_.insertRow(model_.rowCount());
  const auto index = model_.index(model_.rowCount() - 1, 0);
  model_.setData(index, msg);
}

