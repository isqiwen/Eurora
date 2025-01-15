/*** Base Channel definition ***/
class Channel {
public:
    virtual ~Channel() = default;

    virtual void push(Message message)   = 0;
    virtual std::optional<Message> pop() = 0;
    virtual void close()                 = 0;
};
