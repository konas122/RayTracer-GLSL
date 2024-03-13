#ifndef TEXTURE_H
#define TEXTURE_H


class Texture {
public:
    Texture(unsigned int type);
    virtual ~Texture();

    void Bind(unsigned int texStage);
    void Unbind();

protected:
	unsigned int handle;

	unsigned int type;
	unsigned int format;
	unsigned int pixelFormat;
};


class Texture2D : public Texture {
public:
    Texture2D();
    ~Texture2D();

    bool Create(char const *path);
    bool Create(unsigned int width, unsigned int height, unsigned int nrComponents, bool isHDR, void *data);
    void Destroy();
};


#endif