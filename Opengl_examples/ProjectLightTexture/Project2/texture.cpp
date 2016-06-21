#include "texture.h"

// FreeImage
#include <FreeImage.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


unsigned char * loadTexture(const char *filename, int* w, int* h)
{
	FIBITMAP *dib1 = NULL;

	FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(filename);

	dib1 = FreeImage_Load(fif, filename, JPEG_DEFAULT);
	if (!dib1)
	{
		std::cerr << "Erreur ouverture d\'image" << std::endl;
		exit(0);

	}

	int x, y;
	int height, width;

	RGBQUAD rgbquad;

	FREE_IMAGE_TYPE type;
	BITMAPINFOHEADER *header;

	type = FreeImage_GetImageType(dib1);

	*h = height = FreeImage_GetHeight(dib1);
	*w = width = FreeImage_GetWidth(dib1);

	header = FreeImage_GetInfoHeader(dib1);
	int scanLineWidh = ((3 * width) % 4 == 0) ? 3 * width : ((3 * width) / 4) * 4 + 4;
	unsigned char * texels = (GLubyte*)calloc(height*scanLineWidh, sizeof(GLubyte));
	for (x = 0; x < width; x++)
	{
		for (y = 0; y < height; y++)
		{
			FreeImage_GetPixelColor(dib1, x, y, &rgbquad);

			texels[(y*scanLineWidh + 3 * x)] = ((GLubyte*)&rgbquad)[2];
			texels[(y*scanLineWidh + 3 * x) + 1] = ((GLubyte*)&rgbquad)[1];
			texels[(y*scanLineWidh + 3 * x) + 2] = ((GLubyte*)&rgbquad)[0];
		}
	}

	FreeImage_Unload(dib1);

	return texels;
}


Texture::Texture(const std::string& name)
{
	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
										   // Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	// Load image, create texture and generate mipmaps
	int width, height;
	//unsigned char* image = SOIL_load_image("container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	//unsigned char* image = loadTexture("images/awesomeface.png");
	unsigned char* image = loadTexture(name.c_str(), &width, &height);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	free(image);
	//SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

}


Texture::~Texture()
{
}
