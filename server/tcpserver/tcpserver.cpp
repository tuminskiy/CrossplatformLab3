#include "tcpserver.h"
#include "factory/factory.h"
#include "storage/database.h"

#include <QTcpSocket>

#include <iostream>

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

  std::cout << "New connection: " << client->peerAddress().toString().toStdString() << "\n";

  connections_.push_back(client);
}


void TcpServer::client_disconnect()
{
  auto client = qobject_cast<QTcpSocket*>(sender());
  std::cout << "Disconnect: " << client->peerAddress().toString().toStdString() << "\n";
  connections_.remove(client);
  client->deleteLater();
}


void TcpServer::read_header()
{
  auto client = qobject_cast<QTcpSocket*>(sender());
  const auto bytes = client->readAll();

  const auto header = Factory::get_header(bytes);

  switch (header.command) {
  case protocol::TypeCommand::RegistrationRequest: reg_request(client, bytes); break;
  case protocol::TypeCommand::AuthorisationRequest: auth_request(client, bytes); break;
  case protocol::TypeCommand::MessageRequest: msg_request(client, bytes); break;
  default:
    connections_.remove(client);
    client->deleteLater();
    std::cout << "Unkow command: " << protocol::command_to_str(header.command) << '\n';
    break;
  }

  std::cout << "Bytes arrived: " << bytes.size() << '\n';
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
}




} // namespace network
