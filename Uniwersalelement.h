#pragma once
class Integrationpoint;
class OutsideEdges;
class Uniwersalelement
{
public:
	int numberNodes;
	int numberIntegrationpoints;

	double** dN_dKsi;
	double** dN_dEta;
	double** N;
	
	double* wages;             //wagi 

	OutsideEdges * surfedges; //tablica o d³ugoœci maksymalnej liczbie powierzchni wykorzystanego elementu skoñczonego 
	Integrationpoint * points;
	Uniwersalelement(int x);
	Uniwersalelement();
};

class OutsideEdges {
public:
	int numberIntegrationpointsEdge;
	Integrationpoint * points;
	double* wages;

	double** dN_dKsi;
	double** dN_dEta;
	double** N;

	int * localNodes;   //tablica zawieraj¹ca lokalne numery wêz³ów krawêdzi

};

class Integrationpoint {
public:
	double ksi;
	double eta;
};

