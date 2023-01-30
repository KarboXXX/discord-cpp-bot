#pragma once
#include <cstdio>
#include <stdint.h>

enum ImageType {
    PNG, JPG, BMP, TGA
};

struct Image {
    uint8_t* data = NULL;
    size_t size = 0;
    int channels;
    int w;
    int h;

    Image(const char* filename);
    Image(int w, int h, int channels);
    Image(const Image& img);
    ~Image();

    bool read(const char* filename);
    bool write(const char* filename);

    ImageType getFileType(const char* filename);

    Image& crop(uint16_t cx, uint16_t cy, uint16_t cw, uint16_t ch);
    Image& overlay(const Image& source, int x, int y);
    Image& resizeNN(uint16_t nw, uint16_t nh);
};