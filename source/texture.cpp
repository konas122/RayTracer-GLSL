#include <glad/glad.h>

#include "stb_image.h"
#include "texture.h"


Texture::Texture(unsigned int type)
	: handle(0),
    type(type),
    format(GL_RGBA),
    pixelFormat(GL_UNSIGNED_BYTE) {}

Texture::~Texture() {}


void Texture::Bind(unsigned int texStage) {
    if (handle) {
        glActiveTexture(GL_TEXTURE0 + texStage);
        glBindTexture(type, handle);
    }
}


void Texture::Unbind() {
    glBindTexture(type, 0);
}



Texture2D::Texture2D() : Texture(GL_TEXTURE_2D) {}

Texture2D::~Texture2D() {}


bool Texture2D::Create(char const *path) {
    bool isHDR = stbi_is_hdr(path);

    int width, height, nrComponents;
	void* data = nullptr;

    if(isHDR)
		data = stbi_loadf(path, &width, &height, &nrComponents, 0);
	else
		data = stbi_load(path, &width, &height, &nrComponents, 0);
    
    if (data) {
		bool result = Create(width, height, nrComponents, isHDR, data);
		stbi_image_free(data);
		return result;
	}
	else {
		return false;
	}
}

bool Texture2D::Create(unsigned int width, unsigned int height, unsigned int nrComponents, bool isHDR, void *data) {
    if (nrComponents == 1)
		format = GL_RED;
	else if (nrComponents == 3)
		format = GL_RGB;
	else if (nrComponents == 4)
		format = GL_RGBA;
    
    if (isHDR)
		pixelFormat = GL_FLOAT;
	else
		pixelFormat = GL_UNSIGNED_BYTE;

    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);
    glTexImage2D(GL_TEXTURE_2D, 0, (GLint)format, width, height, 0, (GLint)format, (GLint)pixelFormat, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return true;
}


void Texture2D::Destroy() {
    if (handle) {
        glDeleteTextures(1, &handle);
        handle = 0;
    }
}
