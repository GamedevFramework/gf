#include <gf/TcpListener.h>

void dummyTcpListenerUsage1() {
  /// [listener_port]
  // Create a listener on port 24680
  gf::TcpListener listener("24680");
  /// [listener_port]
}

void dummyTcpListenerUsage2() {
  /// [listener_service]
  // Create a listener on port 80 (http)
  gf::TcpListener listener("http");
  /// [listener_service]
}

void dummyTcpListenerUsage3() {
  /// [listener_loop]
  // Create a listener
  gf::TcpListener listener("24680");

  for (;;) {
    // Accept a new connection ...
    gf::TcpSocket client = listener.accept();

    if (client) {
      // and handle the client...

    }
  }
  /// [listener_loop]
}
