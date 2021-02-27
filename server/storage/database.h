#pragma once

#include <QSqlDatabase>

#include "util/datetime.h"

namespace storage {

struct DatabaseConfig
{
  QString type;
  QString hostname;
  int port;
  QString db_name;
  QString username;
  QString password;
};

class Database
{
  QSqlDatabase db_;

public:
  explicit Database(const DatabaseConfig& config);

  bool open();
  void close();

  bool save_user(const QString& login, const QString& password);

  bool user_exist(const QString& login, const QString& password);

  void log_event(const util::DateTime& dt, const QString& event);
};

} // namespace storage
