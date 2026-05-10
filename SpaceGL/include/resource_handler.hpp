#pragma once

namespace SpaceGL
{
    class ResourceHandler
    {
    public:
        struct Image { int width; int height; int nChannels; unsigned char* data; };

        Image loadImage(const char* path);
        void freeImage(Image image);
    };
}