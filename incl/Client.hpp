
class Client {
   private:
    int _fd;

   public:
    Client(int fd);
    bool _isRegistered;
    // Client(const Client &rhs);
    // Client &operator=(const Client &rhs);
    ~Client();
};