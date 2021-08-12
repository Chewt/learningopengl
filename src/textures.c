#include "textures.h"

unsigned int createTexture(const char* file, unsigned int texture_number)
{
    stbi_set_flip_vertically_on_load(1);
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);

    /* Create Texture object */
    unsigned int texture;
    glGenTextures(1, &texture);
    glActiveTexture(texture_number);
    glBindTexture(GL_TEXTURE_2D, texture);
    unsigned int format = GL_RGB;
    if (nrChannels == 4)
        format = GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
            GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    return texture;
}
