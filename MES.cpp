//Samuel Ozimina MES(FEM)
//

#include <iostream>

#include "Grid.h"
#include"Uniwersalelement.h"
//#include "Grid.cpp"

using namespace std;

double* gausIntegration(int n, double** AB, double* X);

int main()
{
	//def global data
	double height = 0.1;
	double width = 0.1;
	int nH = 4;
	int nW = 4;

	double tbegin=100;							/* temperatura poczatkowa C*/
	double fTime=500;							/* czas procesu s*/
	double dTime=50;							/* krok czasowy s*/
	double ambientT=1200;                      // T otoczenia

	double mK = 25;              //wspolczynnik przewodzenia ciepla W/(m*C)
	double ro=7800;                    //gestosc
	double cw=700;                    //cieplo wlasciwe
	double alpha=300;                 //wspolczynnik wymiany ciepla W/(m^2*K)

	//generate grid
	Grid grid1(height, width, nH, nW, tbegin);

	grid1.wypisz();
	Uniwersalelement univelement(7);

	//--------H, C, P :global-----------------
	double** H_g = new double* [nH * nW];
	double** C_g = new double* [nH * nW];
	double* P_g = new double [nH * nW];

	//-------H, HC, C, P, PC : local------------------
	double H_el[4][4];
	double HC_el[4][4];
	double C_el[4][4];
	double P_el[4];
	double PC_el[4];

	double J[2][2], Jinv[2][2], detJ;																/* jakobian przekształcenia, macierz odwrotna i wyznacznik */
	double dNdX[4][4], dNdY[4][4];																	/* dla 4 punktow calkowania: pochodne 4 funkcji ksztaltu po X oraz po Y*/
	double HintegrationPoint[4][4][4], CintegrationPoint[4][4][4];		 /* dla 4 punktow calkowania: macierz H zawierajaca sume calki po objetosci oraz calki po powierzchni, macierz C zawierajaca calke po objetosci */
	double PintegrationPoint[4][4];													/* dla 4 punktow calkowania: wektor obciazen P zawierajacy calke po powierzchni (uwzglednia warunki brzegowe) */
	double T0[4], * T1 = new double[nH * nW];

	for (int i = 0; i <= nH * nW; i++) {
		T1[i] = 0;
	}
	
	//----------------Init--arrays----------------
	
	for (int i = 0; i < nH * nW; i++) {
		H_g[i] = new double[nH*nW];

	}

	for (int i = 0; i < nW * nH; i++) {
		for (int j = 0; j < nW * nH; j++) {
			H_g[i][j] = 0;
		}
		P_g[i] = 0;
	}
	
	//---------------rozpoczecie-procesu-w-czasie-----------
	int iteracja = 0;

	cout << "\n-------press enter to start process-------\n"; getchar();
	//cout << "\n\n---begining-------node.t------------\n"; 



	for (double t = 0; t < fTime; t += dTime)									/* petla po czasie */
	{

		for (int i = 0; i < nW * nH; i++) {
			for (int j = 0; j < nW * nH; j++) {
				H_g[i][j] = 0;
			}
			P_g[i] = 0;
		}
	

		for (int e = 0; e < grid1.nE; e++) 											/* petla po elementach */
		{
			// Zerowanie macierzy dla punktow calkowania
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					for (int k = 0; k < 4; k++) {
						HintegrationPoint[i][j][k] = 0;
						CintegrationPoint[i][j][k] = 0;
					}
					PintegrationPoint[i][j] = 0;
				}
			}
			// Zerowanie temperatur poczatkowych
			for (int i = 0; i < 4; i++) {
				T0[i] = 0;
			}
			for (int i = 0; i<nW * nH; i++) {
				T1[i] = 0;
			}


			// Zerowanie macierzy dla elementow i pochodnych fksztaltu po x i y
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					H_el[i][j] = 0;
					C_el[i][j] = 0;
					HC_el[i][j] = 0;
				}
				P_el[i] = 0;
				PC_el[i] = 0;
			}

			// interpolacja temperatury
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					T0[i] += univelement.N[i][j] * grid1.nodes[grid1.elements[e].node[j].id].t;
					//cout << "\ni" << j << ": " << grid1.nodes[grid1.elements[e].node[j].id].t;
				}//cout << "\ni: " << i<<endl;
			}

			// W konkretnym elemencie petla po punktach calkowania 
			for (int integP = 0; integP < 4; integP++) {

				// Zerowanie  pochodnych fksztaltu po x i y
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						dNdX[i][j] = 0;
						dNdY[i][j] = 0;
					}
				}
				// Zerowanie jakobianu
				detJ = 0;
				for (int i = 0; i < 2; i++) {
					for (int j = 0; j < 2; j++) {
						J[i][j] = 0;
						Jinv[i][j] = 0;
					}
				}
				double ksi = univelement.points[integP].ksi;
				double eta = univelement.points[integP].eta;

				/* dx po dksi */
				J[0][0] = 0.25 * (eta - 1) * grid1.elements[e].node[0].x +
					0.25 * (1 - eta) * grid1.elements[e].node[1].x +
					0.25 * (1 + eta) * grid1.elements[e].node[2].x -
					0.25 * (1 + eta) * grid1.elements[e].node[3].x;

				/* dy po dksi */
				J[0][1] = 0.25 * (eta - 1) * grid1.elements[e].node[0].y +
					0.25 * (1 - eta) * grid1.elements[e].node[1].y +
					0.25 * (1 + eta) * grid1.elements[e].node[2].y -
					0.25 * (1 + eta) * grid1.elements[e].node[3].y;

				/* dx po deta */
				J[1][0] = 0.25 * (ksi - 1) * grid1.elements[e].node[0].x -
					0.25 * (1 + ksi) * grid1.elements[e].node[1].x +
					0.25 * (1 + ksi) * grid1.elements[e].node[2].x +
					0.25 * (1 - ksi) * grid1.elements[e].node[3].x;

				/* dy po deta */
				J[1][1] = 0.25 * (ksi - 1) * grid1.elements[e].node[0].y -
					0.25 * (1 + ksi) * grid1.elements[e].node[1].y +
					0.25 * (1 + ksi) * grid1.elements[e].node[2].y +
					0.25 * (1 - ksi) * grid1.elements[e].node[3].y;

				/* wyznacznik jakobianu przeksztalcenia */
				detJ = J[0][0] * J[1][1] - J[0][1] * J[1][0];

				/* macierz odwrotna do jakobianu przeksztalcenia */
				Jinv[0][0] = J[1][1] * (1 / detJ);
				Jinv[1][1] = J[0][0] * (1 / detJ);
				Jinv[1][0] = -J[1][0] * (1 / detJ);
				Jinv[0][1] = -J[0][1] * (1 / detJ);

				/* wyznaczamy dla punktow calkowania poj elementu pochodne funkcji ksztaltu po x i po y za pomocą jakobianu przeksztalcenia*/
				dNdX[integP][0] = (Jinv[0][0] * univelement.dN_dKsi[integP][0] - Jinv[0][1] * univelement.dN_dEta[integP][0]);
				dNdX[integP][1] = (Jinv[0][0] * univelement.dN_dKsi[integP][1] - Jinv[0][1] * univelement.dN_dEta[integP][1]);
				dNdX[integP][2] = (Jinv[0][0] * univelement.dN_dKsi[integP][2] - Jinv[0][1] * univelement.dN_dEta[integP][2]);
				dNdX[integP][3] = (Jinv[0][0] * univelement.dN_dKsi[integP][3] - Jinv[0][1] * univelement.dN_dEta[integP][3]);

				dNdY[integP][0] = (-Jinv[1][0] * univelement.dN_dKsi[integP][0] + Jinv[1][1] * univelement.dN_dEta[integP][0]);
				dNdY[integP][1] = (-Jinv[1][0] * univelement.dN_dKsi[integP][1] + Jinv[1][1] * univelement.dN_dEta[integP][1]);
				dNdY[integP][2] = (-Jinv[1][0] * univelement.dN_dKsi[integP][2] + Jinv[1][1] * univelement.dN_dEta[integP][2]);
				dNdY[integP][3] = (-Jinv[1][0] * univelement.dN_dKsi[integP][3] + Jinv[1][1] * univelement.dN_dEta[integP][3]);

				/* Liczymy macierz H  i C */
				HintegrationPoint[integP][0][0] = mK * (dNdX[integP][0] * dNdX[integP][0] + dNdY[integP][0] * dNdY[integP][0]) * detJ;
				HintegrationPoint[integP][0][1] = mK * (dNdX[integP][0] * dNdX[integP][1] + dNdY[integP][0] * dNdY[integP][1]) * detJ;
				HintegrationPoint[integP][0][2] = mK * (dNdX[integP][0] * dNdX[integP][2] + dNdY[integP][0] * dNdY[integP][2]) * detJ;
				HintegrationPoint[integP][0][3] = mK * (dNdX[integP][0] * dNdX[integP][3] + dNdY[integP][0] * dNdY[integP][3]) * detJ;

				HintegrationPoint[integP][1][0] = mK * (dNdX[integP][1] * dNdX[integP][0] + dNdY[integP][1] * dNdY[integP][0]) * detJ;
				HintegrationPoint[integP][1][1] = mK * (dNdX[integP][1] * dNdX[integP][1] + dNdY[integP][1] * dNdY[integP][1]) * detJ;
				HintegrationPoint[integP][1][2] = mK * (dNdX[integP][1] * dNdX[integP][2] + dNdY[integP][1] * dNdY[integP][2]) * detJ;
				HintegrationPoint[integP][1][3] = mK * (dNdX[integP][1] * dNdX[integP][3] + dNdY[integP][1] * dNdY[integP][3]) * detJ;

				HintegrationPoint[integP][2][0] = mK * (dNdX[integP][2] * dNdX[integP][0] + dNdY[integP][2] * dNdY[integP][0]) * detJ;
				HintegrationPoint[integP][2][1] = mK * (dNdX[integP][2] * dNdX[integP][1] + dNdY[integP][2] * dNdY[integP][1]) * detJ;
				HintegrationPoint[integP][2][2] = mK * (dNdX[integP][2] * dNdX[integP][2] + dNdY[integP][2] * dNdY[integP][2]) * detJ;
				HintegrationPoint[integP][2][3] = mK * (dNdX[integP][2] * dNdX[integP][3] + dNdY[integP][2] * dNdY[integP][3]) * detJ;

				HintegrationPoint[integP][3][0] = mK * (dNdX[integP][3] * dNdX[integP][0] + dNdY[integP][3] * dNdY[integP][0]) * detJ;
				HintegrationPoint[integP][3][1] = mK * (dNdX[integP][3] * dNdX[integP][1] + dNdY[integP][3] * dNdY[integP][1]) * detJ;
				HintegrationPoint[integP][3][2] = mK * (dNdX[integP][3] * dNdX[integP][2] + dNdY[integP][3] * dNdY[integP][2]) * detJ;
				HintegrationPoint[integP][3][3] = mK * (dNdX[integP][3] * dNdX[integP][3] + dNdY[integP][3] * dNdY[integP][3]) * detJ;
				


				for (int z = 0; z < 4; z++) {
					for (int x = 0; x < 4; x++) {
						H_el[z][x] += HintegrationPoint[integP][z][x];
					}
				}


				CintegrationPoint[integP][0][0] = cw * ro * univelement.N[integP][0] * univelement.N[integP][0] * detJ;
				CintegrationPoint[integP][0][1] = cw * ro * univelement.N[integP][0] * univelement.N[integP][1] * detJ;
				CintegrationPoint[integP][0][2] = cw * ro * univelement.N[integP][0] * univelement.N[integP][2] * detJ;
				CintegrationPoint[integP][0][3] = cw * ro * univelement.N[integP][0] * univelement.N[integP][3] * detJ;

				CintegrationPoint[integP][1][0] = cw * ro * univelement.N[integP][1] * univelement.N[integP][0] * detJ;
				CintegrationPoint[integP][1][1] = cw * ro * univelement.N[integP][1] * univelement.N[integP][1] * detJ;
				CintegrationPoint[integP][1][2] = cw * ro * univelement.N[integP][1] * univelement.N[integP][2] * detJ;
				CintegrationPoint[integP][1][3] = cw * ro * univelement.N[integP][1] * univelement.N[integP][3] * detJ;

				CintegrationPoint[integP][2][0] = cw * ro * univelement.N[integP][2] * univelement.N[integP][0] * detJ;
				CintegrationPoint[integP][2][1] = cw * ro * univelement.N[integP][2] * univelement.N[integP][1] * detJ;
				CintegrationPoint[integP][2][2] = cw * ro * univelement.N[integP][2] * univelement.N[integP][2] * detJ;
				CintegrationPoint[integP][2][3] = cw * ro * univelement.N[integP][2] * univelement.N[integP][3] * detJ;

				CintegrationPoint[integP][3][0] = cw * ro * univelement.N[integP][3] * univelement.N[integP][0] * detJ;
				CintegrationPoint[integP][3][1] = cw * ro * univelement.N[integP][3] * univelement.N[integP][1] * detJ;
				CintegrationPoint[integP][3][2] = cw * ro * univelement.N[integP][3] * univelement.N[integP][2] * detJ;
				CintegrationPoint[integP][3][3] = cw * ro * univelement.N[integP][3] * univelement.N[integP][3] * detJ;
				
				//cout << "\n\n----moj--PC_el[0]----------\n" << PC_el[0];
				for (int z = 0; z < 4; z++) {
					for (int x = 0; x < 4; x++) {
						C_el[z][x] += CintegrationPoint[integP][z][x];
					}
				}

				for (int z = 0; z < 4; z++) {
					for (int x = 0; x < 4; x++) {
						PC_el[z] += (CintegrationPoint[integP][z][x] / dTime) * T0[integP];
					}
				}
			}
			/* Petla po punktach calkowania ale na krawedziach */
			for (int i = 0; i < grid1.elements[e].numContactEdges; i++) {

				int edge_id = grid1.elements[e].idContactEdges[i];

				double X[4], Y[4];
				double DetJ = 0.;

				for (int j = 0; j < 4; j++) {
					X[j] = grid1.elements[e].node[j].x;
					Y[j] = grid1.elements[e].node[j].y;
				}

				switch (edge_id) {
				case 0:	
					DetJ = sqrt(pow((X[0] - X[1]), 2) + pow((Y[0] - Y[1]), 2)) / 2;
					break;
				case 1: 
					DetJ = sqrt(pow((X[1] - X[2]), 2) + pow((Y[1] - Y[2]), 2)) / 2;
					break;
				case 2: 
					DetJ = sqrt(pow((X[2] - X[3]), 2) + pow((Y[2] - Y[3]), 2)) / 2;
					break;
				case 3: 
					DetJ = sqrt(pow((X[3] - X[0]), 2) + pow((Y[3] - Y[0]), 2)) / 2;
					break;
				}
				double n1 = 0., n2 = 0., pn = 0.;
				for (int q = 0; q < 2; q++) {	/* calkowanie po krawedzi */
					for (int w = 0; w < 4; w++) {								/* petla po grupach rownan rownym wierszom macierzy */
						for (int k = 0; k < 4; k++) {							/* petla po kolumnach macierzy */
							//n1 = elem->krawedzie_powierzchniowe[id_sciany].wartosci_f_ksztaltu[q][k];
							n1 = univelement.surfedges[edge_id].N[q][k];
							n2 = univelement.surfedges[edge_id].N[q][w];

							/* dodanie do macierzy H warunku brzegowego */
							H_el[w][k] += alpha * n1 * n2 * DetJ;
						}

						/* dodanie do wektora obciążeń P */
						pn = alpha * ambientT * n2 * DetJ;
						P_el[w] += pn;
					}
				}
			}
			for (int z = 0; z < 4; z++) {
				for (int x = 0; x < 4; x++) {
					HC_el[z][x] += H_el[z][x] + C_el[z][x] / dTime;
				}
				PC_el[z] += P_el[z];
			}
			/* Dodawanie do macierzy globalnej - Agregacja */
			for (int ii = 0; ii < 4; ii++)
			{
				for (int jj = 0; jj < 4; jj++)
				{
					H_g[grid1.elements[e].node[ii].id][grid1.elements[e].node[jj].id] += HC_el[ii][jj];
				}
				P_g[grid1.elements[e].node[ii].id] += PC_el[ii];
			}
			
		}
		printf("------------run-%d------------\n\n", ++iteracja);
		

		printf("-------------- H ([H]+[C]/dt)-----------------------\n");
		for (int i = 0; i < nW*nH; i++) {
			for (int j = 0; j < nW*nH; j++) {
				printf("%.3f ", H_g[i][j]);
			}
			printf("\n");
		}

		printf("----------------------- P ({P}+{[C]/dt}*{T0}) -------------------\n");
		for (int i = 0; i < nW*nH; i++) {
			printf("%.3f ", P_g[i]);
		}
		printf("\n");

		double** AB = new double* [nH * nW];
		for (int j = 0; j < nH * nW; j++) {
			AB[j] = new double[nH * nW + 1];
		}
		for (int i = 0; i < nH * nW; i++) {
			for (int j = 0; j < nH * nW; j++) {
				AB[i][j] = H_g[i][j];
			}
		}
		for (int i = 0; i < nH * nW; i++) {
			AB[i][16] = P_g[i];
		}

		T1 = gausIntegration(nH * nW, AB, T1);

		double temp_min = T1[0];
		double temp_max = T1[0];

		for (int i = 1; i < nH * nW; i++) {
			if (temp_min >= T1[i])temp_min = T1[i];
			if (temp_max <= T1[i])temp_max = T1[i];
		}



		printf("\n\ntmin=%.2f\ntmax=%.2f", temp_min, temp_max);


		printf("\n");

		for (int n = 0; n < nH * nW; n++) {
			grid1.nodes[n].t = T1[grid1.nodes[n].id];
		}
		//cout << "\n\n----------node.t-inside-----------\n";
		for (int s = 0; s < grid1.nE; s++)
		{
			//cout << "	node.t: " << grid1.nodes[s].t;
		}
		printf("\n...................enter..........................\n");
		getchar();
	}
			
			return 0;
	

}
		
	
		double* gausIntegration(int n, double** AB, double* X)
		{
			int i, j, k;
			double m, s;

			// eliminacja współczynników
			for (i = 0; i < n - 1; i++)
			{
				for (j = i + 1; j < n; j++)
				{
					if (fabs(AB[i][i]) < 1e-12) return NULL;
					m = -AB[j][i] / AB[i][i];
					for (k = i + 1; k <= n; k++)
						AB[j][k] += m * AB[i][k];
				}
			}

			// wyliczanie niewiadomych

			for (i = n - 1; i >= 0; i--)
			{
				s = AB[i][n];
				for (j = n - 1; j >= i + 1; j--)
					s -= AB[i][j] * X[j];
				if (fabs(AB[i][i]) < 1e-12) return NULL;
				X[i] = s / AB[i][i];
			}
			return X;
		}
