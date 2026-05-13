#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "resource_handler.hpp"

namespace SpaceGL
{
    ResourceHandler::Image ResourceHandler::loadImage(const char* path)
    {
        int width, height, nrChannels;
        unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 4);

        if (data) return Image {width, height, nrChannels, data};

        std::cerr << "Image data not loaded." << std::endl;
        return Image{-1, -1, -1};
    }

    void ResourceHandler::freeImage(ResourceHandler::Image image)
    {
        stbi_image_free(image.data);
    }
}