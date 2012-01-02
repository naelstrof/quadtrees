/* Notice: This was designed for and on a linux machine, if you want to port 
 * it to windows you'll simply have to replace NWindow with a windows compadible
 * window helper, like glfw. And replace a bunch of linux timers and nanosleeps.*/

#include <iostream>
#include <cstdlib>
#include "NWindow.h"
#include "NText.h"
#include "NQuadTree.h"

unsigned int Width = 800;
unsigned int Height = 800;

int CleanAndExit()
{
	NTexture::CleanUp();
	exit(0);
}

class Dot //This'll be our little object that gets stored into our quad tree
{
public:
	float X,Y;
	float W,H;
	float VX,VY;
	int Draw();
	int Update(NQuadTree::Node*);
	int UpdatePos();
	Dot(float,float,float);
};

Dot::Dot(float x, float y, float w)
{
	VX = 0;
	VY = 0;
	VX = 0;
	VY = 0;
	W = w;
	H = w;
	X = x;
	Y = y;
}
int Dot::Draw()
{
	glPointSize(W);
	glColor3f(0,0,0);
	glBegin(GL_POINTS);
	glVertex2f(X,Y); 
	glEnd();
	return 0;
}
int Dot::Update(NQuadTree::Node* Check)
{
	if (Y<Height-H/2)
	{
		VY+=0.2;
	} else {
		VX*=0.9;
		Y=Height-H/2;
		VY=0;
	}
	if (X>Width-W/2)
	{
		VX=-1;
		X=Width-W/2;
	}
	if (X<W/2)
	{
		VX=1;
		X=W/2;
	}
	NQuadTree::Node* Leaf = Check->GetLeaf(X+VX,Y+VY+(H/2));
	if (Leaf == NULL)	
	{
		return 1;
	}
	for (unsigned int i=0;i<Leaf->Objects.size();i++)
	{
		Dot* CheckingDot = (Dot*)Leaf->Objects[i];
		bool XRange = (abs(CheckingDot->X - X) < (W+CheckingDot->W)/2);
		bool YRange = (abs(CheckingDot->Y - Y) < (H+CheckingDot->H)/2);
		if (XRange && YRange)
		{
			VX*=0.9;
			VY=0;
			Y=CheckingDot->Y-(H+CheckingDot->H)/2;
		}
	}
	return 0;
}
int Dot::UpdatePos()
{
	Y += VY;
	X += VX;
	return 0;
}

static std::vector<Dot> Dots;
int main(int argc, char* argv[])
{
	NWindow Window(&Width,&Height,"Quadtrees!",false,argc,argv); //Create and size window (with opengl bindings)
	NText::SetScreen(Width, Height); //Make sure our text render-er knows how large the window is
	NText::Text MyText("Quadtrees!","textures/boot",16,0,0,1,1,1,1); //Create a text object
	glClearColor(0.2,0.1,0.2,1); //Set clear color to some sweet purple color
	NQuadTree::Node* MyNode = NQuadTree::NewNode(0,0,Width,Height,NULL); //Create our root quad-tree and subdivide it 4 times
	MyNode->SubDivide();
	MyNode->SubDivide();
	MyNode->SubDivide();
	MyNode->SubDivide();
	while(true)
	{
		if (Window.ChangedSize(&Width,&Height)) //If the window changes size, update the shaders.
		{
			NText::SetScreen(Width, Height);
		}
		if (!Window.CheckOpen()) //If the window is closed, clean up and exit.
		{
			std::cout << "X window closed, exiting...\n";
			CleanAndExit();
		}
		int X,Y;
		Window.GetMouse(&X,&Y);
		if (Window.GetKey() == 25) //If W is pressed clear the screen
		{
			Dots.clear();
		}
		for (unsigned int i=0;i<3;i++) //create 3 dots per frame
		{
			Dot MyDot(rand()%Width,0,3+rand()%10);
			MyDot.VX = float(rand()%10-rand()%10);
			Dots.push_back(MyDot);
		}
		MyNode->Clear(); //empty out the quad tree
		for (unsigned int i=0;i<Dots.size();i++) //Update every dot and insert them into the quad tree
		{
			Dots[i].Update(MyNode);
			MyNode->AddObject(&Dots[i]);
		}
		glClear(GL_COLOR_BUFFER_BIT); //Clear buffer to our purple color.
		NQuadTree::DrawNodes(); //Draw some boxes around our quad tree's nodes
		for (unsigned int i=0;i<Dots.size();i++) //Draw our dots
		{
			Dots[i].Draw();
		}
		MyText.Draw(); //Draw our text into the buffer.
		Window.SwapBuffer(); //Swap the buffer, effectively displaying the drawn image.
		Window.CapFPS(60); //Cap the fps at 60 so our program doesn't eat all the CPU it can.
		for (unsigned int i=0;i<Dots.size();i++) //Delayed position update to keep collisions sane
		{
			Dots[i].UpdatePos();
		}
	}
	return 0;
}