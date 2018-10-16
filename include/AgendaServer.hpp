#define ASIO_STANDALONE
#include <asio.hpp>
#include <memory>
#include <utility>
#include <restbed>
#include "AgendaService.hpp"
using asio::ip::tcp;

class AgendaServer {
   public:
    AgendaServer(const AgendaServer&) = delete;

    void startServer() {
        asio::io_context io_context;
        asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), 9000);
    }

    void stopServer() {}

   private:
    AgendaService m_agendaService;
};

class AgendaSession : std::enable_shared_from_this<AgendaSession> {
   tcp::socket m_socket; 
   public:
   AgendaSession(tcp::socket t_socket);
};