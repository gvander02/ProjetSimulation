#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> 

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

// voir s'il est normale d'avoir une augementation du volume 
double CalculVolumeFinale(){
	
	return
}

double CalculMasseFinale(){
	
	return
}

int main(int argc, char * argv[]) {
	double V = CalculVolumeInitial(1500, 300, 20, 200);
	printf("le volume initial du glacier est de %0.2e m^3\n", V); 
	
	double M = CalculMasseInitiale(1500, 300, 20, 200);
	printf("la masse initiale du glacier est de %0.2e Kg\n", M);
	return 0;
}
