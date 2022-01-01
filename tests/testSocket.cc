/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2022 Julien Bernard
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */
#include <gf/SocketSelector.h>
#include <gf/TcpListener.h>
#include <gf/TcpSocket.h>
#include <gf/UdpSocket.h>

#include <cstdint>
#include <thread>
#include <vector>

#include "gtest/gtest.h"

#include <gf/SerializationOps.h>

namespace {
  constexpr const char *TestService = "12345";
  constexpr const char *Host = "localhost";

  template<gf::SocketFamily Family>
  void testTcpListenerService() {
    gf::TcpListener listener(TestService, Family);

    ASSERT_TRUE(listener);

    auto address = listener.getLocalAddress();
    std::cout << "host: " << address.getHostname(gf::SocketAddressFormat::Numeric) << '\n';
    std::cout << "service: " << address.getService(gf::SocketAddressFormat::Numeric) << '\n';
  }

  template<gf::SocketFamily Family>
  void testTcpListenerOneClient() {
    gf::TcpListener listener(TestService, Family);
    ASSERT_TRUE(listener);

    std::thread clientThread([]() {
      gf::TcpSocket socket(Host, TestService, Family);
      ASSERT_TRUE(socket);

      uint8_t buffer[4] = { 0x42, 0x69, 0xFF, 0x12 };
      auto res = socket.sendRawBytes(buffer);
      EXPECT_EQ(res.status, gf::SocketStatus::Data);
      EXPECT_EQ(res.length, 4u);

      auto local = socket.getLocalAddress();
      std::cout << "local host: " << local.getHostname(gf::SocketAddressFormat::Numeric) << '\n';
      std::cout << "local service: " << local.getService(gf::SocketAddressFormat::Numeric) << '\n';

      auto remote = socket.getRemoteAddress();
      std::cout << "remote host: " << remote.getHostname(gf::SocketAddressFormat::Numeric) << '\n';
      std::cout << "remote service: " << remote.getService(gf::SocketAddressFormat::Numeric) << '\n';
    });

    gf::TcpSocket socket = listener.accept();
    ASSERT_TRUE(socket);

    uint8_t buffer[10];
    auto res = socket.recvRawBytes(buffer);
    EXPECT_EQ(res.status, gf::SocketStatus::Data);
    EXPECT_EQ(res.length, 4u);
    EXPECT_EQ(buffer[0], 0x42);
    EXPECT_EQ(buffer[1], 0x69);
    EXPECT_EQ(buffer[2], 0xFF);
    EXPECT_EQ(buffer[3], 0x12);

    res = socket.recvRawBytes(buffer);
    EXPECT_EQ(res.status, gf::SocketStatus::Close);

    clientThread.join();
  }

  template<gf::SocketFamily Family>
  void testTcpListenerMultipleClient() {
    static constexpr int ClientCount = 10;

    gf::TcpListener listener(TestService, Family);
    ASSERT_TRUE(listener);

    std::thread clientThread([]() {
      for (int i = 0; i < ClientCount; ++i) {
        gf::TcpSocket socket(Host, TestService, Family);
        ASSERT_TRUE(socket);

        uint8_t buffer[4] = { 0x42, 0x69, 0xFF, 0x12 };
        auto res = socket.sendRawBytes(buffer);
        EXPECT_EQ(res.status, gf::SocketStatus::Data);
        EXPECT_EQ(res.length, 4u);
      }
    });

    for (int i = 0; i < ClientCount; ++i) {
      gf::TcpSocket socket = listener.accept();
      ASSERT_TRUE(socket);

      uint8_t buffer[10];
      auto res = socket.recvRawBytes(buffer);
      EXPECT_EQ(res.status, gf::SocketStatus::Data);
      EXPECT_EQ(res.length, 4u);
      EXPECT_EQ(buffer[0], 0x42);
      EXPECT_EQ(buffer[1], 0x69);
      EXPECT_EQ(buffer[2], 0xFF);
      EXPECT_EQ(buffer[3], 0x12);

      res = socket.recvRawBytes(buffer);
      EXPECT_EQ(res.status, gf::SocketStatus::Close);
    }

    clientThread.join();
  }

