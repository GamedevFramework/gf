#include <gf/UdpSocket.h>

void dummyUdpSocketUsage1() {
  /// [udp_socket]
  // Create a socket
  gf::UdpSocket socket(gf::Any);

  if (!socket) {
    // Handle error
    return;
  }

  gf::SocketAddress address = socket.getRemoteAddress("example.com", "24680");

  uint8_t bytes[] = { 0x42, 0x69, 0x13, 0x12 };

  if (!socket.sendBytesTo(bytes, address)) {
    // Handle error
    return;
  }

  if (!socket.recvBytesFrom(bytes, address)) {
    // Handle error
    return;
  }
  /// [udp_socket]
}
