#pragma once

#include "NetCommon.hpp"
#include "NetTSQueue.hpp"
#include "NetMessage.hpp"

#include <functional>

namespace EngineG::Network {

template<typename T>
class Connection : public std::enable_shared_from_this<Connection<T>>
{
public:
    enum class Owner
    {
        Server,
        Client
    };

    Connection(
        Owner parent,
        asio::io_context& ioContext,
        asio::ip::tcp::socket socket,
        TSQueue<OwnedMessage<T>>& qIn
    ):mSocket(std::move(socket)),
    mIoContext(ioContext),
    mQMessagesIn(qIn),
    mOwnerType(parent)
    {
    }

    virtual ~Connection()
    {}

    uint32_t GetID() const
    {
        return mId;
    }

public:
    void ConnectToClient(uint32_t uid = 0)
    {
        if (mOwnerType == Owner::Server)
        {
            if (mSocket.is_open())
            {
                mId = uid;
                ReadHeader();
            }
        }
    }

    void ConnectToServer(const asio::ip::tcp::resolver::results_type& endpoints)
    {
        // Only for clients
        if (mOwnerType == Owner::Client)
        {
            asio::async_connect(mSocket, endpoints,
                [this](std::error_code ec, asio::ip::tcp::endpoint endpoint)
                {
                    if (!ec)
                    {
                        ReadHeader();
                    }
                    else
                    {
                        std::cerr << "Error connecting to socket as Client" << std::endl;
                        mSocket.close();
                    }
                }
            );
        }
    }

    void Disconnect()
    {
        if (IsConnected())
        {
            asio::post(mIoContext, [this]()
            {
                mSocket.close();
            });
        }
    }

    bool IsConnected() const
    {
        return mSocket.is_open();
    }

public:
    void Send(const Message<T>& msg)
    {
        asio::post(mIoContext, [this, msg]()
        {
            bool bWritingMessage = !mQMessagesOut.empty();
            mQMessagesOut.push_back(msg);
            if (!bWritingMessage)
            {
                WriteHeader();
            }
        });
    }

private:
    // Async - Prime context ready to read a message header
    void ReadHeader()
    {
        asio::async_read(mSocket, asio::buffer(&mMsgTemporaryIn.header, sizeof(MessageHeader<T>)),
            [this](std::error_code ec, std::size_t length)
            {
                if (!ec)
                {
                    if (mMsgTemporaryIn.header.size > 0)
                    {
                        mMsgTemporaryIn.body.resize(mMsgTemporaryIn.header.size);
                        ReadBody();
                    }
                    else
                    {
                        AddToIncomingMessageQueue();
                    }
                }
                else
                {
                    std::cout << "[" << mId << "] Read Header Fail." << std::endl;
                    mSocket.close();
                }
            }
        );
    }

    // Async - Prime context ready to read a message body
    void ReadBody()
    {
        asio::async_read(mSocket, asio::buffer(mMsgTemporaryIn.body.data(), mMsgTemporaryIn.body.size()),
            [this](std::error_code ec, std::size_t length)
            {
                if (!ec)
                {
                    AddToIncomingMessageQueue();
                }
                else
                {
                    std::cout << "[" << mId << "] ReadBody Fail." << std::endl;
                    mSocket.close();
                }
            }
        );
    }

    void WriteHeader()
    {
        asio::async_write(mSocket, asio::buffer(&mQMessagesOut.front().header, sizeof(MessageHeader<T>)),
            [this](std::error_code ec, std::size_t length)
            {
                if (!ec)
                {
                    if (mQMessagesOut.front().body.size() > 0)
                    {
                        WriteBody();
                    }
                    else
                    {
                        mQMessagesOut.pop_front();

                        if (!mQMessagesOut.empty())
                        {
                            WriteHeader();
                        }
                    }
                }
                else
                {
                    std::cout << "[" << mId << "] WriteHeader Fail." << std::endl;
                    mSocket.close();
                }
            }
        );
    }

    // Async - Prime context to write a message body
    void WriteBody()
    {
        asio::async_write(mSocket, asio::buffer(mQMessagesOut.front().body.data(), mQMessagesOut.front().body.size()),
        [this](std::error_code ec, std::size_t length)
            {
                if (!ec)
                {
                    mQMessagesOut.pop_front();

                    if (!mQMessagesOut.empty())
                    {
                        WriteHeader();
                    }
                }
                else
                {
                    std::cout << "[" << mId << "] WriteBody Fail." << std::endl;
                    mSocket.close();
                }
            }
        );
    }

    void AddToIncomingMessageQueue()
    {
        if (mOwnerType == Owner::Server)
        {
            mQMessagesIn.push_back({ this -> shared_from_this(), mMsgTemporaryIn });
        }
        else if (mOwnerType == Owner::Client)
        {
            mQMessagesIn.push_back({ nullptr, mMsgTemporaryIn });
        }
        else
        {
            mSocket.close();
            throw std::runtime_error("Wrong owner type");
        }

        ReadHeader();
    }

protected:
    asio::ip::tcp::socket mSocket;
    asio::io_context& mIoContext;
    // This queue holds all messages to be sent to the remote side of this connection
    TSQueue<Message<T>> mQMessagesOut;

    // This queue holds all messages that have been recieved from the remote side of this connection
    // Note it is a reference as the "owner" of this connection is expected to provide a queue
    TSQueue<OwnedMessage<T>>& mQMessagesIn;
    Message<T> mMsgTemporaryIn;

    // the "owner" decides how some of the connection behaves
    Owner mOwnerType = Owner::Server;
    uint32_t mId = 0;
};

}