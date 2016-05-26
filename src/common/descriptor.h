#ifndef CHAT_DESCRIPTOR_H
#define CHAT_DESCRIPTOR_H

class Descriptor {
public:
    virtual ~Descriptor() {};
    virtual int getDescriptor() const = 0;
    virtual void destroy() = 0;

    virtual bool operator==(const Descriptor &rhs) const = 0;
    virtual bool operator!=(const Descriptor &rhs) const = 0;
    virtual bool operator<(const Descriptor &rhs) const = 0;
    virtual bool operator>(const Descriptor &rhs) const = 0;
};

#endif //CHAT_DESCRIPTOR_H
