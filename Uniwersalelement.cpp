#include "Uniwersalelement.h"
#include <iostream>
#include <math.h>

using namespace std;

Uniwersalelement::Uniwersalelement(int x) // te x to nic nie robi tylko inny konstruktor :P
{
	cout << "\n hello univ";
	double alpha = 1 / sqrt(3); //alfa - wspolrzedne punktu calkowania sposobem Gaussa -0.577 lub 0.577
	double n, e;

	this->numberNodes = 4;
	this->numberIntegrationpoints = 4;   //inicjacja liczby pkt. calkowania

	//3 macierze 4x4 wartosci dla 4 f ksztaltu i ich pochodnych dla 4 punktow calkowania
	this->dN_dKsi = new double* [this->numberNodes];
	this->dN_dEta = new double* [this->numberNodes];
	this->N = new double* [this->numberNodes];                  //tablica wartosci dla f ksztaltu

	for (int i = 0; i < this->numberNodes; i++) {
		this->dN_dKsi[i] = new double[this->numberNodes];
		this->dN_dEta[i] = new double[this->numberNodes];
		this->N[i] = new double[this->numberNodes];
	}


	this->points = new Integrationpoint[this->numberIntegrationpoints];  //pkt calkowania
	this->wages = new double[this->numberIntegrationpoints];

	wages[0] = 1;
	wages[1] = 1;
	wages[2] = 1;
	wages[3] = 1;

	this->points[0].eta = -alpha;
	this->points[0].ksi = -alpha;

	this->points[1].eta = -alpha;
	this->points[1].ksi = alpha;

	this->points[2].eta = alpha;
	this->points[2].ksi = alpha;

	this->points[3].eta = alpha;
	this->points[3].ksi = -alpha;


	for (int i = 0; i < numberIntegrationpoints; i++) {

		printf("Punkt calkowania %d: ", i + 1);
		n = this->points[i].eta;
		e = this->points[i].ksi;

		printf("ksi=%f,eta=%f\n", e, n);

		this->N[i][0] = 0.25 * (1 - e) * (1 - n);
		this->dN_dKsi[i][0] = -0.25 * (1 - n);
		this->dN_dEta[i][0] = -0.25 * (1 - e);
		printf("N1: N:%.4f\tdNdksi:%.4f\tdNdeta:%.4f\n", N[i][0], dN_dKsi[i][0], dN_dEta[i][0]);

		this->N[i][1] = 0.25 * (1 + e) * (1 - n);
		this->dN_dKsi[i][1] = 0.25 * (1 - n);
		this->dN_dEta[i][1] = -0.25 * (1 + e);
		printf("N2: N:%.4f\tdNdksi:%.4f\tdNdeta:%.4f\n", N[i][1], dN_dKsi[i][1], dN_dEta[i][1]);

		this->N[i][2] = 0.25 * (1 + e) * (1 + n);
		this->dN_dKsi[i][2] = 0.25 * (1 + n);
		this->dN_dEta[i][2] = 0.25 * (1 + e);
		printf("N3: N:%.4f\tdNdksi:%.4f\tdNdeta:%.4f\n", N[i][2], dN_dKsi[i][2], dN_dEta[i][2]);


		this->N[i][3] = 0.25 * (1 - e) * (1 + n);
		this->dN_dKsi[i][3] = -0.25 * (1 + n);
		this->dN_dEta[i][3] = 0.25 * (1 - e);
		printf("N4: N:%.4f\tdNdksi:%.4f\tdNdeta:%.4f\n", N[i][3], dN_dKsi[i][3], dN_dEta[i][3]);

		printf("\n");
	}

	this->surfedges = new OutsideEdges[4]; //rozpatrujemy element 4 wezlowy
		for (int i = 0; i < 4; i++) { //liczba punktow calkowania :4 tyle ile krawedzi 

			this->surfedges[i].numberIntegrationpointsEdge = 2;
			this->surfedges[i].points = new Integrationpoint[this->surfedges[i].numberIntegrationpointsEdge]; //tablica punktow 1x2

			this->surfedges[i].wages = new double[this->surfedges[i].numberIntegrationpointsEdge]; //tablica wag 1x2
			for (int j = 0; j < this->surfedges[i].numberIntegrationpointsEdge; j++) { // nadanie wartosci waga po 1 dla kazdej
				this->surfedges[i].wages[j] = 1;
			}

			this->surfedges[i].dN_dKsi = new double* [this->numberNodes];
			this->surfedges[i].dN_dEta = new double* [this->numberNodes];
			this->surfedges[i].N = new double* [this->numberNodes];

			for (int j = 0; j < this->numberNodes; j++) {// macieze 1D
				this->surfedges[i].dN_dKsi[j] = new double[this->surfedges[i].numberIntegrationpointsEdge];
				this->surfedges[i].dN_dEta[j] = new double[this->surfedges[i].numberIntegrationpointsEdge];
				this->surfedges[i].N[j] = new double[this->surfedges[i].numberIntegrationpointsEdge];
			}

			this->surfedges[i].localNodes = new int[2];
		}
		//do agregacji
		this->surfedges[0].localNodes[0] = 0;		this->surfedges[0].localNodes[1] = 1;
		this->surfedges[1].localNodes[0] = 1;		this->surfedges[1].localNodes[1] = 2;
		this->surfedges[2].localNodes[0] = 2;		this->surfedges[2].localNodes[1] = 3;
		this->surfedges[3].localNodes[0] = 3;		this->surfedges[3].localNodes[1] = 0;
		
		//---------------------edge1-------------------------
		this->surfedges[0].points[0].ksi = -alpha;		this->surfedges[0].points[0].eta = -1;
		this->surfedges[0].points[1].ksi = alpha;		this->surfedges[0].points[1].eta = -1;

		//---------------------edge2-------------------------
		this->surfedges[1].points[0].ksi = 1;		this->surfedges[1].points[0].eta = -alpha;
		this->surfedges[1].points[1].ksi = 1;		this->surfedges[1].points[1].eta = alpha;

		//---------------------edge3-------------------------
		this->surfedges[2].points[0].ksi = alpha;		this->surfedges[2].points[0].eta = 1;
		this->surfedges[2].points[1].ksi = -alpha;		this->surfedges[2].points[1].eta = 1;

		//---------------------edge4-------------------------
		this->surfedges[3].points[0].ksi = -1;		this->surfedges[3].points[0].eta = alpha;
		this->surfedges[3].points[1].ksi = -1;		this->surfedges[3].points[1].eta = -alpha;



		//surface Integration

		for (int i = 0; i < 4; i++) { //liczba krawedzi
			printf("Powierzchnia %d\n", i + 1);
			for (int j = 0; j < 2; j++) { // liczba pkt. calkowania po krawedzi

				printf("Punkt calkowania %d: ", j + 1);

				n = this->surfedges[i].points[j].eta;
				e = this->surfedges[i].points[j].ksi;

				printf("ksi=%f,eta=%f\n", e, n);

				//dla i tej krawedzi brzegowej wartosc funkcji ksztaltu N od X'j dla N1 N2 N3 N4  edge[i] -> N(x'j)[1-4]
				this->surfedges[i].N[j][0] = 0.25 * (1 - e) * (1 - n);
				this->surfedges[i].N[j][1] = 0.25 * (1 + e) * (1 - n);
				this->surfedges[i].N[j][2] = 0.25 * (1 + e) * (1 + n);
				this->surfedges[i].N[j][3] = 0.25 * (1 - e) * (1 + n);
				//printf("N1:%.4f\tN2:%.4f\tN3:%.4f\tN4:%.4f\n", krawedzie_powierzchniowe[i].wartosci_f_ksztaltu[0][j],
				//	krawedzie_powierzchniowe[i].wartosci_f_ksztaltu[1][j], krawedzie_powierzchniowe[i].wartosci_f_ksztaltu[2][j],
				//	krawedzie_powierzchniowe[i].wartosci_f_ksztaltu[3][j]);


				this->surfedges[i].dN_dKsi[j][0] = -0.25 * (1 - n);
				this->surfedges[i].dN_dKsi[j][1] = 0.25 * (1 - n);
				this->surfedges[i].dN_dKsi[j][2] = 0.25 * (1 + n);
				this->surfedges[i].dN_dEta[j][3] = -0.25 * (1 + n);
				//printf("dN1dksi:%.4f\tdN2dksi:%.4f\tdN3dksi:%.4f\tdN4dksi:%.4f\n", krawedzie_powierzchniowe[i].pochodne_f_ksztaltu_po_ksi[0][j],
				//	krawedzie_powierzchniowe[i].pochodne_f_ksztaltu_po_ksi[1][j], krawedzie_powierzchniowe[i].pochodne_f_ksztaltu_po_ksi[2][j],
				//	krawedzie_powierzchniowe[i].pochodne_f_ksztaltu_po_ksi[3][j]);

				this->surfedges[i].dN_dEta[j][0] = -0.25 * (1 - e);
				this->surfedges[i].dN_dEta[j][1] = -0.25 * (1 + e);
				this->surfedges[i].dN_dEta[j][2] = 0.25 * (1 + e);
				this->surfedges[i].dN_dEta[j][3] = 0.25 * (1 - e);
				//printf("dN1deta:%.4f\tdN2deta:%.4f\tdN3deta:%.4f\tdN4deta:%.4f\n", krawedzie_powierzchniowe[i].pochodne_f_ksztaltu_po_eta[0][j],
				//	krawedzie_powierzchniowe[i].pochodne_f_ksztaltu_po_eta[1][j], krawedzie_powierzchniowe[i].pochodne_f_ksztaltu_po_eta[2][j],
				//	krawedzie_powierzchniowe[i].pochodne_f_ksztaltu_po_eta[3][j]);
			}
			printf("\n");
		}


	}


