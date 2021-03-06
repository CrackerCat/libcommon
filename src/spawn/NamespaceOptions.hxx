/*
 * Copyright 2007-2018 Content Management AG
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

#pragma once

#include "MountNamespaceOptions.hxx"
#include "translation/Features.hxx"

#include "util/Compiler.h"

class AllocatorPtr;
struct MountList;
struct SpawnConfig;
struct UidGid;
class MatchInfo;

struct NamespaceOptions {
	/**
	 * Start the child process in a new user namespace?
	 */
	bool enable_user = false;

	/**
	 * Start the child process in a new PID namespace?
	 */
	bool enable_pid = false;

	/**
	 * Start the child process in a new Cgroup namespace?
	 */
	bool enable_cgroup = false;

	/**
	 * Start the child process in a new network namespace?
	 */
	bool enable_network = false;

	/**
	 * Start the child process in a new IPC namespace?
	 */
	bool enable_ipc = false;

	/**
	 * The name of the PID namespace to reassociate with.  The
	 * namespace is requested from the "Spawn" daemon (Package
	 * cm4all-spawn).
	 */
	const char *pid_namespace = nullptr;

	/**
	 * The name of the network namespace (/run/netns/X) to reassociate
	 * with.  Requires #enable_network.
	 */
	const char *network_namespace = nullptr;

	/**
	 * The hostname of the new UTS namespace.
	 */
	const char *hostname = nullptr;

	MountNamespaceOptions mount;

	NamespaceOptions() = default;
	NamespaceOptions(AllocatorPtr alloc, const NamespaceOptions &src);

#if TRANSLATION_ENABLE_EXPAND
	gcc_pure
	bool IsExpandable() const;

	/**
	 * Throws std::runtime_error on error.
	 */
	void Expand(AllocatorPtr alloc, const MatchInfo &match_info);
#endif

	gcc_pure
	int GetCloneFlags(int flags) const;

	void SetupUidGidMap(const UidGid &uid_gid,
			    int pid) const;

	/**
	 * Apply #pid_namespace.  This will affect new child
	 * processes, but not this process.
	 */
	void ReassociatePid() const;

	/**
	 * Apply #network_namespace.
	 */
	void ReassociateNetwork() const;

	/**
	 * Throws std::system_error on error.
	 */
	void Setup(const UidGid &uid_gid) const;

	char *MakeId(char *p) const;

	const char *GetJailedHome() const {
		return mount.GetJailedHome();
	}
};
