#include "NText.h"

void NText::SetScreen(int Width, int Height)
{
	ScreenHeight = Height/2;
	ScreenWidth = Width/2;
	if (!ProgramLoaded)
	{
		ProgramID = LoadShaders("shaders/Text.vert", "shaders/Text.frag", NULL);
		ColorID = glGetUniformLocation(ProgramID, "Color");
		XOffID = glGetUniformLocation(ProgramID, "XOff");
		YOffID = glGetUniformLocation(ProgramID, "YOff");
		UniformID = glGetUniformLocation(ProgramID, "TextureSampler");
		WidthSize = glGetUniformLocation(ProgramID, "ScreenWidth");
		HeightSize = glGetUniformLocation(ProgramID, "ScreenHeight");
		ProgramLoaded = true;
	}
	glUseProgram(ProgramID);
	glUniform1f(WidthSize,float(ScreenWidth));
	glUniform1f(HeightSize,float(ScreenHeight));
	glUseProgram(0);
}
	
NText::Text::Text(const char* DisplayText, const char* Texture, float Siz, float x, float y, float r, float g, float b, float a)
{
	if (!ProgramLoaded)
	{
		ProgramID = LoadShaders("shaders/Text.vert", "shaders/Text.frag", NULL);
		ColorID = glGetUniformLocation(ProgramID, "Color");
		XOffID = glGetUniformLocation(ProgramID, "XOff");
		YOffID = glGetUniformLocation(ProgramID, "YOff");
		UniformID = glGetUniformLocation(ProgramID, "TextureSampler");
		WidthSize = glGetUniformLocation(ProgramID, "ScreenWidth");
		HeightSize = glGetUniformLocation(ProgramID, "ScreenHeight");
		ProgramLoaded = true;
	}
	Size = Siz;
	Font = NTexture::Texture(Texture);
	unsigned int StringLength = strlen(DisplayText);
	Width = StringLength*Size;
	Position[0] = x;
	Position[1] = y;
	Color[0] = r;
	Color[1] = g;
	Color[2] = b;
	Color[3] = a;
	VertexCount = 0;
	std::vector<float> Vertex;
	std::vector<float> UV;
	for (unsigned int i=0;i<StringLength;i++)
	{
		float top_leftx=i*Size;
		float top_lefty=Size;
		float top_rightx=(1+i)*Size;
		float top_righty=Size;
		float bottom_leftx=i*Size;
		float bottom_lefty=0;
		float bottom_rightx=(1+i)*Size;
		float bottom_righty=0;
		
		float tilesize = 1.f/16.f;
		float xoff = float((DisplayText[i]-1)%16)/16.f;
		float yoff = 1.f-(float((DisplayText[i]-1)/16)/16.f);
		yoff -= tilesize;
		
		float uv_bottom_leftx = xoff;
		float uv_bottom_lefty = yoff;
		float uv_bottom_rightx=xoff+tilesize;
		float uv_bottom_righty=yoff;
		float uv_top_leftx=xoff;
		float uv_top_lefty=yoff+tilesize;
		float uv_top_rightx=xoff+tilesize;
		float uv_top_righty=yoff+tilesize;
		
		Vertex.push_back(top_leftx);
		Vertex.push_back(top_lefty);
		UV.push_back(uv_top_leftx);
		UV.push_back(uv_top_lefty);
		Vertex.push_back(bottom_leftx);
		Vertex.push_back(bottom_lefty);
		UV.push_back(uv_bottom_leftx);
		UV.push_back(uv_bottom_lefty);
		Vertex.push_back(top_rightx);
		Vertex.push_back(top_righty);
		UV.push_back(uv_top_rightx);
		UV.push_back(uv_top_righty);
		
		Vertex.push_back(bottom_rightx);
		Vertex.push_back(bottom_righty);
		UV.push_back(uv_bottom_rightx);
		UV.push_back(uv_bottom_righty);
		Vertex.push_back(top_rightx);
		Vertex.push_back(top_righty);
		UV.push_back(uv_top_rightx);
		UV.push_back(uv_top_righty);
		Vertex.push_back(bottom_leftx);
		Vertex.push_back(bottom_lefty);
		UV.push_back(uv_bottom_leftx);
		UV.push_back(uv_bottom_lefty);
		VertexCount+=6;
	}
	glGenBuffers(1,&VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, Vertex.size() * sizeof(float), &Vertex[0], GL_STATIC_DRAW);
	
	glGenBuffers(1,&UVBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
	glBufferData(GL_ARRAY_BUFFER, UV.size() * sizeof(float), &UV[0], GL_STATIC_DRAW);
}

int NText::Text::Draw()
{
	glUseProgram(ProgramID);
	
	glActiveTexture(GL_TEXTURE0);
	Font.Apply();
	glEnable(GL_TEXTURE_2D);
	glUniform1i(UniformID, 0);
	
	glUniform1f(XOffID,Position[0]);
	glUniform1f(YOffID,Position[1]);
	glUniform4f(ColorID,Color[0],Color[1],Color[2],Color[3]);
	
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,VertexBuffer);
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,(void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,(void*)0);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glDrawArrays(GL_TRIANGLES,0,VertexCount);
	
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glUseProgram(0);
	return 0;
}