Uniwersalelement::Uniwersalelement()
{
	cout << "fdffdfddff\n\nfdfdfdfd";
	double alpha = 1 / sqrt(3); //alfa - wspolrzedne punktu calkowania sposobem Gaussa -0.577 lub 0.577
	double n, e;

	this->numberNodes = 4;
	this->numberIntegrationpoints = 4;   //inicjacja liczby pkt. calkowania

	//3 macierze 4x4 wartosci dla 4 f ksztaltu i ich pochodnych dla 4 punktow calkowania
	this->dN_dKsi = new double* [this->numberNodes];
	this->dN_dEta = new double* [this->numberNodes];
	this->N = new double* [this->numberNodes];                  //tablica wartosci dla f ksztaltu


	for (int i = 0; i < this->numberNodes; i++) {
		this->dN_dKsi[i] = new double[this->numberNodes];
		this->dN_dEta[i] = new double[this->numberNodes];
		this->N[i] = new double[this->numberNodes];
	}


	this->points = new Integrationpoint[this->numberIntegrationpoints];  //pkt calkowania
	this->wages = new double[this->numberIntegrationpoints];

	wages[0] = 1;
	wages[1] = 1;
	wages[2] = 1;
	wages[3] = 1;

	this->points[0].eta = -alpha;
	this->points[0].ksi = -alpha;

	this->points[1].eta = -alpha;
	this->points[1].ksi = alpha;

	this->points[2].eta = alpha;
	this->points[2].ksi = alpha;

	this->points[3].eta = alpha;
	this->points[3].ksi = -alpha;


	for (int i = 0; i < numberIntegrationpoints; i++) {

		printf("Punkt calkowania %d: ", i + 1);
		n = this->points[i].eta;
		e = this->points[i].ksi;

		printf("ksi=%f,eta=%f\n", e, n);
	
		this->N[i][0] = 0.25 * (1 - e) * (1 - n);
		this->dN_dKsi[i][0] = -0.25 * (1 - n);
		this->dN_dEta[i][0] = -0.25 * (1 - e);
		printf("1fksztaltu: N:%.4f\tdNdksi:%.4f\tdNdeta:%.4f\n", N[i][0], dN_dKsi[i][0], dN_dEta[i][0]);

		this->N[i][1] = 0.25 * (1 - e) * (1 - n);
		this->dN_dKsi[i][1] = -0.25 * (1 - n);
		this->dN_dEta[i][1] = -0.25 * (1 - e);
		printf("1fksztaltu: N:%.4f\tdNdksi:%.4f\tdNdeta:%.4f\n", N[i][1], dN_dKsi[i][1], dN_dEta[i][1]);

		this->N[i][2] = 0.25 * (1 - e) * (1 - n);
		this->dN_dKsi[i][2] = -0.25 * (1 - n);
		this->dN_dEta[i][2] = -0.25 * (1 - e);
		printf("1fksztaltu: N:%.4f\tdNdksi:%.4f\tdNdeta:%.4f\n", N[i][2], dN_dKsi[i][2], dN_dEta[i][2]);


		this->N[i][3] = 0.25 * (1 - e) * (1 - n);
		this->dN_dKsi[i][3] = -0.25 * (1 - n);
		this->dN_dEta[i][3] = -0.25 * (1 - e);
		printf("1fksztaltu: N:%.4f\tdNdksi:%.4f\tdNdeta:%.4f\n", N[i][3], dN_dKsi[i][3], dN_dEta[i][3]);

		printf("\n");
	}
}