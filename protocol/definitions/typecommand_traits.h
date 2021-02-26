#pragma once

#include "protocol.h"
#include "messages.h"

namespace traits {

template <class T>
struct TypeCommandTraits
{
  static constexpr auto Command = protocol::TypeCommand::Unknown;
};

template <class T>
constexpr auto command_v = TypeCommandTraits<T>::Command;


template <>
struct TypeCommandTraits<protocol::RegRequest>
{
  static constexpr auto Command = protocol::TypeCommand::RegistrationRequest;
};

template <>
struct TypeCommandTraits<protocol::RegResponse>
{
  static constexpr auto Command = protocol::TypeCommand::RegistrationResponse;
};



template <>
struct TypeCommandTraits<protocol::AuthRequest>
{
  static constexpr auto Command = protocol::TypeCommand::AuthorisationRequest;
};

template <>
struct TypeCommandTraits<protocol::AuthResponse>
{
  static constexpr auto Command = protocol::TypeCommand::AuthorisationResponse;
};



template <>
struct TypeCommandTraits<protocol::MsgRequest>
{
  static constexpr auto Command = protocol::TypeCommand::MessageRequest;
};

template <>
struct TypeCommandTraits<protocol::MsgResponse>
{
  static constexpr auto Command = protocol::TypeCommand::MessageResponse;
};

} // namespace traits
