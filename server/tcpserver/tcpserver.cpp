#include "tcpserver.h"
#include "factory/factory.h"
#include "storage/database.h"

#include <QTcpSocket>

namespace network {

using Factory = protocol::Factory<QByteArray>;

TcpServer::TcpServer(storage::Database* db, QObject* parent)
  : QObject(parent)
  , db_(db)
{ }

bool TcpServer::listen(quint16 port)
{
  const auto success = server_.listen(QHostAddress::Any, port);

  if (success) {
    db_->log_event(util::current_datetime(), "Server started");
    connect(&server_, &QTcpServer::newConnection,
            this, &TcpServer::new_connection);
  }

  return success;
}

void TcpServer::new_connection()
{
  auto client = server_.nextPendingConnection();
  client->setParent(this);

  connect(client, &QTcpSocket::disconnected,
          this, &TcpServer::client_disconnect);
  connect(client, &QTcpSocket::readyRead,
          this, &TcpServer::read_header);

  connections_.push_back(client);

  db_->log_event(util::current_datetime(), "New connection: " + client->peerAddress().toString());
}


void TcpServer::client_disconnect()
{
  auto client = qobject_cast<QTcpSocket*>(sender());
  connections_.remove(client);
  client->deleteLater();

  db_->log_event(util::current_datetime(), "Disconnect: " + client->peerAddress().toString());
}


void TcpServer::read_header()
{
  auto client = qobject_cast<QTcpSocket*>(sender());
  const auto bytes = client->readAll();

  const auto header = Factory::get_header(bytes);

  switch (header.command()) {
  case protocol::TypeCommand::RegistrationRequest: reg_request(client, bytes); break;
  case protocol::TypeCommand::AuthorisationRequest: auth_request(client, bytes); break;
  case protocol::TypeCommand::MessageRequest: msg_request(client, bytes); break;
  default:
    connections_.remove(client);
    client->deleteLater();
    db_->log_event(header.date(), "Request unknow command: "
                   + QString{protocol::command_to_str(header.command()).data()});
    break;
  }
}

void TcpServer::reg_request(QTcpSocket* sender, const QByteArray& bytes)
{
  const auto data = Factory::deserialize<protocol::RegRequest>(bytes);

  protocol::Status status = protocol::Status::Fail;

  if (db_->save_user(data.login(), data.password()))
    status = protocol::Status::Ok;

  protocol::RegResponse response;
  response.set_status(status);

  sender->write(Factory::serialize(response));

  const auto header = Factory::get_header(bytes);
  db_->log_event(header.date(),
                 QString{protocol::command_to_str(header.command()).data()}
                 + " from " + sender->peerAddress().toString());
}

void TcpServer::auth_request(QTcpSocket* sender, const QByteArray& bytes)
{
  const auto data = Factory::deserialize<protocol::AuthRequest>(bytes);

  protocol::Status status = protocol::Status::Fail;

  if (db_->user_exist(data.login(), data.password()))
    status = protocol::Status::Ok;

  protocol::AuthResponse response;
  response.set_status(status);

  sender->write(Factory::serialize(response));

  const auto header = Factory::get_header(bytes);
  db_->log_event(header.date(),
                 QString{protocol::command_to_str(header.command()).data()}
                 + " from " + sender->peerAddress().toString());
}

void TcpServer::msg_request(QTcpSocket* sender, const QByteArray& bytes)
{
  const auto data = Factory::deserialize<protocol::MsgRequest>(bytes);

  protocol::MsgResponse response;
  response.set_login(data.login());
  response.set_msg(data.msg());

  const auto res_data = Factory::serialize(response);

  for (auto client : connections_) {
    if (client != sender)
      client ->write(res_data);
  }

  const auto header = Factory::get_header(bytes);
  db_->log_event(header.date(),
                 QString{protocol::command_to_str(header.command()).data()} + " from "
                 + sender->peerAddress().toString());
}




} // namespace network
