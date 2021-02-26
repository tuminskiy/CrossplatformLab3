#include <QCoreApplication>

#include <iostream>

#include "definitions/protocol.h"
#include "tcpserver/tcpserver.h"
#include "storage/database.h"

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  storage::DatabaseConfig config;
  config.type = "QPSQL7";
  config.hostname = "127.0.0.1";
  config.port = 5432;
  config.db_name = "tumin";
  config.username = "tumin";
  config.password = "1234";

  storage::Database db(config);

  if (!db.open()) {
    std::cout << "Failed connect to database" << std::endl;
    return 0;
  }

  network::TcpServer server(&db);

  if (!server.listen(protocol::SERVER_DEFAULT_PORT)) {
    std::cout << "Failed start server on port: " << protocol::SERVER_DEFAULT_PORT << std::endl;
    return 0;
  }

  return a.exec();
}
