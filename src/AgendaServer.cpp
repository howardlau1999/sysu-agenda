#include "AgendaServer.hpp"

AgendaSession::AgendaSession(tcp::socket t_socket)
    : m_socket(std::move(t_socket)) {}