  template<gf::SocketFamily Family>
  void testTcpListenerNonBlocking() {
    gf::TcpListener listener(TestService, Family);
    ASSERT_TRUE(listener);

    std::thread clientThread([]() {
      gf::TcpSocket socket(Host, TestService, Family);
      ASSERT_TRUE(socket);

      socket.setNonBlocking();

      uint8_t buffer[4] = { 0x42, 0x69, 0xFF, 0x12 };
      auto res = socket.sendRawBytes(buffer);
      EXPECT_EQ(res.status, gf::SocketStatus::Data);
      EXPECT_EQ(res.length, 4u);

      res = socket.recvRawBytes(buffer);
      EXPECT_EQ(res.status, gf::SocketStatus::Block);
      EXPECT_EQ(res.length, 0u);
    });

    gf::TcpSocket socket = listener.accept();
    ASSERT_TRUE(socket);

    uint8_t buffer[10];
    auto res = socket.recvRawBytes(buffer);
    EXPECT_EQ(res.status, gf::SocketStatus::Data);
    EXPECT_EQ(res.length, 4u);
    EXPECT_EQ(buffer[0], 0x42);
    EXPECT_EQ(buffer[1], 0x69);
    EXPECT_EQ(buffer[2], 0xFF);
    EXPECT_EQ(buffer[3], 0x12);

    res = socket.recvRawBytes(buffer);
    EXPECT_EQ(res.status, gf::SocketStatus::Close);

    clientThread.join();
  }

  template<gf::SocketFamily Family>
  void testUdpSocketService() {
    gf::UdpSocket socket(TestService, Family);

    ASSERT_TRUE(socket);

    auto address = socket.getLocalAddress();
    std::cout << "host: " << address.getHostname(gf::SocketAddressFormat::Numeric) << '\n';
    std::cout << "service: " << address.getService(gf::SocketAddressFormat::Numeric) << '\n';
  }

  template<gf::SocketFamily Family>
  void testUdpSocketAny() {
    gf::UdpSocket socket(gf::Any, Family);

    ASSERT_TRUE(socket);

    auto address = socket.getLocalAddress();
    std::cout << "host: " << address.getHostname(gf::SocketAddressFormat::Numeric) << '\n';
    std::cout << "service: " << address.getService(gf::SocketAddressFormat::Numeric) << '\n';
  }

  template<gf::SocketFamily Family>
  void testUdpSocketOneWayCommunication() {
    gf::UdpSocket socket(TestService, Family);
    ASSERT_TRUE(socket);

    std::thread clientThread([]() {
      gf::UdpSocket socket(gf::Any, Family);
      ASSERT_TRUE(socket);

      auto actualFamily = socket.getLocalAddress().getFamily();

      gf::SocketAddress address = socket.getRemoteAddress(Host, TestService);
      EXPECT_EQ(address.getFamily(), actualFamily);

      uint8_t buffer[4] = { 0x42, 0x69, 0xFF, 0x12 };
      auto res = socket.sendRawBytesTo(buffer, address);
      EXPECT_EQ(res.status, gf::SocketStatus::Data);
      EXPECT_EQ(res.length, 4u);
    });

    auto actualFamily = socket.getLocalAddress().getFamily();
    gf::SocketAddress address;

    uint8_t buffer[10];
    auto res = socket.recvRawBytesFrom(buffer, address);
    EXPECT_EQ(res.status, gf::SocketStatus::Data);
    EXPECT_EQ(res.length, 4u);
    EXPECT_EQ(buffer[0], 0x42);
    EXPECT_EQ(buffer[1], 0x69);
    EXPECT_EQ(buffer[2], 0xFF);
    EXPECT_EQ(buffer[3], 0x12);

    EXPECT_EQ(address.getFamily(), actualFamily);

    clientThread.join();
  }

