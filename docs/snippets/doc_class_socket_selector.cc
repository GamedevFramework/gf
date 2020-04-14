#include <gf/SocketSelector.h>
#include <gf/TcpListener.h>
#include <gf/TcpSocket.h>

void dummySocketSelectorUsage1(const std::string& service) {
  /// [socket_selector]
  gf::TcpListener listener(service);

  gf::SocketSelector selector;
  selector.addSocket(listener);

  std::vector<gf::TcpSocket> sockets;

  for (;;) {
    if (selector.wait() == gf::SocketSelectorStatus::Event) {
      for (auto& socket : sockets) {
        if (selector.isReady(socket)) {
          // read something on the socket
          // ...
        }
      }

      if (selector.isReady(listener)) {
        // the listener is ready, accept a new connection
        gf::TcpSocket socket = listener.accept();
        sockets.push_back(std::move(socket));
        selector.addSocket(sockets.back());
      }
    }
  }
  /// [socket_selector]
}
