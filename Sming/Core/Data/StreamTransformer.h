/****
 * Sming Framework Project - Open Source framework for high efficiency native ESP8266 development.
 * Created 2015 by Skurydin Alexey
 * http://github.com/SmingHub/Sming
 * All files of the Sming Core are provided under the LGPL v3 license.
 *
 * StreamTransformer.h
 *
 * @author Slavey Karadzhov <slaff@attachix.com>
 *
 ****/

#pragma once

#include "Buffer/CircularBuffer.h"

/**
 * @brief Class that can be used to transform streams of data on the fly
 */
class StreamTransformer : public IDataSourceStream
{
public:
	StreamTransformer(IDataSourceStream* stream, size_t resultSize = 256, size_t blockSize = 64)
		: sourceStream(stream), result(new uint8_t[resultSize]), resultSize(resultSize), blockSize(blockSize)
	{
	}

	~StreamTransformer()
	{
		delete[] result;
		delete tempStream;
		delete sourceStream;
	}

	//Use base class documentation
	StreamType getStreamType() const override
	{
		return sourceStream->getStreamType();
	}

	/**
	 * @brief Return the total length of the stream
	 * @retval int -1 is returned when the size cannot be determined
	*/
	int available() override
	{
		return -1;
	}

	bool isValid() const
	{
		return sourceStream != nullptr && sourceStream->isValid();
	}

	uint16_t readMemoryBlock(char* data, int bufSize) override;

	bool seek(int len) override;

	bool isFinished() override;

	String getName() const override
	{
		return (sourceStream == nullptr) ? nullptr : sourceStream->getName();
	}

	/**
	 * @brief A method that backs up the current state
	 *
	 */
	virtual void saveState(){};

	/**
	 * @brief A method that restores the last backed up state
	 */
	virtual void restoreState(){};

protected:
	/**
	 * @brief Inherited class implements this method to transform a block of data
	 * @param in source data
	 * @param inLength source data length
	 * @param out output buffer
	 * @param outLength size of output buffer
	 * @retval size_t number of output bytes written
	 * @note Called with `in = nullptr` and `inLength = 0` at end of input stream
	 */
	virtual size_t transform(const uint8_t* in, size_t inLength, uint8_t* out, size_t outLength) = 0;

private:
	void fillTempStream(char* buffer, size_t bufSize);

	IDataSourceStream* sourceStream{nullptr};
	CircularBuffer* tempStream{nullptr};
	uint8_t* result{nullptr};
	size_t resultSize;
	size_t blockSize;
};
