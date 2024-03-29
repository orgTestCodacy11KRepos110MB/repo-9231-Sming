/***************************************************
  This is a library for the Adafruit 1.8" SPI display.

This library works with the Adafruit 1.8" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/358
The 1.8" TFT shield
  ----> https://www.adafruit.com/product/802
The 1.44" TFT breakout
  ----> https://www.adafruit.com/product/2088
as well as Adafruit raw 1.8" TFT display
  ----> http://www.adafruit.com/products/618

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
/****************************************************
 * ported for Sming by H.Boettcher.
 * this implementation uses SPIFFS storage instead of SD cards
 * hbottc@gmail.com
 ***************************************************/
/*
 * Revised as a template class so it can be used with any supported Adafruit display library,
 * such as Adafruit_ILI9341, Adafruit_ST7735, etc.
 * June 2019 mikee47 <mikee@sillyhouse.net>
 */

#pragma once

#include "Arduino.h"

/*
 * This function opens a Windows Bitmap (BMP) file and displays it at the given coordinates.
 * It's sped up by reading many pixels worth of data at a time (rather than pixel by pixel).
 * Increasing the buffer size takes more of the esp8266's precious RAM but makes loading a
 * little faster.  20 pixels seems a good balance.
 *
 * The `Adafruit_TFT` class must provide these methods:
 *
 *		unsigned width()
 * 		unsigned height()
 *      startWrite()
 *      endWrite()
 * 		setAddrWindow(x1, y1, x2, y2)
 * 		writePixels(uint16_t rgb565)
 *
 * Return types and parameters don't need to be exact.
 * Note that this would probably be more efficient using a virtual base class...
*/

template <class Adafruit_TFT> bool bmpDraw(Adafruit_TFT& tft, String fileName, uint8_t x, uint8_t y)
{
	if((x >= tft.width()) || (y >= tft.height())) {
		return false;
	}

	debug_i("Loading image '%s'", fileName.c_str());

	uint32_t startTime = millis();

	File file;
	if(!file.open(fileName)) {
		debug_e("File wasn't found: %s", fileName.c_str());
		return false;
	}

	// These read 16- and 32-bit types from the SPIFFS file.
	// BMP data is stored little-endian, esp8266 is little-endian too.
	// May need to reverse subscript order if porting elsewhere.

	auto read16 = [&file]() -> uint16_t {
		char bytes[2];
		file.read(bytes, 2);
		return (bytes[1] << 8) + bytes[0];
	};

	auto read32 = [&file]() -> uint32_t {
		char bytes[4];
		file.read(bytes, 4);
		return (bytes[3] << 24) + (bytes[2] << 16) + (bytes[1] << 8) + bytes[0];
	};

	// Parse BMP header
	bool goodBmp = false; // Set to true on valid header parse
	while(true) {
		if(read16() != 0x4D42) { // BMP signature
			debug_e("Invalid BMP signature");
			break;
		}

		debug_i("File size: %d", read32()); // get File Size
		(void)read32();						// Read & ignore creator bytes
		uint32_t bmpImageoffset = read32(); // Start of image data
		debug_i("Image Offset: %d", bmpImageoffset);
		debug_i("Header size: %d", read32()); // Read DIB header
		int bmpWidth = read32();
		int bmpHeight = read32();
		if(read16() != 1) { // # planes -- must be '1'
			debug_e("Un-supported BMP planes");
			break;
		}

		uint8_t bmpDepth = read16(); // bits per pixel
		debug_i("Bit Depth: %d", bmpDepth);
		if((bmpDepth != 24) || (read32() != 0)) { // 0 = uncompressed
			debug_e("Un-supported BMP depth");
			break;
		}

		// Supported BMP format -- proceed!
		goodBmp = true;

		debug_i("Image size: %d x %d", bmpWidth, bmpHeight);

		// BMP rows are padded (if needed) to 4-byte boundary
		uint32_t rowSize = (bmpWidth * 3 + 3) & ~3;

		// If bmpHeight is negative, image is in top-down order.
		// This is not canon but has been observed in the wild.
		bool flip = true; // BMP is stored bottom-to-top
		if(bmpHeight < 0) {
			bmpHeight = -bmpHeight;
			flip = false;
		}

		// Crop area to be loaded
		int w = bmpWidth;
		int h = bmpHeight;
		if((x + w - 1) >= tft.width()) {
			w = tft.width() - x;
		}
		if((y + h - 1) >= tft.height()) {
			h = tft.height() - y;
		}

		// Set TFT address window to clipped image bounds
		tft.startWrite();
		tft.setAddrWindow(x, y, w, h);

		constexpr size_t BUFFPIXEL{20};
		struct Buffer {
			union {
				uint8_t rgb[BUFFPIXEL][3];  // pixel buffer (R+G+B per pixel)
				uint16_t colors[BUFFPIXEL]; // Resulting colours
			};
			uint8_t index{0};

			// Convert rgb values into RGB565 format
			static uint16_t color565(uint8_t r, uint8_t g, uint8_t b)
			{
				return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
			}

			bool convertPixel()
			{
				uint16_t color = color565(rgb[index][2], rgb[index][1], rgb[index][0]);
				colors[index] = __builtin_bswap16(color);
				++index;
				return index == BUFFPIXEL;
			}
		};
		Buffer buf;
		bool reload{true};

		auto flushPixels = [&]() {
			if(buf.index == 0) {
				return;
			}
			tft.writePixels(buf.colors, buf.index, true, true);
			buf.index = 0;
		};

		uint32_t filePos = file.tell();
		for(int row = 0; row < h; row++) { // For each scanline...
			/*
			 * Seek to start of scan line.  It might seem inefficient to be doing this on
			 * every line, but this method covers a lot of gritty details like cropping
			 * and scanline padding.  Also, the seek only takes place if the file position
			 * actually needs to change (avoids a lot of cluster math in SD library).
			 */
			uint32_t pos;
			if(flip) {
				// Bitmap is stored bottom-to-top order (normal BMP)
				pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
			} else {
				// Bitmap is stored top-to-bottom
				pos = bmpImageoffset + row * rowSize;
			}
			if(filePos != pos) {
				filePos = file.seek(pos, SeekOrigin::Start);
				reload = true;
			}
			for(int col = 0; col < w; ++col) {
				// Time to read more pixel data?
				if(reload) {
					flushPixels();
					filePos += file.read(buf.rgb, sizeof(buf.rgb));
					reload = false;
				}
				reload = buf.convertPixel();
			}
		}
		flushPixels();
		tft.endWrite();

		debug_i("Loaded in %d ms", millis() - startTime);
		break;
	}

	if(!goodBmp) {
		debug_e("BMP format not recognized.");
	}

	return goodBmp;
}
