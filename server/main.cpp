#include <QCoreApplication>
#include <QSettings>

#include <iostream>

#include "definitions/protocol.h"
#include "tcpserver/tcpserver.h"
#include "storage/database.h"

bool is_valid_settings(QSettings& settings);

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  if (argc != 2) {
    std::cout << "Database configuration file was not transferred\n";
    return 0;
  }

  QSettings settings(argv[1], QSettings::IniFormat);

  if (!is_valid_settings(settings)) {
    std::cout << "Incorrect config file\n";
    return 0;
  }

  storage::DatabaseConfig config;
  settings.beginGroup("DatabaseConfig");
  config.type = settings.value("type").toString();
  config.hostname = settings.value("hostname").toString();
  config.port = settings.value("port").toInt();
  config.db_name = settings.value("db_name").toString();
  config.username = settings.value("username").toString();
  config.password = settings.value("password").toString();
  settings.endGroup();

  storage::Database db(config);

  if (!db.open()) {
    std::cout << "Failed connect to database\n";
    return 0;
  }

  network::TcpServer server(&db);

  if (!server.listen(protocol::SERVER_DEFAULT_PORT)) {
    std::cout << "Failed start server on port: " << protocol::SERVER_DEFAULT_PORT << '\n';
    return 0;
  }

  return a.exec();
}

bool is_valid_settings(QSettings& settings)
{
  bool result = true;

  settings.beginGroup("DatabaseConfig");
  result &= settings.contains("type");
  result &= settings.contains("hostname");
  result &= settings.contains("port");
  result &= settings.contains("db_name");
  result &= settings.contains("username");
  result &= settings.contains("password");
  settings.endGroup();

  return result;
}
