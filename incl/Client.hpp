#pragma once

#include "Shared.hpp"

class Client {
   protected:
    int _fd;
    std::string _host;
    bool _isAuthenticated;
    bool _isRegistered;
    bool _hasNickname;
    bool _welcomeMessageDelivered;
    std::string _nickName;
    std::string _userName;
    std::string _realName;
    std::string _msgBuffer;

   public:
    Client(int fd, std::string host);
    Client(const Client &rhs);
    Client &operator=(const Client &rhs);
    virtual ~Client();

    void setMsg(std::string msg);
    std::string getMsg(void) const;
    void clearMsg(void);

    bool isAuthenticated(void) const;
    bool isRegistered(void) const;
    bool hasNickname(void) const;
    void setRegistration(bool isRegistered);
    void setHasNickname(bool hasNickname);
    void setAuthentication(bool isAuthenticated);
    void setWelcomeMessageDelivered(bool delivered);
    void setUsername(std::string userName);
    void setRealName(std::string realName);
    void setNickName(std::string nickname);
    std::string getNickName(void) const;
    std::string getUserName(void) const;
    std::string getHost(void) const;
    int getFd(void) const;
    bool getWelcomeMessageDelivered(void) const;
};