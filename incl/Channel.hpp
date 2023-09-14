#pragma once

#include <map>
#include <string>
#include <limits>

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
    std::map<std::string, std::vector<std::string> > _ops;                          // channel name is key and list of ops is value
    State _topicMode;                                                               // off (only operators can change topic), on (everyone can change topic)
    State _passwordMode;
    State _userLimitMode;

   public:
    Channel(std::string &name, std::string &password);
    ~Channel();

    // Channel Methods
    bool join(Client *client, int fd);                                              // join the channel specified
    bool kick(Client *client, int fd);                                              // kick a client from the channel
    bool invite(Client *client, int fd);                                            // invite a client to the channel
    bool leave(Client *client, int fd);                                             // leave the channel specified
    bool modifyOpsPrivileges(const std::string &channel_name, const std::string &nickname, char option);

    // Setters
    bool setTopic(const std::string &topic);                                       // set the chann`l topic
    bool setPassword(const std::string &password);                                 // set the channel password
    bool setUserLimit(unsigned int limit);

    // Mode Setters
    void setTopicMode(State mode);
    void setPasswordMode(State mode);
    void setInviteOnlyMode(State mode);
    void setUserLimitMode(State mode);

    // Getters
    const std::string &getName() const;                                            // get the channel name
    const std::string &getTopic() const;                                           // get the channel topic
    const std::string &getPassword() const;                                        // get the channel password
    bool isInviteOnly() const;                                                     // check if the channel is invite only
    bool getPasswordMode() const;                                                  // check if the channel requires a password
    bool getTopicMode() const;                                                     // check if the channel topic can be changed
    void printMembers();                                                             // get the members of a channel
    unsigned int getLimit() const;                                                 // get the channel limit
    unsigned int getChannelSize() const;                                           // get the number of members
    std::map<int, Client *> getMembersList(void);                                  // get the members of a channel
    std::map<std::string, std::vector<std::string> > getOpsList(void) const;       // get the ops of a channel

    // Utils
    bool isMember(Client *client, int fd) const;                                    // check if a client is a member of the channel
    bool isTopicValid(const std::string &topic);                                    // check if the topic is valid
    static bool userIsChannelOp(Client *client, Channel *chName);
};
