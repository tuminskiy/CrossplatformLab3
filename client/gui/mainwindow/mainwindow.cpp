#include "mainwindow.h"
#include "definitions/protocol.h"
#include "factory/factory.h"

#include <QTcpSocket>
#include <QMessageBox>
#include <QTextStream>
#include <QScrollBar>

using Factory = protocol::Factory<QByteArray>;

MainWindow::MainWindow(QTcpSocket* socket, QWidget *parent)
  : QMainWindow(parent)
  , ui_()
  , socket_(socket)
  , scrolled_to_end_(true)
{
  ui_.setupUi(this);

  connect(ui_.teMsg, &QTextEdit::textChanged,
          this, &MainWindow::msg_change);

  connect(ui_.bSend, &QPushButton::clicked,
          this, &MainWindow::send_click);

  connect(ui_.tbChat->verticalScrollBar(), &QScrollBar::valueChanged,
          this, &MainWindow::chat_scroll);
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
  const auto msg = ui_.teMsg->toPlainText().trimmed();

  if (msg.isEmpty())
    return;

  protocol::MsgRequest req;
  req.set_login(login_.toStdString());
  req.set_msg(msg.toStdString());

  socket_->write(Factory::serialize(req));

  append_msg(login_, msg);

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

  append_msg(data.login(), data.msg());
}

void MainWindow::chat_scroll(int value)
{
  scrolled_to_end_ = (value == ui_.tbChat->verticalScrollBar()->maximum());
}

void MainWindow::append_msg(const QString& login, const QString& msg)
{
  auto cursor = ui_.tbChat->textCursor();
  cursor.movePosition(QTextCursor::End);
  cursor.insertHtml("<br/><b>" + login + ":</b> ");
  cursor.insertText(msg);

  if (scrolled_to_end_) {
    auto scroll_bar = ui_.tbChat->verticalScrollBar();
    scroll_bar->setValue(scroll_bar->maximum());
  }
}

