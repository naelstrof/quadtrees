#include "NQuadTree.h"

int NQuadTree::DrawNodes() //Loop through all nodes and draw a box around active ones
{
	glLineWidth(3);
	glColor3f(0.18,0.08,0.18);
	glBegin(GL_LINES);
	for (unsigned int i=0;i<Nodes.size();i++)
	{
		if (Nodes[i]->Objects.size() == 0)
		{
			continue;
		}
		glVertex2f(Nodes[i]->X, Nodes[i]->Y); 
		glVertex2f(Nodes[i]->X+Nodes[i]->W, Nodes[i]->Y);
		
		glVertex2f(Nodes[i]->X+Nodes[i]->W, Nodes[i]->Y);
		glVertex2f(Nodes[i]->X+Nodes[i]->W, Nodes[i]->Y+Nodes[i]->H);
		
		glVertex2f(Nodes[i]->X+Nodes[i]->W, Nodes[i]->Y+Nodes[i]->H);
		glVertex2f(Nodes[i]->X, Nodes[i]->Y+Nodes[i]->H);
		
		glVertex2f(Nodes[i]->X, Nodes[i]->Y+Nodes[i]->H);
		glVertex2f(Nodes[i]->X, Nodes[i]->Y); 
	}
	glEnd();
	return 0;
}

NQuadTree::Node* NQuadTree::NewNode(float x,float y,float w,float h,Node* parent) //Create a new node and push it onto a node array
{
	NQuadTree::Node* MyNode = new Node[1];
	MyNode->X = x;
	MyNode->Y = y;
	MyNode->W = w;
	MyNode->H = h;
	MyNode->Parent = parent;
	MyNode->HasChildren = false;
	MyNode->LastAdded = NULL;
	NQuadTree::Nodes.push_back(MyNode);
	return MyNode;
}

int NQuadTree::Node::SubDivide() //Create four new nodes inside our parent, unless it already has children, then make them subdivide instead.
{
	if (!HasChildren)
	{
		float hW = W/2;
		float hH = H/2;
		Children[0][0] = NQuadTree::NewNode(X,Y,hW,hH,this);
		Children[1][0] = NQuadTree::NewNode(X+hW,Y,hW,hH,this);
		Children[0][1] = NQuadTree::NewNode(X,Y+hH,hW,hH,this);
		Children[1][1] = NQuadTree::NewNode(X+hW,Y+hH,hW,hH,this);
		HasChildren = true;
	} else {
		for (unsigned int i=0;i<2;i++)
		{
			for (unsigned int j=0;j<2;j++)
			{
				Children[i][j]->SubDivide();
			}
		}
	}
	return 0;
}

int cround(float x) //I use this to keep from segfaulting when a position is out of bounds
{
	int t = round(x);
	if (t<0)
	{
		return 0;
	} else if (t>1)
	{
		return 1;
	}
	return t;
}

int NQuadTree::Node::AddObject(void* Obj) //Adding objects, it isn't perfect but i don't know how to do it optmially.
{
	if (LastAdded == Obj) //If we've already added this object, exit.
	{
		return 0;
	}
	Objects.push_back(Obj); //Add the object to our node.
	if (!HasChildren) //If we have no children to push the object into, exit.
	{
		return 0;
	}
	LastAdded = Obj;
	float Objx = ((float*)Obj)[0]; //get the position and sizes from our object
	float Objy = ((float*)Obj)[1];
	float Objw = ((float*)Obj)[2];
	float Objh = ((float*)Obj)[3];
	float Winc = Objw/W;
	float Hinc = Objh/H;
	float NX = (Objx-X)/W;
	float NY = (Objy-Y)/H;
	Children[cround(NX+Winc)][cround(NY+Hinc)]->AddObject(Obj); //if any of the object's corners are in a node, add it to them.
	Children[cround(NX+Winc)][cround(NY-Hinc)]->AddObject(Obj); //This is why we have the LastAdded check, too keep it from adding repediately.
	Children[cround(NX-Winc)][cround(NY+Hinc)]->AddObject(Obj); //This won't work for objects bigger than the smallest leaf node.
	Children[cround(NX-Winc)][cround(NY-Hinc)]->AddObject(Obj);
	return 0;
}

int NQuadTree::Node::Clear() //Empty the quad tree's objects.
{
	Objects.clear();
	if (!HasChildren)
	{
		return 0;
	}
	for (unsigned int i=0;i<2;i++)
	{
		for (unsigned int j=0;j<2;j++)
		{
			Children[i][j]->Clear();
		}
	}
	return 0;
}

NQuadTree::Node* NQuadTree::Node::GetLeaf(float x, float y) //Recursively dive into the tree until we reach either an empty node or a leaf node.
{
	if (x < X || x > X+W || y < Y || y > Y+H || Objects.size() == 0)
	{
		return NULL;
	}
	if (!HasChildren)
	{
		return this;
	}
	int NX = cround((x-X)/W);
	int NY = cround((y-Y)/H);
	NQuadTree::Node* Check = Children[NX][NY]->GetLeaf(x,y);
	return Check;
}