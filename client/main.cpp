#include "gui/mainwindow/mainwindow.h"
#include "gui/loginwindow/loginwindow.h"
#include "gui/registrationwindow/registrationwindow.h"

#include "definitions/protocol.h"

#include <QApplication>
#include <QMessageBox>
#include <QTcpSocket>
#include <QHostAddress>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  QTcpSocket socket;

  socket.connectToHost("localhost", protocol::SERVER_DEFAULT_PORT);

  if (!socket.waitForConnected(5000)) {
    QMessageBox::critical(nullptr, "Critical", "Failed connect to server: 127.0.0.1:"
                          + QString::number(protocol::SERVER_DEFAULT_PORT));
    return 0;
  }

  LoginWindow lw(&socket);
  RegistrationWindow rw(&socket);
  MainWindow mw(&socket);

  QObject::connect(&lw, &LoginWindow::authorized,
    [&mw](const auto& login) {
      mw.set_login(login);
      mw.show();
    }
  );

  QObject::connect(&lw, &LoginWindow::reg_clicked,
                   &rw, &RegistrationWindow::show);

  lw.show();

  return a.exec();
}

