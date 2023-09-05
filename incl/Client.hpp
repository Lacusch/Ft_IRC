#include <string>

#include "Shared.hpp"

class Client {
   private:
    int _fd;
    bool _isAuthenticated;
    bool _isRegistered;
    bool _isOperator;
    bool _welcomeMessageDelivered;
    std::string _nickName;
    std::string _userName;
    std::string _realName;

   public:
    Client(int fd);
    Client(const Client &rhs);
    Client &operator=(const Client &rhs);
    ~Client();

    bool isAuthenticated(void) const;
    bool isRegistered(void) const;
    bool isOperator(void) const;

    void setRegistration(bool isRegistered);
    void setAuthentication(bool isAuthenticated);
    void setOperator(bool isOperator);
    void setWelcomeMessageDelivered(bool delivered);
    void setUsername(std::string userName);
    void setRealName(std::string realName);

    void setNickName(std::string nickname);

    std::string getNickName(void) const;
    std::string getUserName(void) const;
    bool getWelcomeMessageDelivered(void) const;
};