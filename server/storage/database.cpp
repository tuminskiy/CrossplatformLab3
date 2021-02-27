#include "database.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QChar>

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
  query.prepare("SELECT login FROM users WHERE login=:login AND password=:password;");
  query.bindValue(":login", login);
  query.bindValue(":password", password);

  return query.exec() && query.next();
}

void Database::log_event(const util::DateTime& dt, const QString& event)
{
  auto dt_str = QString{"%1-%2-%3 %4:%5:%6"}
      .arg(dt.date.year, 4, 10, QChar{'0'})
      .arg(dt.date.month, 2, 10, QChar{'0'})
      .arg(dt.date.day, 2, 10, QChar{'0'})
      .arg(dt.time.hours, 2, 10, QChar{'0'})
      .arg(dt.time.minutes, 2, 10, QChar{'0'})
      .arg(dt.time.seconds, 2, 10, QChar{'0'});

  QSqlQuery query(db_);
  query.prepare("INSERT INTO logs VALUES (:date, :event);");
  query.bindValue(":date", dt_str);
  query.bindValue(":event", event);

  query.exec();
}



} // namespace storage
