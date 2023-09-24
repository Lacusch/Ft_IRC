#pragma once

#include "Channel.hpp"
#include "Client.hpp"
#include "Request.hpp"
#include "Responses.hpp"
#include "Shared.hpp"

class Server {
   private:
    int _iport;
    int _fd;
    std::string _name;
    std::string _version;
    std::time_t _creation_date;
    std::string _port;
    std::string _password;
    std::vector<pollfd> _sockets;
    std::map<int, Client *> _clients;
    std::map<std::string, Channel *> _channels;

   public:
    Server(std::string name, std::string version, std::string port, std::string password);
    Server(const Server &rhs);
    Server &operator=(const Server &rhs);
    ~Server();

    void clean(void);
    // -----------------------
    // Getters
    // -----------------------
    std::string getName() const;
    std::string getVersion() const;
    std::string getPort() const;
    std::string getCreationDate() const;
    std::string getPassword() const;
    int getFdFromNickName(std::string nickname);
    std::map<int, Client *> getClientsList() const;
    Channel *getChannel(std::string channel_name) const;

    int valid_args();
    int start_server();
    int run();
    int newClient();
    int clientMessage(int i);
    int sendMessage(int fd, Res res, Request req);
    bool nickNameInUse(std::string nickname);
    bool channelExists(std::string channel_name);

    // -----------------------
    //				Messages
    // -----------------------
    int handlePassword(int fd, Request req);
    int handleNickName(int fd, Request req);
    int handleUser(int fd, Request req);
    int handlePrivateMsg(int fd, Request req);
    int handleJoinChannel(int fd, Request req);
    int handleSingleChannel(int fd, Request req, std::string channel, std::string key);
    int handleChannelMessage(int fd, Request req);
    int handlePing(int fd, Request req);
    int handleWho(int fd, Request req);
    int handleMode(int fd, Request req);
    int handleKick(int fd, Request req);
    int handleInvite(int fd, Request req);
    int handleTopic(int fd, Request req);
    int handleChannelMode(int fd, Request req, Channel *ch);
    int handlePart(int fd, Request req);
    int handleQuit(int fd, Request req);
    int broadcastChannel(int fd, Request req, Channel *channel, Res res);
    int sendRegisteredUsers(int fd, Request req, Channel *channel);
    int broadcastQuitMsg(int fd, Channel *channel);

    Res handleKeyMode(Request req, Channel *ch);
    Res handleUserLimitMode(Request req, Channel *ch);
    Res handleInviteOnlyMode(Request req, Channel *ch);
    Res handleTopicMode(Request req, Channel *ch);
    Res handleOperatorMode(Client *target, Request req, Channel *ch);
    std::string create_response(int fd, Res res, Request req);
    void updateOpNickName(Client *client, std::string newNickName);
};
