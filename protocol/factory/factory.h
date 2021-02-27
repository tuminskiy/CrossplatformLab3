#pragma once

#include "definitions/protocol.h"
#include "definitions/typecommand_traits.h"
#include "definitions/messages.h"

#include <utility> // std::forward
#include <type_traits>
#include <cstring>
#include <functional>

namespace protocol {

template <class ByteArray>
class Factory
{
public:
  template <class T>
  static auto serialize(const T& value)
  {
    ByteArray buff(block::Header + sizeof(T), '\0');

    const auto command = traits::command_v<T>;

    if (command == TypeCommand::Unknown) {
      throw std::logic_error("Serialize: unknow message, TypeCommand is Unknow");
    }

    const auto dt = util::current_datetime();

    std::memcpy(buff.data(), &command, block::Command);
    std::memcpy(buff.data() + block::Command, &dt, block::DateTime);
    std::memcpy(buff.data() + block::Header, &value, sizeof(T));

    return buff;
  }

  static Header get_header(const ByteArray& bytes)
  {
    TypeCommand command;
    util::DateTime date;

    std::memcpy(&command, bytes.data(), block::Command);
    std::memcpy(&date, bytes.data() + block::Command, block::DateTime);

    Header header;
    header.set_command(command);
    header.set_date(date);

    return header;
  }

  template <class T>
  static T deserialize(const ByteArray& bytes)
  {
    T result;
    std::memcpy(&result, bytes.data() + block::Header, sizeof(T));
    return result;
  }
};


} // namespace protocol
