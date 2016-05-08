#ifndef CHAT_OPTIONS_H
#define CHAT_OPTIONS_H

class Options {
public:
    virtual ~Options() {}
    virtual void parse() = 0;
};

#endif //CHAT_OPTIONS_H