#ifndef NAELSTROF_TEXT
#define NAELSTROF_TEXT

#include <vector>
#include <string>
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include "NTexture.h"
#include "NShaderManager.h"

namespace NText
{
	static GLuint ProgramID;
	static GLuint UniformID;
	static GLuint ColorID;
	static GLuint XOffID;
	static GLuint YOffID;
	static GLuint WidthSize;
	static GLuint HeightSize;
	static bool ProgramLoaded;

	static int ScreenHeight = 0;
	static int ScreenWidth = 0;

	void SetScreen(int, int);

	class Text
	{
	private:
		float Size;
		NTexture::Texture Font;
		float Position[2];
		float Color[4];
		short VertexCount;
		GLuint VertexBuffer;
		GLuint UVBuffer;
	public:
		float Width;
		Text();
		Text(void*);
		Text(const char*,const char*,float,float,float,float,float,float,float);
		~Text();
		int SetText(const char*);
		int SetPos(float,float);
		int SetSize(float);
		int Draw();
	};
}
#endif
