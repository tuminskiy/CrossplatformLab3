#pragma once

#include "definitions/protocol.h"

#include <string_view>
#include <cstring>

namespace protocol {

class Header
{
  TypeCommand command_;
  util::DateTime date_;

public:
  TypeCommand command() const { return command_; }
  void set_command(TypeCommand command) { command_ = command; }

  util::DateTime date() const { return date_; }
  void set_date(const util::DateTime& date) { date_ = date; }
};

class RegRequest
{
  char login_[block::Login];
  char password_[block::Password];

public:
  RegRequest()
  {
    clear_login();
    std::memset(password_, 0, block::Password);
  }

  const char* login() const { return login_; }
  void set_login(std::string_view sv)
  {
    clear_login();
    const auto count = sv.size() < block::Login ? sv.size() : block::Login;
    sv.copy(login_, count);
  }

  const char* password() const { return password_; }
  void set_password(std::string_view sv)
  {
    clear_password();
    const auto count = sv.size() < block::Password ? sv.size() : block::Password;
    sv.copy(password_, count);
  }

private:
  void clear_login() { std::memset(login_, 0, block::Login); }
  void clear_password() { std::memset(password_, 0, block::Password); }
};

class AuthRequest
{
  char login_[block::Login];
  char password_[block::Password];

public:
  AuthRequest()
  {
    clear_login();
    clear_password();
  }

  const char* login() const { return login_; }
  void set_login(std::string_view sv)
  {
    clear_login();
    const auto count = sv.size() < block::Login ? sv.size() : block::Login;
    sv.copy(login_, count);
  }

  const char* password() const { return password_; }
  void set_password(std::string_view sv)
  {
    clear_password();
    const auto count = sv.size() < block::Password ? sv.size() : block::Password;
    sv.copy(password_, count);
  }

private:
  void clear_login() { std::memset(login_, 0, block::Login); }
  void clear_password() { std::memset(password_, 0, block::Password); }
};

class MsgRequest
{
  char login_[block::Login];
  char msg_[block::Message];

public:
  MsgRequest()
  {
    clear_login();
    clear_msg();
  }

  const char* login() const { return login_; }
  void set_login(std::string_view sv)
  {
    clear_login();
    const auto count = sv.size() < block::Login ? sv.size() : block::Login;
    sv.copy(login_, count);
  }

  const char* msg() const { return msg_; }
  void set_msg(std::string_view sv)
  {
    clear_msg();
    const auto count = sv.size() < block::Message ? sv.size() : block::Message;
    sv.copy(login_, count);
  }

private:
  void clear_login() { std::memset(login_, 0, block::Login); }
  void clear_msg() { std::memset(msg_, 0, block::Message); }
};


enum class Status { Fail = 0, Ok };

class RegResponse
{
  Status status_;

public:
  Status status() const { return status_; }
  void set_status(Status status) { status_ = status; }
};

class AuthResponse
{
  Status status_;

public:
  Status status() const { return status_; }
  void set_status(Status status) { status_ = status; }
};

class MsgResponse
{
  char login_[block::Login];
  char msg_[block::Message];

public:
  MsgResponse()
  {
    clear_login();
    clear_msg();
  }

  const char* login() const { return login_; }
  void set_login(std::string_view sv)
  {
    clear_login();
    const auto count = sv.size() < block::Login ? sv.size() : block::Login;
    sv.copy(login_, count);
  }

  const char* msg() const { return msg_; }
  void set_msg(std::string_view sv)
  {
    clear_msg();
    const auto count = sv.size() < block::Message ? sv.size() : block::Message;
    sv.copy(login_, count);
  }

private:
  void clear_login() { std::memset(login_, 0, block::Login); }
  void clear_msg() { std::memset(msg_, 0, block::Message); }
};

} // namespace protocol
