#include "protocol.h"

namespace protocol {

const std::string_view command_to_str(TypeCommand command)
{
  const auto it = type_command_str.find(command);
  return it != type_command_str.end() ? it->second : type_command_str.at(TypeCommand::Unknown);
}

} // namespace protocol

