#pragma once

#include <unordered_map>
#include <string_view>
#include <cstdint>

#include "util/datetime.h"

namespace protocol {

using id_t = int32_t;
constexpr unsigned short SERVER_DEFAULT_PORT = 27015;

enum class TypeCommand : uint16_t {
  Unknown = 0,
  RegistrationRequest,
  RegistrationResponse,
  AuthorisationRequest,
  AuthorisationResponse,
  MessageRequest,
  MessageResponse
};

static const std::unordered_map<TypeCommand, std::string_view> type_command_str = {
  {TypeCommand::Unknown,               "Unknow"},
  {TypeCommand::RegistrationRequest,   "RegistrationRequest"},
  {TypeCommand::RegistrationResponse,  "RegistrationResponse"},
  {TypeCommand::AuthorisationRequest,  "AuthorisationRequest"},
  {TypeCommand::AuthorisationResponse, "AuthorisationResponse"},
  {TypeCommand::MessageRequest,        "MessageRequest"},
  {TypeCommand::MessageResponse,       "MessageResponse"}
};

inline const std::string_view command_to_str(TypeCommand command)
{
  const auto it = type_command_str.find(command);
  return it != type_command_str.end() ? it->second : type_command_str.at(TypeCommand::Unknown);
}

} // namespace protocol

namespace protocol::block {

constexpr uint32_t Command = sizeof(TypeCommand);
constexpr uint32_t DateTime = sizeof(util::DateTime);

constexpr uint32_t Login = 16;
constexpr uint32_t Password = 32;
constexpr uint32_t Message = 256;

constexpr uint32_t Header = Command + DateTime;

} // namespace protocol::block
