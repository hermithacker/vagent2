/*
 * Copyright (c) 2012 Varnish Software AS
 * All rights reserved.
 *
 * Author: Kristian Lyngstøl <kristian@bohemians.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "ipc.h"

struct agent_plugin_t *plugin_find(struct agent_core_t *core, const char *name)
{
	struct agent_plugin_t *plug;
	for (plug = core->plugins; plug != NULL; plug = plug->next) {
		if (!strcmp(name,plug->name))
			return plug;
	}
	return NULL;
}
struct agent_plugin_t *plugin_find_last(struct agent_core_t *core)
{
	struct agent_plugin_t *plug;
	for (plug = core->plugins; plug != NULL; plug = plug->next) {
		if (plug->next == NULL)
			return plug;
	}
	assert("Not reachable!");
	return NULL;
}

void plugin_alloc(char *name, struct agent_core_t *core)
{
	struct agent_plugin_t *plug = calloc(1, sizeof(struct agent_plugin_t));
	plug->ipc = calloc(1, sizeof(struct ipc_t ));
	ipc_init(plug->ipc);
	plug->name = strdup(name);
	plug->next = core->plugins;
	core->plugins = plug;
}