  template<gf::SocketFamily Family>
  void testUdpSocketTwoWayCommunication() {
    gf::UdpSocket socket(TestService, Family);
    ASSERT_TRUE(socket);

    std::thread clientThread([]() {
      gf::UdpSocket socket(gf::Any, Family);
      ASSERT_TRUE(socket);

      auto actualFamily = socket.getLocalAddress().getFamily();

      gf::SocketAddress address = socket.getRemoteAddress(Host, TestService);
      EXPECT_EQ(address.getFamily(), actualFamily);

      {
        uint8_t buffer[4] = { 0x42, 0x69, 0xFF, 0x12 };
        auto res = socket.sendRawBytesTo(buffer, address);
        EXPECT_EQ(res.status, gf::SocketStatus::Data);
        EXPECT_EQ(res.length, 4u);
      }

      {
        uint8_t buffer[10];
        auto res = socket.recvRawBytesFrom(buffer, address);
        EXPECT_EQ(res.status, gf::SocketStatus::Data);
        EXPECT_EQ(res.length, 4u);
        EXPECT_EQ(buffer[0], 0x23);
        EXPECT_EQ(buffer[1], 0x17);
        EXPECT_EQ(buffer[2], 0x21);
        EXPECT_EQ(buffer[3], 0x23);
        EXPECT_EQ(address.getFamily(), actualFamily);
      }
    });

    auto actualFamily = socket.getLocalAddress().getFamily();
    gf::SocketAddress address;

    {
      uint8_t buffer[10];
      auto res = socket.recvRawBytesFrom(buffer, address);
      EXPECT_EQ(res.status, gf::SocketStatus::Data);
      EXPECT_EQ(res.length, 4u);
      EXPECT_EQ(buffer[0], 0x42);
      EXPECT_EQ(buffer[1], 0x69);
      EXPECT_EQ(buffer[2], 0xFF);
      EXPECT_EQ(buffer[3], 0x12);
    }

    EXPECT_EQ(address.getFamily(), actualFamily);

    {
      uint8_t buffer[4] = { 0x23, 0x17, 0x21, 0x23 };
      auto res = socket.sendRawBytesTo(buffer, address);
      EXPECT_EQ(res.status, gf::SocketStatus::Data);
      EXPECT_EQ(res.length, 4u);
    }

    clientThread.join();
  }

}


TEST(SocketTest, TcpSocketDefault) {
  gf::TcpSocket socket;

  EXPECT_FALSE(socket);
}

TEST(SocketTest, TcpListenerDefault) {
  gf::TcpListener listener;

  EXPECT_FALSE(listener);
}

TEST(SocketTest, TcpListenerServiceUnspec) {
  testTcpListenerService<gf::SocketFamily::Unspec>();
}

TEST(SocketTest, TcpListenerServiceV4) {
  testTcpListenerService<gf::SocketFamily::IPv4>();
}

TEST(SocketTest, TcpListenerServiceV6) {
  testTcpListenerService<gf::SocketFamily::IPv6>();
}

TEST(SocketTest, TcpListenerOneClientUnspec) {
  testTcpListenerOneClient<gf::SocketFamily::Unspec>();
}

TEST(SocketTest, TcpListenerOneClientV4) {
  testTcpListenerOneClient<gf::SocketFamily::IPv4>();
}

TEST(SocketTest, TcpListenerOneClientV6) {
  testTcpListenerOneClient<gf::SocketFamily::IPv6>();
}

TEST(SocketTest, TcpListenerMultipleClientUnspec) {
  testTcpListenerMultipleClient<gf::SocketFamily::Unspec>();
}

TEST(SocketTest, TcpListenerMultipleClientV4) {
  testTcpListenerMultipleClient<gf::SocketFamily::IPv4>();
}

TEST(SocketTest, TcpListenerMultipleClientV6) {
  testTcpListenerMultipleClient<gf::SocketFamily::IPv6>();
}

TEST(SocketTest, TcpListenerNonBlockingUnspec) {
  testTcpListenerNonBlocking<gf::SocketFamily::Unspec>();
}

TEST(SocketTest, TcpListenerNonBlockingV4) {
  testTcpListenerNonBlocking<gf::SocketFamily::IPv4>();
}

TEST(SocketTest, TcpListenerNonBlockingV6) {
  testTcpListenerNonBlocking<gf::SocketFamily::IPv6>();
}

TEST(SocketTest, UdpSocketDefault) {
  gf::UdpSocket socket;

  EXPECT_FALSE(socket);
}

TEST(SocketTest, UdpSocketServiceUnspec) {
  testUdpSocketService<gf::SocketFamily::Unspec>();
}

