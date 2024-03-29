/**
 * options.h - command-line option parsing
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

/*
 * tag			option tag
 * has_arg 		no_argument, optional_argument, required_argument
 * brief		Brief description of option
 * argname		Name of argument, if any
 * argdesc		Description of argument
 * longdesc		Long description, lines are null-terminated
 */
#define OPTION_MAP(XX)                                                                                                 \
	XX(help, no_argument, "Show help", nullptr, nullptr, nullptr)                                                      \
	XX(uart, required_argument, "Enable UART server", "PORT", "Which UART number to enable",                           \
	   "e.g. --uart=0 --uart=1 enable servers for UART0, UART1\0")                                                     \
	XX(device, required_argument, "Set device for uart", "DEVICE",                                                     \
	   "Optionally map uart to device. Use `console` to change printf target.",                                        \
	   "e.g. --uart=0 --device=/dev/ttyUSB0\0"                                                                         \
	   "     --uart=1 --device=console\0")                                                                             \
	XX(baud, required_argument, "Set baud rate for UART", "BAUD", "Requires --device argument",                        \
	   "e.g. --uart=0 --device=/dev/ttyUSB0 --baud=115200\0")                                                          \
	XX(portbase, required_argument, "Specify base port number for UART socket servers", "PORT", "IP port number",      \
	   nullptr)                                                                                                        \
	XX(ifname, required_argument, "Specify network interface", "NAME", "Network interface to use (e.g. tap0)",         \
	   nullptr)                                                                                                        \
	XX(ipaddr, required_argument, "Specify network IP address", "ADDR", "IP4 network address (e.g. 192.168.13.2)",     \
	   nullptr)                                                                                                        \
	XX(gateway, required_argument, "Specify network gateway address", "ADDR",                                          \
	   "IP4 network address (e.g. 192.168.1.254)", nullptr)                                                            \
	XX(netmask, required_argument, "Specify IP network mask", "MASK", "e.g. 255.255.255.0", nullptr)                   \
	XX(pause, optional_argument, "Pause at startup", "SECS", "How long to pause for, omit to wait for ENTER", nullptr) \
	XX(exitpause, optional_argument, "Pause at exit", "SECS", "How long to pause for, omit to wait for ENTER",         \
	   nullptr)                                                                                                        \
	XX(flashfile, required_argument, "Use alternative flash backing file", "FILENAME", "Path to flash backing file",   \
	   nullptr)                                                                                                        \
	XX(flashsize, required_argument, "Change default flash size if file doesn't exist", "SIZE",                        \
	   "Size of flash in bytes (e.g. 512K, 524288, 0x80000)", nullptr)                                                 \
	XX(initonly, no_argument, "Initialise only, do not start Sming", nullptr, nullptr, nullptr)                        \
	XX(loopcount, required_argument, "Run Sming loop a fixed number of times then exit", nullptr, nullptr,             \
	   "Useful for running samples in CI\0")                                                                           \
	XX(nonet, no_argument, "Skip network initialisation", nullptr, nullptr, nullptr)                                   \
	XX(debug, required_argument, "Set debug verbosity", "LEVEL", "Maximum debug message level to print",               \
	   "0 = errors only, 1 = +warnings, 2 = +info\0")                                                                  \
	XX(cpulimit, required_argument, "Set CPU limit", "COUNT", "0 = no limit", nullptr)

enum option_tag_t {
#define XX(tag, has_arg, desc, argname, arghelp, examples) opt_##tag,
	OPTION_MAP(XX)
#undef XX
		opt_none = -1
};

option_tag_t get_option(int argc, char* argv[], const char*& arg);
int get_first_non_option();
void print_help();
