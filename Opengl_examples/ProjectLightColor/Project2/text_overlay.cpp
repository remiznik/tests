
#define _CRT_SECURE_NO_WARNINGS
#include "text_overlay.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

struct header
{
	unsigned char       identifier[12];
	unsigned int        endianness;
	unsigned int        gltype;
	unsigned int        gltypesize;
	unsigned int        glformat;
	unsigned int        glinternalformat;
	unsigned int        glbaseinternalformat;
	unsigned int        pixelwidth;
	unsigned int        pixelheight;
	unsigned int        pixeldepth;
	unsigned int        arrayelements;
	unsigned int        faces;
	unsigned int        miplevels;
	unsigned int        keypairbytes;
};

static const unsigned char identifier[] =
{
	0xAB, 0x4B, 0x54, 0x58, 0x20, 0x31, 0x31, 0xBB, 0x0D, 0x0A, 0x1A, 0x0A
};

static unsigned int calculate_stride(const header& h, unsigned int width, unsigned int pad = 4)
{
	unsigned int channels = 0;

	switch (h.glbaseinternalformat)
	{
	case GL_RED:    channels = 1;
		break;
	case GL_RG:     channels = 2;
		break;
	case GL_BGR:
	case GL_RGB:    channels = 3;
		break;
	case GL_BGRA:
	case GL_RGBA:   channels = 4;
		break;
	}

	unsigned int stride = h.gltypesize * channels * width;

	stride = (stride + (pad - 1)) & ~(pad - 1);

	return stride;
}

static const unsigned int swap32(const unsigned int u32)
{
	union
	{
		unsigned int u32;
		unsigned char u8[4];
	} a, b;

	a.u32 = u32;
	b.u8[0] = a.u8[3];
	b.u8[1] = a.u8[2];
	b.u8[2] = a.u8[1];
	b.u8[3] = a.u8[0];

	return b.u32;
}

static unsigned int calculate_face_size(const header& h)
{
	unsigned int stride = calculate_stride(h, h.pixelwidth);

	return stride * h.pixelheight;
}


