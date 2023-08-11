
class Client {
   public:
    Client(int fd);
    Client(const Client &rhs);
    Client &operator=(const Client &rhs);
    ~Client();

   private:
    int _fd;
};