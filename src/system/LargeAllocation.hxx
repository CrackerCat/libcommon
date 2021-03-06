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

#ifndef LARGE_ALLOCATION_HXX
#define LARGE_ALLOCATION_HXX

#include <utility>

#include <stddef.h>

/**
 * Allocates anonymous memory using mmap().
 */
class LargeAllocation {
	void *data = nullptr;
	size_t the_size;

public:
	LargeAllocation() = default;

	/**
	 * Throws std::bad_alloc on error.
	 */
	explicit LargeAllocation(size_t _size);

	LargeAllocation(LargeAllocation &&src) noexcept
		:data(std::exchange(src.data, nullptr)), the_size(src.the_size) {}

	~LargeAllocation() noexcept {
		if (data != nullptr)
			Free(data, the_size);
	}

	LargeAllocation &operator=(LargeAllocation &&src) noexcept {
		using std::swap;
		swap(data, src.data);
		swap(the_size, src.the_size);
		return *this;
	}

	void reset() noexcept {
		if (data != nullptr) {
			Free(data, the_size);
			data = nullptr;
		}
	}

	void *get() const noexcept {
		return data;
	}

	size_t size() const noexcept {
		return the_size;
	}

private:
	static void Free(void *p, size_t size) noexcept;
};

#endif