TEST(SocketTest, UdpSocketServiceV4) {
  testUdpSocketService<gf::SocketFamily::IPv4>();
}

TEST(SocketTest, UdpSocketServiceV6) {
  testUdpSocketService<gf::SocketFamily::IPv6>();
}

TEST(SocketTest, UdpSocketAnyUnspec) {
  testUdpSocketAny<gf::SocketFamily::Unspec>();
}

TEST(SocketTest, UdpSocketAnyV4) {
  testUdpSocketAny<gf::SocketFamily::IPv4>();
}

TEST(SocketTest, UdpSocketAnyV6) {
  testUdpSocketAny<gf::SocketFamily::IPv6>();
}

TEST(SocketTest, UdpSocketOneWayCommunicationUnspec) {
  testUdpSocketOneWayCommunication<gf::SocketFamily::Unspec>();
}

TEST(SocketTest, UdpSocketOneWayCommunicationV4) {
  testUdpSocketOneWayCommunication<gf::SocketFamily::IPv4>();
}

TEST(SocketTest, UdpSocketOneWayCommunicationV6) {
  testUdpSocketOneWayCommunication<gf::SocketFamily::IPv6>();
}

TEST(SocketTest, UdpSocketTwoWayCommunicationUnspec) {
  testUdpSocketTwoWayCommunication<gf::SocketFamily::Unspec>();
}

TEST(SocketTest, UdpSocketTwoWayCommunicationV4) {
  testUdpSocketTwoWayCommunication<gf::SocketFamily::IPv4>();
}

TEST(SocketTest, UdpSocketTwoWayCommunicationV6) {
  testUdpSocketTwoWayCommunication<gf::SocketFamily::IPv6>();
}

#if 0
TEST(SocketTest, SocketSelector) {
    static constexpr int ClientCount = 10;
    static constexpr int ClientId = 3;

    gf::TcpListener listener(TestService, gf::SocketFamily::Unspec);
    ASSERT_TRUE(listener);

    std::thread clientThread([]() {
      std::vector<gf::TcpSocket> sockets;

      for (int i = 0; i < ClientCount; ++i) {
        gf::TcpSocket socket(Host, TestService, gf::SocketFamily::Unspec);
        ASSERT_TRUE(socket);

        sockets.push_back(std::move(socket));
      }

      uint8_t buffer[4] = { 0x42, 0x69, 0xFF, 0x12 };
      auto res = sockets[ClientId].sendRawBytes(buffer);
      EXPECT_EQ(res.status, gf::SocketStatus::Data);
      EXPECT_EQ(res.length, 4u);

      sockets.pop_back();

      res = sockets[ClientId].recvRawBytes(buffer);
      EXPECT_EQ(res.status, gf::SocketStatus::Data);
      EXPECT_EQ(res.length, 1u);
    });

    std::vector<gf::TcpSocket> sockets;

    gf::SocketSelector selector;
    selector.addSocket(listener);

    bool endOfTest = false;

    while (!endOfTest) {

      if (selector.wait() == gf::SocketSelectorStatus::Event) {
        for (auto& socket : sockets) {
          if (selector.isReady(socket)) {
            uint8_t buffer[10];
            auto res = socket.recvRawBytes(buffer);

            if (res.status == gf::SocketStatus::Data) {
              EXPECT_EQ(res.status, gf::SocketStatus::Data);
              EXPECT_EQ(res.length, 4u);
              EXPECT_EQ(buffer[0], 0x42);
              EXPECT_EQ(buffer[1], 0x69);
              EXPECT_EQ(buffer[2], 0xFF);
              EXPECT_EQ(buffer[3], 0x12);

              uint8_t byte = 0;
              socket.sendRawBytes(gf::array(&byte, 1));
            } else if (res.status == gf::SocketStatus::Close) {
              selector.removeSocket(socket);
              endOfTest = true;
            } else {
              FAIL();
            }
          }
        }

        if (selector.isReady(listener)) {
          gf::TcpSocket socket = listener.accept();
          ASSERT_TRUE(socket);

          sockets.push_back(std::move(socket));
          selector.addSocket(sockets.back());
        }
      } else {
        FAIL();
      }
    }

    clientThread.join();
}
#endif
