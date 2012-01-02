#ifndef NAELSTROF_QUADTREE
#define NAELSTROF_QUADTREE

#include <iostream>
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <vector>
#include <cmath>

namespace NQuadTree
{
	class Node
	{
	public:
		float X,Y,W,H;
		unsigned int Level;
		Node* Parent;
		void** GetObjects(int*);
		Node** GetChildren(int*);
		int SubDivide();
		int AddObject(void*);
		int Update();
		Node* GetLevel(float,float,unsigned int);
		Node* GetChild(float,float);
		int RemoveObject(void*);
		Node* GetLeaf(float,float);
		int Clear();
		std::vector<void*> Objects;
		bool HasChildren;
		Node* Children[2][2];
		void* LastAdded;
	};
	static std::vector<Node*> Nodes;
	int DrawNodes();
	Node* NewNode(float,float,float,float,Node*);
};

#endif