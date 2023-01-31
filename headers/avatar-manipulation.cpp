#include "image.hpp"
#include "image-processing.cpp"
#include "image.hpp"
#include "stb_image_write.h"
#include "stb_image.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <iostream>
#include <cmath>
#include <algorithm>

class Avatar {
    private:
        std::string exec(std::string command) {
        char buffer[128];
        std::string result = "";
        FILE* pipe = popen(command.c_str(), "r");
        if (!pipe) return "popen failed!";
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != NULL) result += buffer;
        }
        pclose(pipe);
        return result;
        }
    public:
        void run(std::string link1, std::string link2, char* middle) {
            exec("rm ./media/avatar.png");
            exec("rm ./media/avatar2.png");
            exec(std::string("wget -nc -q -O ./media/avatar.png ") + std::string(link1));
            exec(std::string("wget -nc -q -O ./media/avatar2.png ") + std::string(link2));
            
            char* possible[] = {(char*)"./media/heart.png", (char*)"./media/sparkling-heart.png"};
            std::string stringmiddle = middle;
            if (stringmiddle == "heart") {
                middle = possible[std::rand() % 2];
            }

            Image heart(middle);
            Image avatar("./media/avatar.png");
            Image avatar2("./media/avatar2.png");
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
            blank.write("result.png");
        }
};
