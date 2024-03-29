/**
 * hostapi.h - Host functions for use by applications
 *
 * Copyright 2019 mikee47 <mike@sillyhouse.net>
 *
 * This file is part of the Sming Framework Project
 *
 * This library is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, version 3 or later.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with SHEM.
 * If not, see <https://www.gnu.org/licenses/>.
 *
 ****/

#pragma once

#include "include/hostlib/hostmsg.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Instruct the host to finish and return the given code to the OS
 */
void host_exit(int code);

#ifdef __cplusplus
}
#endif
