#pragma once

#include "NetCommon.hpp" // Includes ASIO, standard types
#include "NetTSQueue.hpp"
#include "NetMessage.hpp"

namespace EngineG::Network {

template<typename T>
class Connection : public std::enable_shared_from_this<Connection<T>> {
public:
    enum class Owner {
        Server,
        Client
    };

    Connection(
        Owner parent,
        asio::io_context& ioContext,
        asio::ip::tcp::socket socket,
        TSQueue<OwnedMessage<T>>& qIn
    );

    virtual ~Connection();

    [[nodiscard]] uint32_t GetID() const;

public:
    void ConnectToClient(uint32_t uid = 0);
    void ConnectToServer(const asio::ip::tcp::resolver::results_type& endpoints);
    void Disconnect();
    [[nodiscard]] bool IsConnected() const;
    void Send(const Message<T>& msg);

private:
    void ReadHeader();
    void ReadBody();
    void WriteHeader();
    void WriteBody();
    void AddToIncomingMessageQueue();

protected:
    asio::ip::tcp::socket mSocket;
    asio::io_context& mIoContext;
    TSQueue<Message<T>> mQMessagesOut;
    TSQueue<OwnedMessage<T>>& mQMessagesIn;
    Message<T> mMsgTemporaryIn;
    Owner mOwnerType = Owner::Server;
    uint32_t mId = 0;
    static constexpr size_t MAX_MESSAGE_BODY_SIZE = 65536; // 64KB
};

template<typename T>
Connection<T>::Connection(
    Owner parent,
    asio::io_context& ioContext,
    asio::ip::tcp::socket socket,
    TSQueue<OwnedMessage<T>>& qIn
) : mSocket(std::move(socket)),
    mIoContext(ioContext),
    mQMessagesIn(qIn),
    mOwnerType(parent)
{}

template<typename T>
Connection<T>::~Connection()
{}

template<typename T>
uint32_t Connection<T>::GetID() const {
    return mId;
}

template<typename T>
void Connection<T>::ConnectToClient(uint32_t uid) {
    if (mOwnerType == Owner::Server) {
        if (mSocket.is_open()) {
            mId = uid;
            ReadHeader();
        }
    }
}

template<typename T>
void Connection<T>::ConnectToServer(const asio::ip::tcp::resolver::results_type& endpoints) {
    if (mOwnerType == Owner::Client) {
        asio::async_connect(mSocket, endpoints,
            [this](std::error_code ec, const asio::ip::tcp::endpoint& /*endpoint*/) {
                if (!ec) {
                    ReadHeader();
                } else {
                    std::cerr << "[" << mId << "] Connect To Server Error: " << ec.message() << std::endl;
                    mSocket.close();
                }
            }
        );
    }
}

template<typename T>
void Connection<T>::Disconnect() {
    if (IsConnected()) {
        asio::post(mIoContext, [this]() {
             std::error_code ec;
             mSocket.shutdown(asio::ip::tcp::socket::shutdown_both, ec);
             mSocket.close(ec);
             if (ec) { std::cerr << "[" << mId << "] Socket close error: " << ec.message() << std::endl; }
        });
    }
}

template<typename T>
bool Connection<T>::IsConnected() const {
    return mSocket.is_open();
}

template<typename T>
void Connection<T>::Send(const Message<T>& msg) {
    asio::post(mIoContext, [this, msg]() {
        bool bWritingMessage = !mQMessagesOut.empty();
        mQMessagesOut.push_back(msg);
        if (!bWritingMessage && IsConnected()) {
            WriteHeader();
        }
    });
}

template<typename T>
void Connection<T>::ReadHeader() {
    auto self = this->shared_from_this();
    asio::async_read(mSocket, asio::buffer(&mMsgTemporaryIn.header, sizeof(MessageHeader<T>)),
        [this, self](std::error_code ec, std::size_t length) {
            if (!ec) {
                if (mMsgTemporaryIn.header.size > MAX_MESSAGE_BODY_SIZE) {
                     std::cerr << "[" << mId << "] Read Header Error: Message size exceeds limit (" << mMsgTemporaryIn.header.size << "). Disconnecting." << std::endl;
                     mSocket.close();
                     return;
                }

                if (mMsgTemporaryIn.header.size > 0) {
                    mMsgTemporaryIn.body.resize(mMsgTemporaryIn.header.size);
                    ReadBody();
                } else {
                    AddToIncomingMessageQueue();
                }
            } else {
                std::cout << "[" << mId << "] Read Header Fail: " << ec.message() << ". Connection closing." << std::endl;
                mSocket.close();
            }
        }
    );
}

template<typename T>
void Connection<T>::ReadBody() {
    auto self = this->shared_from_this();
    asio::async_read(mSocket, asio::buffer(mMsgTemporaryIn.body.data(), mMsgTemporaryIn.body.size()),
        [this, self](std::error_code ec, std::size_t length) {
            if (!ec) {
                AddToIncomingMessageQueue();
            } else {
                std::cout << "[" << mId << "] Read Body Fail: " << ec.message() << ". Connection closing." << std::endl;
                mSocket.close();
            }
        }
    );
}

template<typename T>
void Connection<T>::WriteHeader() {
    if (mQMessagesOut.empty()) return;
    auto self = this->shared_from_this();
    asio::async_write(mSocket, asio::buffer(&mQMessagesOut.front().header, sizeof(MessageHeader<T>)),
        [this, self](std::error_code ec, std::size_t length) {
            if (!ec) {
                if (mQMessagesOut.front().body.size() > 0) {
                    WriteBody();
                } else {
                    mQMessagesOut.pop_front();
                    if (!mQMessagesOut.empty()) {
                        WriteHeader();
                    }
                }
            } else {
                std::cout << "[" << mId << "] Write Header Fail: " << ec.message() << ". Connection closing." << std::endl;
                mSocket.close();
            }
        }
    );
}

template<typename T>
void Connection<T>::WriteBody() {
     if (mQMessagesOut.empty()) return;
     auto self = this->shared_from_this();
     asio::async_write(mSocket, asio::buffer(mQMessagesOut.front().body.data(), mQMessagesOut.front().body.size()),
        [this, self](std::error_code ec, std::size_t length) {
            if (!ec) {
                mQMessagesOut.pop_front();
                if (!mQMessagesOut.empty()) {
                    WriteHeader();
                }
            } else {
                std::cout << "[" << mId << "] Write Body Fail: " << ec.message() << ". Connection closing." << std::endl;
                mSocket.close();
            }
        }
    );
}

template<typename T>
void Connection<T>::AddToIncomingMessageQueue() {
    OwnedMessage<T> ownedMsg;
    if (mOwnerType == Owner::Server) {
        ownedMsg.remote = this->shared_from_this();
    } else { // Owner::Client
        ownedMsg.remote = nullptr;
    }
    ownedMsg.message = std::move(mMsgTemporaryIn);

    mQMessagesIn.push_back(ownedMsg);

    ReadHeader();
}


} // namespace EngineG::Network