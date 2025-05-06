#pragma once

#include "NetCommon.hpp"
#include "GameMsgTypes.hpp"
#include "PlayerStates.hpp"

#include <chrono>
#include <cstdint>

namespace EngineG::Network{

template<typename T>
class Connection;

template<typename T>
struct MessageHeader{
    T id{};
    uint32_t size = 0;
};

template<typename T>
struct Message{
    MessageHeader<T> header{};
    std::vector<uint8_t> body;

    [[nodiscard]] size_t size() const {
      return body.size();
    }

    friend std::ostream& operator<<(std::ostream& os, const Message<T>& msg) {
      os << "ID:" << int(msg.header.id) << " Size:" << msg.header.size();
      return os;
    }

    template<typename DataType>
    friend Message<T>& operator<<(Message<T>& msg, DataType& data) {
      static_assert(std::is_standard_layout<DataType>::value, "DataType must be standard layout! Its too complex to be pushed!");
      size_t i = msg.body.size();
      msg.body.resize(msg.body.size() + sizeof(DataType));
      std::memcpy(msg.body.data() + i, &data, sizeof(DataType));
      msg.header.size = msg.size();
      return msg;
    }

    template<typename DataType>
    friend Message<T>& operator>>(Message<T>& msg, DataType& data) {
      static_assert(std::is_standard_layout<DataType>::value, "DataType must be standard layout! Its too complex to be pushed!");
      size_t i = msg.body.size() - sizeof(DataType);
      std::memcpy(&data, msg.body.data() + i, sizeof(DataType));
      msg.body.resize(i);
      msg.header.size = msg.size();
      return msg;
    }
};

template<typename T>
struct OwnedMessage
{
    std::shared_ptr<Connection<T>> remote = nullptr;
    Message<T> message;

    friend std::ostream& operator<<(std::ostream& os, const OwnedMessage<T>& msg)
    {
        os << msg.message;
        return os;
    }
};

template<typename... Args>
inline Message<GameMsgTypes> CreateMessage(GameMsgTypes type, Args&&... args) {
    Message<GameMsgTypes> msg;
    msg.header.id = type;
    (msg << ... << args);
    return msg;
}

// ---------- ServerAccept ----------
struct ServerAccept {
    uint32_t clientID;
};

inline Message<GameMsgTypes> CreateMsgServerAccept(uint32_t clientID) {
    return CreateMessage(GameMsgTypes::ServerAccept, clientID);
}

inline ServerAccept ParseMsgServerAccept(Message<GameMsgTypes>& msg) {
    ServerAccept payload;
    msg >> payload.clientID;
    return payload;
}

// ---------- ServerPing ----------
struct ServerPing {
    int64_t timestamp;
};

inline Message<GameMsgTypes> CreateMsgServerPing(int64_t timestamp) {
    return CreateMessage(GameMsgTypes::ServerPing, timestamp);
}

inline ServerPing ParseMsgServerPing(Message<GameMsgTypes>& msg) {
    ServerPing payload;
    msg >> payload.timestamp;
    return payload;
}

// ---------- GamePlayerDisconnect ----------
struct PlayerDisconnect {
    uint32_t clientID;
};

inline Message<GameMsgTypes> CreateMsgGamePlayerDisconnect(uint32_t clientID) {
    return CreateMessage(GameMsgTypes::GamePlayerDisconnect, clientID);
}

inline PlayerDisconnect ParseMsgGamePlayerDisconnect(Message<GameMsgTypes>& msg) {
    PlayerDisconnect payload;
    msg >> payload.clientID;
    return payload;
}

// ---------- ClientPing ----------
inline Message<GameMsgTypes> CreateMsgClientPing() {
    int64_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
    return CreateMessage(GameMsgTypes::ClientPing, timestamp);
}

inline std::chrono::system_clock::time_point ParseMsgClientPing(Message<GameMsgTypes>& msg) {
    int64_t timestamp;
    msg >> timestamp;
    return std::chrono::system_clock::time_point(std::chrono::milliseconds(timestamp));
}

// ---------- ClientInputUpdate ----------
inline Message<GameMsgTypes> CreateMsgClientInputUpdate(const PlayerInput& inputState) {
    return CreateMessage(GameMsgTypes::ClientInputUpdate, inputState);
}

inline PlayerInput ParseMsgClientInputUpdate(Message<GameMsgTypes>& msg) {
    PlayerInput input;
    msg >> input;
    return input;
}

}