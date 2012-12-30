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

#ifndef COMMON_H
#define COMMON_H

#include <pthread.h>

/*
 * Configuration, handled by main for now.
 */
struct agent_config_t {
	double timeout;
	char *T_arg;
	char *S_arg;
	char *n_arg;
	char *c_arg;
};

/*
 * Core structure.
 *
 * Only contains the config list and plugins, but is used all over.
 *
 * While searching for a plugin frequently is "wasteful", the number of
 * plugins present is so few that the simplicity offered outweighs the
 * disadvantages.
 */
struct agent_core_t {
	struct agent_config_t *config;
	struct agent_plugin_t *plugins;
};

/*
 * Generic plugin structure.
 *
 * Name is the text name of the plugin, which should match the file name.
 * It is used to search for the plugin.
 * *data is a private data structure for the plugin.
 * the start-function is run to start the plugin. If the plugin only
 * listens to an IPC (e.g: varnishadm, logd, ...) this can be set to
 * ipc_start. Otherwise a custom function needs to be provided, or NULL for
 * plugins that do not run in the background (e.g: they trigger in the
 * context of other plugins, like /html/).
 * thread needs to be set to the thread, if it exists.
 *
 * XXX: Having both a return type of pthread_t on start and a
 * thread-reference here is a bit redundant...
 */
struct agent_plugin_t {
	const char *name;
	void *data;
	struct ipc_t *ipc;
	struct agent_plugin_t *next;
	pthread_t *(*start)(struct agent_core_t *core, const char *name);
	pthread_t *thread;
};

/*
 * Logger macro to include file, func, line etc.
 * Register with the logd-plugin and use that as the handle.
 */
#define logger(l,...) logger_real(l, __FILE__, __func__, __LINE__, __VA_ARGS__)

/*
 * Avoid using this directly.
 *
 * FIXME: Should be fixed to just be ipc_run directly now that it does
 * va_args.
 */
void logger_real(int handle, const char *file, const char *func, const unsigned int line, const char *fmt, ...);

#endif