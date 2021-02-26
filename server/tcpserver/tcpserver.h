#pragma once

#include <QTcpServer>

#include <list>

class QTcpSocket;

namespace storage { class Database; }

namespace network {

class TcpServer : public QObject
{
  Q_OBJECT

  storage::Database* db_;
  QTcpServer server_;
  std::list<QTcpSocket*> connections_;

public:
  explicit TcpServer(storage::Database* db, QObject* parent = nullptr);

  bool listen(quint16 port);

private slots:
  void new_connection();
  void client_disconnect();
  void read_header();

  void reg_request(QTcpSocket* sender, const QByteArray& bytes);
  void auth_request(QTcpSocket* sender, const QByteArray& bytes);
  void msg_request(QTcpSocket* sender, const QByteArray& bytes);
};

} // namespace network
