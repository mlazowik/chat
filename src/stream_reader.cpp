#include <stdexcept>
#include "stream_reader.h"

StreamReader::StreamReader(Stream &stream, void *buffer, size_t bytes)
        : stream(stream), buffer(buffer), bytes(bytes) {
}

void StreamReader::readNextChunk() {
    if (this->finished()) {
        throw std::logic_error("cannot read: already finished");
    }

    size_t remains = bytes - this->bufferPosition;

    this->bufferPosition += this->stream.getChunk(
            (char*)this->buffer + this->bufferPosition, remains);
}

bool StreamReader::finished() const {
    return this->bufferPosition == this->bytes;
}