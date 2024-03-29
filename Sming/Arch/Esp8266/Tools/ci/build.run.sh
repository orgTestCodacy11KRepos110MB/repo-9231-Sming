#!/bin/bash
#
# Esp8266 build.run.sh

$MAKE_PARALLEL samples
make clean samples-clean
$MAKE_PARALLEL Basic_Blink ENABLE_CUSTOM_HEAP=1 DEBUG_VERBOSE_LEVEL=3
$MAKE_PARALLEL HttpServer_ConfigNetwork ENABLE_CUSTOM_LWIP=2 STRICT=1

$MAKE_PARALLEL component-samples
