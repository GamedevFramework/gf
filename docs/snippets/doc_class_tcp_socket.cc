#include <gf/TcpSocket.h>

void dummyTcpSocketUsage1() {
  /// [tcp_socket]
  // Create a socket to example.com on port 24680
  gf::TcpSocket socket("example.com", "24680");

  if (!socket) {
    // Handle error
    return;
  }

  uint8_t bytes[] = { 0x42, 0x69, 0x13, 0x12 };

  if (socket.sendBytes(bytes) != gf::SocketStatus::Data) {
    // Handle error
    return;
  }


  if (socket.recvBytes(bytes) != gf::SocketStatus::Data) {
    // Handle error
    return;
  }
  /// [tcp_socket]
}

