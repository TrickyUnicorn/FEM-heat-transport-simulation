#include <iostream>

#pragma once 
#include "Grid.h"

using namespace std;


 Grid::Grid(double height, double width, int nHeight, int nWidth, double mTbegin)
 {
	this->H = height;
	this->W = width;
	this->nH = nHeight;
	this->nW = nWidth;

	this->nN = nH * nW;
	this->nE = (nH - 1) * (nW - 1);

	this->dx = W / (nW-1);
	this->dy = H / (nH-1);

	//add nodes to grid
	this->nodes = new Node [nN];
	

	for (int i = 0; i < nW; i++) {
		for (int j = 0; j < nH; j++) {
			int iD = i * nH + j;
			//boundary condition =1 if not 0
			bool bcond=0;
			if (i == 0 || j == 0 || (j == nH - 1) || (i == nW - 1)) bcond = 1;
			nodes[iD] = Node(i*dx, j*dy, iD, bcond, mTbegin);
		}
	}

	//add elements to grid
	this->elements = new Element [nE];


	for (int i = 0; i < nW-1; i++) {
		for (int j = 0; j < nH-1; j++) {
			int iD = i * (nH - 1)+j;
			Node nod[4];
			nod[0] = nodes[i*nH +j];
			nod[1] = nodes[i*nH +j + nH];
			nod[2] = nodes[i*nH +j + nH +1];
			nod[3] = nodes[i*nH +j +1];

			elements[iD] = Element(iD,nod ) ;
			cout << iD << "  ";
		}
		cout << endl;
	}

	//-----------dla-4pc--------
	

	cout << "Grid created succesfully!\n";
 }

 void Grid::wypisz()
 {
	 for (int i = 0; i < nH - 1; i++) {
		 for (int j = 0; j < nW - 1; j++) {
			 cout << elements[i + j*nW ].id<<"  ";
		 }
		 cout << endl;
	 }
	 /*
	 for (int i = 0; i < nH ; i++) {
		 for (int j = 0; j < nW ; j++) {
			 cout << nodes[i + j * (nW+1)].bc << "  ";
		 }
		 cout << endl;
	 }
	 */

	// cout << elements[nE - 1].node[0].id;
	 
 }

 Node::Node(double x, double y, int id, bool bc=false, double mTbegin=0)
 {
	 this->x = x;
	 this->y = y;
	 this->id = id;
	 this->bc = bc;
	 this->t = mTbegin;
 }
 Node::Node()
 {
 }

 Element::Element(int id, Node nodes[4])
 {
	 this->id = id;
	 this->node[0] = nodes[0];
	 this->node[1] = nodes[1];
	 this->node[2] = nodes[2];
	 this->node[3] = nodes[3];

	 this->H = new double* [4];
	 this->C = new double* [4];
	 this->P = new double [4];

	 this->HC = new double* [4];
	 this->PC = new double [4];

	 for (int i = 0; i < 4; i++)
	 {
		 this->H[i] = new double[4];
		 this->C[i] = new double[4];
		 this->HC[i] = new double[4];
	 }

	 //-------------------boudnary-condition=1-if-YES--------------------------------------
	 bool bc0 = 0, bc1 = 0, bc2 = 0, bc3 = 0;
	 this->numContactEdges = 0;
	 this->idContactEdges[0] = -1;
	 this->idContactEdges[1] = -1;

		 /* sprawdzenie lokalnych statusow */
		 bc0 = node[0].bc;
		 bc1 = node[1].bc;
		 bc2 = node[2].bc;
		 bc3 = node[3].bc;

		 /*=LOKALNE ID SCIAN KONTAKTOWYCH=*/
		 /*=			 gora=2				=*/
		 /*=  lewo=3			  prawo=1	=*/
		 /*=			  dol=0				=*/
		 /*===============================*/

/* down nodes 0 & 1 */
		 if (bc0 == 1 && bc1 == 1) {
			 if (this->numContactEdges == 0) {
				this->idContactEdges[0] = 0;
			 }
			 else {
				 this->idContactEdges[1] = 0;
			 }
			 this->numContactEdges++;
		 }

		 /* right nodes 1 & 2 */
		 if (bc1 == 1 && bc2 == 1) {
			 if (this->numContactEdges == 0) {
				 this->idContactEdges[0] = 1;
			 }
			 else {
				 this->idContactEdges[1] = 1;
			 }
			 this->numContactEdges++;
		 }

		 /* wezly 2 i 3 - warunek brzegowy zachodzi na gornej scianie */
		 if (bc2 == 1 && bc3 == 1) {
			 if (this->numContactEdges == 0) {
				 this->idContactEdges[0] = 2;
			 }
			 else {
				 this->idContactEdges[1] = 2;
			 }
			 this->numContactEdges++;
		 }

		 /* wezly 3 i 0 - warunek brzegowy zachodzi na lewej scianie */
		 if (bc3 == 1 && bc0 == 1) {
			 if (this->numContactEdges == 0) {
				 this->idContactEdges[0] = 3;
			 }
			 else {
				 this->idContactEdges[1] = 3;
			 }
			 this->numContactEdges++;
		 }

		 /* jezeli nie zachodzi warunek brzegowy */
		 if (this->numContactEdges <= 0) {
			 this->idContactEdges[0] = -1;
			 this->idContactEdges[1] = -1;

		 }

		 printf("ilosc scian kontaktowych: %d\n", this->numContactEdges);
		 printf("lokalne id scian kontaktowych: %d %d\n\n", this->idContactEdges[0], idContactEdges[1]);

	 }

 

 Element::Element()
 {
 }
 