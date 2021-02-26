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

  connections_.push_back(client);

  connect(client, &QTcpSocket::readyRead,
          this, &TcpServer::read_header);
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
    client->deleteLater();
    connections_.remove(client);
    std::cerr << "Unkow command: " << protocol::command_to_str(header.command) << '\n';
    break;
  }

  std::clog << "Bytes arrived: " << bytes.size() << '\n';
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
