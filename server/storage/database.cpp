#include "database.h"

#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QDebug>

namespace storage {

Database::Database(const DatabaseConfig& config)
  : db_(QSqlDatabase::addDatabase(config.type))
{
  db_.setHostName(config.hostname);
  db_.setPort(config.port);
  db_.setDatabaseName(config.db_name);
  db_.setUserName(config.username);
  db_.setPassword(config.password);
}

bool Database::open() { return db_.open(); }

void Database::close() { db_.close(); }

bool Database::save_user(const QString& login, const QString& password)
{
  QSqlQuery query(db_);
  query.prepare("INSERT INTO users VALUES (:login, :password);");
  query.bindValue(":login", login);
  query.bindValue(":password", password);

  return query.exec();
}

bool Database::user_exist(const QString& login, const QString& password)
{
  QSqlQuery query(db_);
  query.prepare("SELECT login FROM users WHERE login=:login AND password=:password");
  query.bindValue(":login", login);
  query.bindValue(":password", password);

  return query.exec() && query.next();
}



} // namespace storage
