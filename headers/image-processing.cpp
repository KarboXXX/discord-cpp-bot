#include <cstring>
#include <iostream>
#include <complex>
#include <cmath>
#include "image.hpp"
#include "./stb/stb_image_write.h"
#include "./stb/stb_image.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define BYTE_BOUND(value) value < 0 ? 0 : (value > 255 ? 255 : value)

Image::Image(const char* filename) {
    if (read(filename)) {
        // printf("read: %s", filename);
        size = w*h*channels;
    } else {
        printf("fail reading: %s", filename);
    }
}
Image::Image(int w, int h, int channels) : w(w), h(h), channels(channels) {
    size = w*h*channels;
    data = new uint8_t[size];
}
Image::Image(const Image& img) : Image(img.w, img.h, img.channels){
    memcpy(data, img.data, size);
}
Image::~Image() {
    stbi_image_free(data);
}

bool Image::read(const char* filename) {
    data = stbi_load(filename, &w, &h, &channels, 0);
    return data != NULL;
}
bool Image::write(const char* filename) {
    ImageType type = getFileType(filename);
    int success;
    switch(type) {
        case PNG:
            success = stbi_write_png(filename, w, h, channels, data,  w*channels);
            break;
        case JPG:
            success = stbi_write_jpg(filename, w, h, channels, data,  100);
            break;
        case TGA:
            success = stbi_write_tga(filename, w, h, channels, data);
            break;
        case BMP:
            success = stbi_write_bmp(filename, w, h, channels, data);
            break;
    }

    return success != 0;
}

ImageType Image::getFileType(const char* filename) {
    const char* ext = strrchr(filename, '.');
    if (ext != nullptr) {
        if (strcmp(ext, ".png") == 0) return PNG;
        if (strcmp(ext, ".jpg") == 0) return JPG;
        if (strcmp(ext, ".tga") == 0) return TGA;
        if (strcmp(ext, ".bmp") == 0) return BMP;
    }
}

Image& Image::crop(uint16_t cx, uint16_t cy, uint16_t cw, uint16_t ch) {
    size = cw * ch * channels;
	uint8_t* croppedImage = new uint8_t[size];
	memset(croppedImage, 0, size);

	for(uint16_t y = 0;y < ch;++y) {
		if(y + cy >= h) {break;}
		for(uint16_t x = 0;x < cw;++x) {
			if(x + cx >= w) {break;}
			memcpy(&croppedImage[(x + y * cw) * channels], &data[(x + cx + (y + cy) * w) * channels], channels);
		}
	}

	w = cw;
	h = ch;
	

	delete[] data;
	data = croppedImage;
	croppedImage = nullptr;

	return *this;
}

Image& Image::overlay(const Image& source, int x, int y) {
	uint8_t* srcPx;
	uint8_t* dstPx;

	for(int sy = 0;sy < source.h;++sy) {
		if(sy + y < 0) {continue;}
		else if(sy + y >= h) {break;}
		for(int sx = 0;sx < source.w;++sx) {
			if(sx + x < 0) {continue;}
			else if(sx + x >= w) {break;}
			srcPx = &source.data[(sx + sy * source.w) * source.channels];
			dstPx = &data[(sx + x + (sy + y) * w) * channels];

			float srcAlpha = source.channels < 4 ? 1 : srcPx[3] / 255.f;
			float dstAlpha = channels < 4 ? 1 : dstPx[3] / 255.f;

			if(srcAlpha > .99 && dstAlpha > .99) {
				if(source.channels >= channels) {
					memcpy(dstPx, srcPx, channels);
				}
				else memset(dstPx, srcPx[0], channels);
			}
			else {
				float outAlpha = srcAlpha + dstAlpha * (1 - srcAlpha);
				if(outAlpha < .01) {
					memset(dstPx, 0, channels);
				}
				else {
					for(int chnl = 0;chnl < channels;++chnl) {
						dstPx[chnl] = (uint8_t)BYTE_BOUND((srcPx[chnl]/255.f * srcAlpha + dstPx[chnl]/255.f * dstAlpha * (1 - srcAlpha)) / outAlpha * 255.f);
					}
					if(channels > 3) { dstPx[3] = (uint8_t)BYTE_BOUND(outAlpha * 255.f); } 
				}
			}
		}

	}
	return *this;
}

Image& Image::resizeNN(uint16_t nw, uint16_t nh) {
	size = nw * nh * channels;
	uint8_t* newImage = new uint8_t[size];

	float scaleX = (float)nw / (w);
	float scaleY = (float)nh / (h);
	uint16_t sx, sy;

	for(uint16_t y = 0;y < nh;++y) {
		sy = (uint16_t)(y / scaleY);
		for(uint16_t x = 0;x < nw;++x) {
			sx = (uint16_t)(x / scaleX);

			memcpy(&newImage[(x + y * nw) * channels], &data[(sx + sy * w) * channels], channels);

		}
	}


	w = nw;
	h = nh;
	delete[] data;
	data = newImage;
	newImage = nullptr;

	return *this;
}
