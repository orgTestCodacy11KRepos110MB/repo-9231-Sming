/****
 * Sming Framework Project - Open Source framework for high efficiency native ESP8266 development.
 * Created 2015 by Skurydin Alexey
 * http://github.com/SmingHub/Sming
 * All files of the Sming Core are provided under the LGPL v3 license.
 *
 * FileStream.h
 *
 ****/

#pragma once

#include "IFS/FileStream.h"
#include <FileSystem.h>

/**
 * @brief    File stream class
 * @ingroup  stream data
 */
class FileStream : public IFS::FileStream
{
public:
	using IFS::FileStream::FileStream;

	/** @brief  Create a file stream
     *  @param  fileName Name of file to open
     *  @param	openFlags
     */
	FileStream(const String& fileName, FileOpenFlags openFlags = File::ReadOnly) : FileStream()
	{
		open(fileName, openFlags);
	}

	using IFS::FileStream::attach;
};
