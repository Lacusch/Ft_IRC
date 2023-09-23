#pragma once

#include "Client.hpp"
#include "Shared.hpp"
#include "Utils.hpp"

#define CHANNEL_USER_LIMIT 5

enum State { off, on };

class Channel {
   private:
    std::string _name;
    std::string _password;
    std::string _topic;
    unsigned int _limit;
    bool _inviteOnly;
    State _topicMode;  // off (only operators can change topic), on (everyone can change topic)
    State _passwordMode;
    std::map<int, Client *> _members;
    std::vector<std::string> _opsList;
    std::vector<std::string> _invitedList;

   public:
    Channel(std::string &name, std::string &password);
    ~Channel();

    // Channel Methods
    bool join(Client *client, int fd);    // join the channel specified
    bool invite(Client *client, int fd);  // invite a client to the channel
    bool leave(Client *client, int fd);   // leave the channel specified
    bool modifyOpsPrivileges(Client *target, char option);

    // Setters
    bool setTopic(const std::string &topic);        // set the chann`l topic
    bool setPassword(const std::string &password);  // set the channel password
    bool setUserLimit(unsigned int limit);
    void updateOpsListNick(std::string oldNick, std::string newNick); // updates the nickname of the operator that changes their nickname
    void addToInvitedList(Client *client);
    void removeFromInvitedList(Client *client);

    // Mode Setters
    void setTopicMode(State mode);
    void setPasswordMode(State mode);
    void setInviteOnlyMode(State mode);

    // Getters
    const std::string &getName() const;      // get the channel name
    const std::string &getTopic() const;     // get the channel topic
    const std::string &getPassword() const;  // get the channel password
    bool isInviteOnly() const;               // check if the channel is invite only
    bool getPasswordMode() const;            // check if the channel requires a password
    bool getTopicMode() const;               // check if the channel topic can be changed
    unsigned int getLimit() const;           // get the channel limit
    unsigned int getChannelSize() const;     // get the number of members
    Client *getClientByNickName(std::map<int, Client *> clients, std::string nickname) const;
    std::map<int, Client *> getMembersList(void) const;  // get the members of a channel
    std::vector<std::string> getOpsList(void) const;     // get the ops of a channel
    bool inInvitedList(Client *client) const;

    // Utils
    bool isMember(Client *client, int fd) const;  // check if a client is a member of the channel
    bool isTopicValid(const std::string &topic);  // check if the topic is valid
    bool userIsChannelOp(Client *client);
    void printMembers();    // get the members of a channel (Testing Purpose)
    void printOperators();  // get the ops of a channel (Testing Purpose)
};
