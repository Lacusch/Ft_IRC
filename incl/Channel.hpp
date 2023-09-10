#pragma once

#include <map>
#include <string>

#include "Client.hpp"
#include "Utils.hpp"

enum State { off, on };

class Channel {
   private:
    std::string _name;
    std::string _password;
    std::string _topic;
    unsigned int _limit;
    bool _inviteOnly;
    std::map<int, Client *> _members;
    State _topicMode;
    State _passwordMode;
    State _userLimitMode;

   public:
    Channel(std::string &name, std::string &password);
    ~Channel();

    // Channel Methods
    bool join(Client *client, int fd);    // join the channel specified
    bool kick(Client *client, int fd);    // kick a client from the channel
    bool invite(Client *client, int fd);  // invite a client to the channel
    bool leave(Client *client, int fd);   // leave the channel specified

    // Setters
    bool setTopic(const std::string &topic);        // set the chann`l topic
    bool setPassword(const std::string &password);  // set the channel password
    bool setUserLimit(unsigned int limit);

    // Mode Setters
    void setTopicMode(State mode);
    void setPasswordMode(State mode);
    void setInviteOnlyMode(State mode);
    void setUserLimitMode(State mode);

    // Getters
    const std::string &getName() const;      // get the channel name
    const std::string &getTopic() const;     // get the channel topic
    const std::string &getPassword() const;  // get the channel password
    unsigned int getLimit() const;           // get the channel limit
    bool isInviteOnly() const;               // check if the channel is invite only
    void getMembers();                       // get the members of a channel
    std::map<int, Client *> getMembersList(void);

    // Utils
    bool isMember(Client *client, int fd) const;  // check if a client is a member of the channel
    bool isTopicValid(const std::string &topic);  // check if the topic is valid
};
