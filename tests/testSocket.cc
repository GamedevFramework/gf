/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2018 Julien Bernard
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
#include <gf/TcpListener.h>
#include <gf/TcpSocket.h>

#include <cstdint>
#include <thread>

#include "gtest/gtest.h"


TEST(SocketTest, TcpSocketDefault) {
  gf::TcpSocket socket;

  EXPECT_FALSE(socket);
}

// TEST(SocketTest, TcpSocketService) {
//   gf::TcpSocket socket("google.com", "80");
//
//   EXPECT_TRUE(socket);
// }

TEST(SocketTest, TcpListenerDefault) {
  gf::TcpListener listener("12345");

  EXPECT_TRUE(listener);
}

TEST(SocketTest, TcpListenerOneClient) {
  std::thread serverThread([]() {
    gf::TcpListener listener("12345");
    EXPECT_TRUE(listener);

    gf::TcpSocket socket = listener.accept();
    EXPECT_TRUE(socket);

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
  });

  std::this_thread::sleep_for(std::chrono::milliseconds(200));

  {
    gf::TcpSocket socket("localhost", "12345");
    EXPECT_TRUE(socket);

    uint8_t buffer[4] = { 0x42, 0x69, 0xFF, 0x12 };
    auto res = socket.sendRawBytes(buffer);
    EXPECT_EQ(res.status, gf::SocketStatus::Data);
    EXPECT_EQ(res.length, 4u);
  }

  serverThread.join();
}

TEST(SocketTest, TcpListenerMultipleClient) {
  static constexpr int ClientCount = 10;

  std::thread serverThread([]() {
    gf::TcpListener listener("12345");
    EXPECT_TRUE(listener);

    for (int i = 0; i < ClientCount; ++i) {
      gf::TcpSocket socket = listener.accept();

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
  });

  std::this_thread::sleep_for(std::chrono::milliseconds(200));

  for (int i = 0; i < ClientCount; ++i) {
    gf::TcpSocket socket("localhost", "12345");
    EXPECT_TRUE(socket);

    uint8_t buffer[4] = { 0x42, 0x69, 0xFF, 0x12 };
    auto res = socket.sendRawBytes(buffer);
    EXPECT_EQ(res.status, gf::SocketStatus::Data);
    EXPECT_EQ(res.length, 4u);
  }

  serverThread.join();
}
