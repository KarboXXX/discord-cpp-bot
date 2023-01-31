#include "image.hpp"
#include "image-processing.cpp"
#include "image.hpp"
#include "./stb/stb_image_write.h"
#include "./stb/stb_image.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <iostream>
#include <cmath>
#include <algorithm>
#include <curl/curl.h>
#include <stdio.h>


class Avatar {
    private:
        static size_t callbackfunction(void *ptr, size_t size, size_t nmemb, void* userdata) {
            FILE* stream = (FILE*)userdata;
            if (!stream) {
                printf("no stream");
                return 0;
            }
            size_t written = fwrite((FILE*)ptr, size, nmemb, stream);
            return written;
        }
        bool download_jpeg(const char* url, const char* outfilename) {
            FILE* fp = fopen(outfilename, "wb");
            if (!fp) {
                printf("failed to create file on the disk");
                return false;
            }

            CURL* curlCtx = curl_easy_init();
            curl_easy_setopt(curlCtx, CURLOPT_URL, url);
            curl_easy_setopt(curlCtx, CURLOPT_WRITEDATA, fp);
            curl_easy_setopt(curlCtx, CURLOPT_WRITEFUNCTION, callbackfunction);
            curl_easy_setopt(curlCtx, CURLOPT_FOLLOWLOCATION, 1);

            CURLcode rc = curl_easy_perform(curlCtx);
            if (rc) {
                printf("failed to download: %s\n", url);
                return false;
            }

            long res_code = 0;
            curl_easy_getinfo(curlCtx, CURLINFO_RESPONSE_CODE, &res_code);
            if (!((res_code == 200 || res_code == 201) && rc != CURLE_ABORTED_BY_CALLBACK)) {
                printf("response code: %ld\n", res_code);
                return false;
            }

            curl_easy_cleanup(curlCtx);
            fclose(fp);
            return true;
        }
    public:
        std::string run(std::string link1, std::string link2, char* middle) {
            const std::string resultname = std::to_string(std::rand()) + ".png";

            std::string avatar1filename = link1.substr(link2.length() - 14);
            std::string avatar2filename = link2.substr(link2.length() - 14);
            std::replace(avatar1filename.begin(), avatar1filename.end(), '/', '0');
            std::replace(avatar2filename.begin(), avatar2filename.end(), '/', '0');

            //std::cout << avatar1filename << std::endl;
            //std::cout << avatar2filename << std::endl;

            download_jpeg(link1.c_str(), avatar1filename.c_str());
            download_jpeg(link2.c_str(), avatar2filename.c_str());

            char* possible[] = {(char*)"./media/heart.png", (char*)"./media/sparkling-heart.png"};
            std::string stringmiddle = middle;
            if (stringmiddle == "heart") {
                middle = possible[std::rand() % 2];
            }

            Image heart(middle);
            Image avatar(avatar1filename.c_str());
            Image avatar2(avatar2filename.c_str());
            Image copy = heart;
            Image blank(900*2, 900, 3);
            for (int i=0; i<blank.size; i++) {
                blank.data[i] = 0;
            }
            heart = heart.resizeNN(round(heart.w*3), round(heart.h*3));
            avatar = avatar.resizeNN(round(avatar.w*3), round(avatar.h*3));
            avatar2 = avatar2.resizeNN(round(avatar2.w*3), round(avatar2.h*3));
            blank.overlay(heart, round((blank.w-heart.w)/2), round((blank.h-heart.h)/2));
            blank.overlay(avatar, 70, round((blank.h-avatar.h)/2));
            blank.overlay(avatar2, round(blank.w-avatar2.w-70), round((blank.h-avatar2.h)/2));

            blank.write(resultname.c_str());
            remove(avatar1filename.c_str());
            remove(avatar2filename.c_str());
            return resultname;
        }
};