unsigned int load(const char * filename, unsigned int tex)
{
	FILE * fp;
	GLuint temp = 0;
	GLuint retval = 0;
	header h;
	size_t data_start, data_end;
	unsigned char * data;
	GLenum target = GL_NONE;

	fp = fopen(filename, "rb");

	if (!fp)
		return 0;

	if (fread(&h, sizeof(h), 1, fp) != 1)
		goto fail_read;

	if (memcmp(h.identifier, identifier, sizeof(identifier)) != 0)
		goto fail_header;

	if (h.endianness == 0x04030201)
	{
		// No swap needed
	}
	else if (h.endianness == 0x01020304)
	{
		// Swap needed
		h.endianness = swap32(h.endianness);
		h.gltype = swap32(h.gltype);
		h.gltypesize = swap32(h.gltypesize);
		h.glformat = swap32(h.glformat);
		h.glinternalformat = swap32(h.glinternalformat);
		h.glbaseinternalformat = swap32(h.glbaseinternalformat);
		h.pixelwidth = swap32(h.pixelwidth);
		h.pixelheight = swap32(h.pixelheight);
		h.pixeldepth = swap32(h.pixeldepth);
		h.arrayelements = swap32(h.arrayelements);
		h.faces = swap32(h.faces);
		h.miplevels = swap32(h.miplevels);
		h.keypairbytes = swap32(h.keypairbytes);
	}
	else
	{
		goto fail_header;
	}

	// Guess target (texture type)
	if (h.pixelheight == 0)
	{
		if (h.arrayelements == 0)
		{
			target = GL_TEXTURE_1D;
		}
		else
		{
			target = GL_TEXTURE_1D_ARRAY;
		}
	}
	else if (h.pixeldepth == 0)
	{
		if (h.arrayelements == 0)
		{
			if (h.faces == 0)
			{
				target = GL_TEXTURE_2D;
			}
			else
			{
				target = GL_TEXTURE_CUBE_MAP;
			}
		}
		else
		{
			if (h.faces == 0)
			{
				target = GL_TEXTURE_2D_ARRAY;
			}
			else
			{
				target = GL_TEXTURE_CUBE_MAP_ARRAY;
			}
		}
	}
	else
	{
		target = GL_TEXTURE_3D;
	}

	// Check for insanity...
	if (target == GL_NONE ||                                    // Couldn't figure out target
		(h.pixelwidth == 0) ||                                  // Texture has no width???
		(h.pixelheight == 0 && h.pixeldepth != 0))              // Texture has depth but no height???
	{
		goto fail_header;
	}

	temp = tex;
	if (tex == 0)
	{
		glGenTextures(1, &tex);
	}

	glBindTexture(target, tex);

	data_start = ftell(fp) + h.keypairbytes;
	fseek(fp, 0, SEEK_END);
	data_end = ftell(fp);
	fseek(fp, data_start, SEEK_SET);

	data = new unsigned char[data_end - data_start];
	memset(data, 0, data_end - data_start);

	fread(data, 1, data_end - data_start, fp);

	if (h.miplevels == 0)
	{
		h.miplevels = 1;
	}

	switch (target)
	{
	case GL_TEXTURE_1D:
		glTexStorage1D(GL_TEXTURE_1D, h.miplevels, h.glinternalformat, h.pixelwidth);
		glTexSubImage1D(GL_TEXTURE_1D, 0, 0, h.pixelwidth, h.glformat, h.glinternalformat, data);
		break;
	case GL_TEXTURE_2D:
		// glTexImage2D(GL_TEXTURE_2D, 0, h.glinternalformat, h.pixelwidth, h.pixelheight, 0, h.glformat, h.gltype, data);
		if (h.gltype == GL_NONE)
		{
			glCompressedTexImage2D(GL_TEXTURE_2D, 0, h.glinternalformat, h.pixelwidth, h.pixelheight, 0, 420 * 380 / 2, data);
		}
		else
		{
			glTexStorage2D(GL_TEXTURE_2D, h.miplevels, h.glinternalformat, h.pixelwidth, h.pixelheight);
			{
				unsigned char * ptr = data;
				unsigned int height = h.pixelheight;
				unsigned int width = h.pixelwidth;
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				for (unsigned int i = 0; i < h.miplevels; i++)
				{
					glTexSubImage2D(GL_TEXTURE_2D, i, 0, 0, width, height, h.glformat, h.gltype, ptr);
					ptr += height * calculate_stride(h, width, 1);
					height >>= 1;
					width >>= 1;
					if (!height)
						height = 1;
					if (!width)
						width = 1;
				}
			}
		}
		break;
	case GL_TEXTURE_3D:
		glTexStorage3D(GL_TEXTURE_3D, h.miplevels, h.glinternalformat, h.pixelwidth, h.pixelheight, h.pixeldepth);
		glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, h.pixelwidth, h.pixelheight, h.pixeldepth, h.glformat, h.gltype, data);
		break;
	case GL_TEXTURE_1D_ARRAY:
		glTexStorage2D(GL_TEXTURE_1D_ARRAY, h.miplevels, h.glinternalformat, h.pixelwidth, h.arrayelements);
		glTexSubImage2D(GL_TEXTURE_1D_ARRAY, 0, 0, 0, h.pixelwidth, h.arrayelements, h.glformat, h.gltype, data);
		break;
	case GL_TEXTURE_2D_ARRAY:
		glTexStorage3D(GL_TEXTURE_2D_ARRAY, h.miplevels, h.glinternalformat, h.pixelwidth, h.pixelheight, h.arrayelements);
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, h.pixelwidth, h.pixelheight, h.arrayelements, h.glformat, h.gltype, data);
		break;
	case GL_TEXTURE_CUBE_MAP:
		glTexStorage2D(GL_TEXTURE_CUBE_MAP, h.miplevels, h.glinternalformat, h.pixelwidth, h.pixelheight);
		// glTexSubImage3D(GL_TEXTURE_CUBE_MAP, 0, 0, 0, 0, h.pixelwidth, h.pixelheight, h.faces, h.glformat, h.gltype, data);
		{
			unsigned int face_size = calculate_face_size(h);
			for (unsigned int i = 0; i < h.faces; i++)
			{
				glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, h.pixelwidth, h.pixelheight, h.glformat, h.gltype, data + face_size * i);
			}
		}
		break;
	case GL_TEXTURE_CUBE_MAP_ARRAY:
		glTexStorage3D(GL_TEXTURE_CUBE_MAP_ARRAY, h.miplevels, h.glinternalformat, h.pixelwidth, h.pixelheight, h.arrayelements);
		glTexSubImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 0, 0, 0, 0, h.pixelwidth, h.pixelheight, h.faces * h.arrayelements, h.glformat, h.gltype, data);
		break;
	default:                                               // Should never happen
		goto fail_target;
	}

	if (h.miplevels == 1)
	{
		glGenerateMipmap(target);
	}

	retval = tex;

fail_target:
	delete[] data;

fail_header:;
fail_read:;
	fclose(fp);

	return retval;
}