int NText::Text::SetPos(float x, float y)
{
	Position[0] = x;
	Position[1] = y;
	return 0;
}

NText::Text::Text()
{
	//do nothing, can be initialized with SetText
}

int NText::Text::SetText(const char* DisplayText)
{
	if (!ProgramLoaded)
	{
		ProgramID = LoadShaders("shaders/Text.vert", "shaders/Text.frag", NULL);
		ColorID = glGetUniformLocation(ProgramID, "Color");
		XOffID = glGetUniformLocation(ProgramID, "XOff");
		YOffID = glGetUniformLocation(ProgramID, "YOff");
		UniformID = glGetUniformLocation(ProgramID, "TextureSampler");
		WidthSize = glGetUniformLocation(ProgramID, "ScreenWidth");
		HeightSize = glGetUniformLocation(ProgramID, "ScreenHeight");
		ProgramLoaded = true;
	}
	unsigned int StringLength = strlen(DisplayText);
	Width = StringLength*Size;
	VertexCount = 0;
	std::vector<float> Vertex;
	std::vector<float> UV;
	for (unsigned int i=0;i<StringLength;i++)
	{
		float top_leftx=i*Size;
		float top_lefty=Size;
		float top_rightx=(1+i)*Size;
		float top_righty=Size;
		float bottom_leftx=i*Size;
		float bottom_lefty=0;
		float bottom_rightx=(1+i)*Size;
		float bottom_righty=0;
		
		float tilesize = 1.f/16.f;
		float xoff = float((DisplayText[i]-1)%16)/16.f;
		float yoff = 1.f-(float((DisplayText[i]-1)/16)/16.f);
		yoff -= tilesize;
		
		float uv_bottom_leftx = xoff;
		float uv_bottom_lefty = yoff;
		float uv_bottom_rightx=xoff+tilesize;
		float uv_bottom_righty=yoff;
		float uv_top_leftx=xoff;
		float uv_top_lefty=yoff+tilesize;
		float uv_top_rightx=xoff+tilesize;
		float uv_top_righty=yoff+tilesize;
		
		Vertex.push_back(top_leftx);
		Vertex.push_back(top_lefty);
		UV.push_back(uv_top_leftx);
		UV.push_back(uv_top_lefty);
		Vertex.push_back(bottom_leftx);
		Vertex.push_back(bottom_lefty);
		UV.push_back(uv_bottom_leftx);
		UV.push_back(uv_bottom_lefty);
		Vertex.push_back(top_rightx);
		Vertex.push_back(top_righty);
		UV.push_back(uv_top_rightx);
		UV.push_back(uv_top_righty);
		
		Vertex.push_back(bottom_rightx);
		Vertex.push_back(bottom_righty);
		UV.push_back(uv_bottom_rightx);
		UV.push_back(uv_bottom_righty);
		Vertex.push_back(top_rightx);
		Vertex.push_back(top_righty);
		UV.push_back(uv_top_rightx);
		UV.push_back(uv_top_righty);
		Vertex.push_back(bottom_leftx);
		Vertex.push_back(bottom_lefty);
		UV.push_back(uv_bottom_leftx);
		UV.push_back(uv_bottom_lefty);
		VertexCount+=6;
	}
	glDeleteBuffers(1, &VertexBuffer);
	glGenBuffers(1,&VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, Vertex.size() * sizeof(float), &Vertex[0], GL_STATIC_DRAW);
	
	glDeleteBuffers(1, &UVBuffer);
	glGenBuffers(1,&UVBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
	glBufferData(GL_ARRAY_BUFFER, UV.size() * sizeof(float), &UV[0], GL_STATIC_DRAW);
	return 0;
}

int NText::Text::SetSize(float Siz)
{
	Size = Siz;
	return 0;
};

NText::Text::~Text()
{
	glDeleteBuffers(1, &VertexBuffer);
	glDeleteBuffers(1, &UVBuffer);
}
