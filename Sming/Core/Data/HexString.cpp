/****
 * Sming Framework Project - Open Source framework for high efficiency native ESP8266 development.
 * Created 2015 by Skurydin Alexey
 * http://github.com/SmingHub/Sming
 * All files of the Sming Core are provided under the LGPL v3 license.
 *
 * HexString.cpp
 *
 */

#include "HexString.h"
#include <stringutil.h>

String makeHexString(const void* data, unsigned length, char separator)
{
	if(data == nullptr || length == 0) {
		return nullptr;
	}

	String result;

	unsigned stringLength = length * 2; // 2 hex chars per byte
	if(separator != '\0') {
		stringLength += length - 1;
	}
	if(!result.setLength(stringLength)) {
		return nullptr;
	}

	auto inptr = static_cast<const uint8_t*>(data);
	char* outptr = result.begin();
	for(unsigned i = 0; i < length; ++i, ++inptr) {
		if(i != 0 && separator != '\0') {
			*outptr++ = separator;
		}
		*outptr++ = hexchar(*inptr >> 4);
		*outptr++ = hexchar(*inptr & 0x0F);
	}

	return result;
}
