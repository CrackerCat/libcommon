/*
 * Copyright 2007-2017 Content Management AG
 * All rights reserved.
 *
 * author: Max Kellermann <mk@cm4all.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * FOUNDATION OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef BENG_PROXY_TRANSLATE_READER_HXX
#define BENG_PROXY_TRANSLATE_READER_HXX

#include "Protocol.hxx"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

class AllocatorPtr;

/**
 * Parse translation response packets.
 */
class TranslatePacketReader {
    enum class State {
        HEADER,
        PAYLOAD,
        COMPLETE,
    };

    State state = State::HEADER;

    TranslationHeader header;

    char *payload;
    size_t payload_position;

public:
    /**
     * Read a packet from the socket.
     *
     * @return the number of bytes consumed
     */
    size_t Feed(AllocatorPtr alloc, const uint8_t *data, size_t length);

    bool IsComplete() const {
        return state == State::COMPLETE;
    }

    TranslationCommand GetCommand() const {
        assert(IsComplete());

        return header.command;
    }

    const void *GetPayload() const {
        assert(IsComplete());

        return payload != nullptr
            ? payload
            : "";
    }

    size_t GetLength() const {
        assert(IsComplete());

        return header.length;
    }
};

#endif
