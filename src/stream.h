#ifndef CHAT_STREAM_H
#define CHAT_STREAM_H

#include <cstddef>

class Stream {
public:
    virtual ~Stream() {}
    virtual size_t getChunk(void *buffer, size_t bufferSize) = 0;
    virtual void sendChunk(void *buffer, size_t bufferSize) = 0;
};

#endif //CHAT_STREAM_H
