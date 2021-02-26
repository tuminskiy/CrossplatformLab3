#pragma once

#include "definitions/protocol.h"

#include <string_view>
#include <cstring>

namespace protocol {

struct Header
{
  TypeCommand command;
  util::DateTime dt;
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
    const auto count = std::min(static_cast<uint32_t>(sv.size()), block::Login);
    sv.copy(login_, count);
  }

  const char* password() const { return password_; }
  void set_password(std::string_view sv)
  {
    clear_password();
    const auto count = std::min(static_cast<uint32_t>(sv.size()), block::Password);
    sv.copy(password_, block::Password);
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
    const auto count = std::min(static_cast<uint32_t>(sv.size()), block::Login);
    sv.copy(login_, count);
  }

  const char* password() const { return password_; }
  void set_password(std::string_view sv)
  {
    clear_password();
    const auto count = std::min(static_cast<uint32_t>(sv.size()), block::Password);
    sv.copy(password_, block::Password);
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
    const auto count = std::min(static_cast<uint32_t>(sv.size()), block::Login);
    sv.copy(login_, count);
  }

  const char* msg() const { return msg_; }
  void set_msg(std::string_view sv)
  {
    clear_msg();
    const auto count = std::min(static_cast<uint32_t>(sv.size()), block::Message);
    sv.copy(msg_, block::Message);
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
  Status status() { return status_; }
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
  util::DateTime dt_;
  char msg_[block::Message];

public:
  MsgResponse()
  {
    clear_login();
    clear_msg();
  }

  void set_login(std::string_view sv)
  {
    clear_login();
    const auto count = std::min(static_cast<uint32_t>(sv.size()), block::Login);
    sv.copy(login_, count);
  }

  util::DateTime dt() const { return dt_; }
  void set_dt(const util::DateTime& dt) { dt_ = dt; }

  const char* msg() const { return msg_; }
  void set_msg(std::string_view sv)
  {
    clear_msg();
    const auto count = std::min(static_cast<uint32_t>(sv.size()), block::Message);
    sv.copy(msg_, block::Message);
  }

private:
  void clear_login() { std::memset(login_, 0, block::Login); }
  void clear_msg() { std::memset(msg_, 0, block::Message); }
};

} // namespace protocol
