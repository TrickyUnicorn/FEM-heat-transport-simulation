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

	OutsideEdges * surfedges; //tablica o d�ugo�ci maksymalnej liczbie powierzchni wykorzystanego elementu sko�czonego 
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

	int * localNodes;   //tablica zawieraj�ca lokalne numery w�z��w kraw�dzi

};

class Integrationpoint {
public:
	double ksi;
	double eta;
};

