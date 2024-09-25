#include "AFK_Shape.h"
#include "AFK_Image.h"

unsigned int Image::iLoadImage(const char filename[])
{
    int width, height, bpp;

    unsigned int texture;

    unsigned char* data(0);
    data = stbi_load(filename, &width, &height, &bpp, 4);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D,
        0,
        GL_RGBA,
        width, height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        data);

    stbi_image_free(data);

    return texture;
}