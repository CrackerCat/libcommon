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

#ifndef PG_TRANSACTION_HXX
#define PG_TRANSACTION_HXX

#include "Connection.hxx"
#include "Error.hxx"

namespace Pg {

/**
 * Like Connection::DoSerializable(), but retry on
 * "serialization_failure" (Error::IsSerializationFailure()).
 */
template<typename F>
void DoSerializableRepeat(Pg::Connection &connection, unsigned retries,
			  F &&f) {
	while (true) {
		try {
			connection.DoSerializable(f);
			break;
		} catch (const Pg::Error &e) {
			if (e.IsSerializationFailure() &&
			    retries-- > 0)
				/* try again */
				continue;

			throw;
		}
	}
}

/**
 * Like Connection::DoRepeatableRead(), but retry on
 * "serialization_failure" (Error::IsSerializationFailure()).
 */
template<typename F>
void DoRepeatableReadRepeat(Pg::Connection &connection, unsigned retries,
			  F &&f) {
	while (true) {
		try {
			connection.DoRepeatableRead(f);
			break;
		} catch (const Pg::Error &e) {
			if (e.IsSerializationFailure() &&
			    retries-- > 0)
				/* try again */
				continue;

			throw;
		}
	}
}

} /* namespace Pg */

#endif
