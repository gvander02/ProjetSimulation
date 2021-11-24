#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> 

_Bool readCsv(char * filename, double * values, int sizeX, int sizeY) {
    FILE * file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "File %s not found.", filename);
		return "false";
    }

    int y = 0;
    char buffer[10000];
    while (fgets(buffer, 10000, file) != NULL) {
        int x = 0;
        char * start = buffer;
        while ("true") {
            values[y * sizeX + x] = atof(start);
            start = strchr(start, ',');
            if (start == NULL) break;
            start += 1;

            x += 1;
            if (x >= sizeX) break;
        }

        y += 1;
        if (y >= sizeY) break;
    }

    fclose(file);
    return "true";
}

double CalculVolumeInitial(int longueur, int largeur, double pente, int epaisseur){
	double alpha = atan(pente/100);
	double longueurglace = longueur/cos(alpha);
	double volume = longueurglace * epaisseur * largeur;
	return volume;
}

double CalculMasseInitiale(int longueur, int largeur, double pente, int epaisseur){
	double massevolumique = 917;
	return massevolumique*CalculVolumeInitial(longueur, largeur, pente, epaisseur);
}

double CalculVolumeFinal(double tableau[], int longueur){
	double resultat = 0.0;
	for (int i = 0; i<longueur; i++){
		double x = tableau[4*3000+i];
		resultat += x*300;
		printf("%f\n", x);
	}
	return resultat;
}

double CalculMasseFinale(){

	
	return 0;
}

void Fichiercsv(char * fichier, double tableau[]){
	FILE * file = fopen(fichier, "w+");
		for (int y = 0; y < 5; y++){
			for (int x = 0; x < 3000; x++){
				int p = tableau[y*2+x];
				if (x > 0) fprintf(file, ", ");
					fprintf(file, "%d", p);
			}
			fprintf(file, "\n");
		}
	fclose(file);
}

int main(int argc, char * argv[]){
	double * Valeursdiff = malloc(5 * 3000 * sizeof (double));
    readCsv("donnees_fonte.csv", Valeursdiff, 5, 3000);
    
    Fichiercsv("Valeursdiff.csv", Valeursdiff);
    
    double Vf = CalculVolumeFinal(Valeursdiff, 1500);
	printf("le volume final du glacier est de %0.2e m^3\n", Vf);
	
	double V = CalculVolumeInitial(1500, 300, 20, 200);
	printf("le volume initial du glacier est de %0.2e m^3\n", V); 
	
	double M = CalculMasseInitiale(1500, 300, 20, 200);
	printf("la masse initiale du glacier est de %0.2e Kg\n", M);
	
	free(Valeursdiff);
	return 0;
}
