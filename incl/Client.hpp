#include "Shared.hpp"

class Client {
   private:
    int _fd;

   public:
    Client(int fd);
    bool _isRegistered;
    std::string _nickName;

    Client(const Client &rhs);
    Client &operator=(const Client &rhs);
    ~Client();
};