void text_overlay::init(int width, int height, const char* font)
{
	GLuint vs;
	GLuint fs;

	buffer_width = width;
	buffer_height = height;

	vs = glCreateShader(GL_VERTEX_SHADER);
	fs = glCreateShader(GL_FRAGMENT_SHADER);

	static const char * vs_source[] =
	{
		"#version 440 core\n"
		"void main(void)\n"
		"{\n"
		"    gl_Position = vec4(float((gl_VertexID >> 1) & 1) * 2.0 - 1.0,\n"
		"                       float((gl_VertexID & 1)) * 2.0 - 1.0,\n"
		"                       0.0, 1.0);\n"
		"}\n"
	};

	static const char * fs_source[] =
	{
		"#version 440 core\n"
		"layout (origin_upper_left) in vec4 gl_FragCoord;\n"
		"layout (location = 0) out vec4 o_color;\n"
		"layout (binding = 0) uniform isampler2D text_buffer;\n"
		"layout (binding = 1) uniform isampler2DArray font_texture;\n"
		"void main(void)\n"
		"{\n"
		"    ivec2 frag_coord = ivec2(gl_FragCoord.xy);\n"
		"    ivec2 char_size = textureSize(font_texture, 0).xy;\n"
		"    ivec2 char_location = frag_coord / char_size;\n"
		"    ivec2 texel_coord = frag_coord % char_size;\n"
		"    int character = texelFetch(text_buffer, char_location, 0).x;\n"
		"    float val = texelFetch(font_texture, ivec3(texel_coord, character), 0).x;\n"
		"    if (val == 0.0)\n"
		"        discard;\n"
		"    o_color = vec4(1.0);\n"
		"}\n"
	};

	glShaderSource(vs, 1, vs_source, nullptr);
	glCompileShader(vs);

	glShaderSource(fs, 1, fs_source, nullptr);
	glCompileShader(fs);

	text_program = glCreateProgram();
	glAttachShader(text_program, vs);
	glAttachShader(text_program, fs);
	glLinkProgram(text_program);

	glDeleteShader(fs);
	glDeleteShader(vs);

	// glCreateVertexArrays(1, &vao);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// glCreateTextures(GL_TEXTURE_2D, 1, &text_buffer);
	glGenTextures(1, &text_buffer);
	glBindTexture(GL_TEXTURE_2D, text_buffer);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_R8UI, width, height);

	font_texture = load(font ? font : "cp437_9x16.ktx", 0);

	screen_buffer = new char[width * height];
	memset(screen_buffer, 0, width * height);
}

void text_overlay::teardown()
{
	delete[] screen_buffer;
	glDeleteTextures(1, &font_texture);
	glDeleteTextures(1, &text_buffer);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(text_program);
}

void text_overlay::draw()
{
	glUseProgram(text_program);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, text_buffer);
	if (dirty)
	{
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, buffer_width, buffer_height, GL_RED_INTEGER, GL_UNSIGNED_BYTE, screen_buffer);
		dirty = false;
	}
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D_ARRAY, font_texture);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void text_overlay::drawText(const char* str, int x, int y)
{
	char* dst = screen_buffer + y * buffer_width + x;
	strcpy(dst, str);
	dirty = true;
}

void text_overlay::scroll(int lines)
{
	const char* src = screen_buffer + lines * buffer_width;
	char * dst = screen_buffer;

	memmove(dst, src, (buffer_height - lines) * buffer_width);

	dirty = true;
}

void text_overlay::print(const char* str)
{
	const char* p = str;
	char c;
	char* dst = screen_buffer + cursor_y * buffer_width + cursor_x;

	while (*p != 0)
	{
		c = *p++;
		if (c == '\n')
		{
			cursor_y++;
			cursor_x = 0;
			if (cursor_y >= buffer_height)
			{
				cursor_y--;
				scroll(1);
			}
			dst = screen_buffer + cursor_y * buffer_width + cursor_x;
		}
		else
		{
			*dst++ = c;
			cursor_x++;
			if (cursor_x >= buffer_width)
			{
				cursor_y++;
				cursor_x = 0;
				if (cursor_y >= buffer_height)
				{
					cursor_y--;
					scroll(1);
				}
				dst = screen_buffer + cursor_y * buffer_width + cursor_x;
			}
		}
	}

	dirty = true;
}

void text_overlay::moveCursor(int x, int y)
{
	cursor_x = x;
	cursor_y = y;
}

void text_overlay::clear()
{
	memset(screen_buffer, 0, buffer_width * buffer_height);
	dirty = true;
	cursor_x = 0;
	cursor_y = 0;
}

