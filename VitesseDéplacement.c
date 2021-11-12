#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//fonction pour créer le fichier csv
void Fichiercsv(char * fichier, int tableau[]){
	FILE * file = fopen(fichier, "w+");
		for (int y = 0; y < 400; y++){
			for (int x = 0; x < 3000; x++){
				int p = tableau[y*400+x];
				if (x > 0) fprintf(file, ", ");
					fprintf(file, "%d", p);
			}
			fprintf(file, "\n");
		}
	fclose(file);
}

void positioni(int longueur, int epaisseur, int tableau[]){
	for (int i = 0; i < epaisseur; i++){
		for (int j = 0; j < 3000; j++){
			tableau[i*400+j] = 5;
		}
	}
}

void positiont(int longueur, int largeur, int pente, int epaisseur, double tableau[]){
	for (int i = 0; i < longueur; i++){
		int H = (pente * longueur/100) - (pente * i/100) + epaisseur;
		for (int j = 0; j < largeur; j++){
			tableau[i*largeur+j] = H;
		}
	}
}


void colonne(int epaisseur, double alpha, double tableau[]){
	int p = 900;
	double g = 9.81;
	double n = 10^14;
	double k = p*g/n;
	double B = k*sin(alpha)/2;
	double v = 0;
	double V = 10/365;
	for (int i = 0; i < epaisseur; i++){
		v = V + B*i*(2*epaisseur-i);
		tableau[i] = v;
		printf("%f\n", v);
	}
}
		

int main(int argc, char * argv[]) {
	int * vitesse = calloc(400 * 3000, sizeof (double));
	for (int i = 0; i < 400*3000; i++){
		vitesse[i]=1;
	}
	double vit[100];
	
	colonne(100, 20, vit);
    
	positioni(1500, 200, vitesse);
	
	//appel de notre fonction pour la création de notre fichier
	Fichiercsv("position-glacier.csv", vitesse);
		
    return 0;
}

