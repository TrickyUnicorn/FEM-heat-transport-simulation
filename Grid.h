#pragma once
class Node;
class Element;

class Grid
{
public:
	double H; //height
	double W; //width

	int nH; // number of nodes height
	int nW; // number of nodes width

	int nN; // number of nodes
	int nE; // number of elements

	double dx; // step of one node	x1---dx---x2
	double dy; // step of one node	y1---dy---y2

	Node *nodes;
	Element* elements;



	Grid(double height, double width, int nHeight, int nWidth, double mTbegin);
	void wypisz();


};





class Node {
	public:
	double x, y; //position in grid(x, y)
	int id;       //node id
	bool bc;	 //boundary condition
	double t;

	Node(double x, double y, int id, bool bc, double t);
	Node();
};

class Element {
public:
	int id;       //element id
	Node node[4];

	double** H;
	double** C;
	double *P;
	double **HC;
	double* PC;

	int numContactEdges;
	int idContactEdges[2];

	Element(int id, Node * nodes);
	Element();
};