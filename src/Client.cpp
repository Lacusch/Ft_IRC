#include "../incl/Client.hpp"

Client::Client(int fd) : _fd(fd), _isRegistered(false) { (void)_fd; }

Client::~Client() {}