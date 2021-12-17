<!DOCTYPE html>
<html>

<head>
    <meta charset="utf-8">
    <title>Modélisation de glacier</title>
    <style>
        body { background: white; font-family: sans-serif; margin: 0; padding: 25px }
    </style>
</head>

<body>
    <h1><center>Modélisation de l'évolution d'un glacier</center></h1>
    <p>Un glacier est une accumulation de glace le long d'une vallée.
	Il se forme grâce aux couches de neige qui viennent se superposer.
	Écrasée sous son propre poids, la neige se compact et rejete les substances gazeuses.
	Ainsi, elle se soude en une masse dense et homogène pour se transformer en glace. Les glaciers sont des reserves primordiales en eau douce. De nos jours, ils sont le sujet de nombreuses discussions.
	C'est pourquoi, nous avons décidé de modéliser comment et à quelle vitesse un glacier des alpes devrait fondre...</p>
	<p> Pour ce faire, nous observons le glacier d'Aletsch qui est le plus grand et le plus emblématique des alpes.
	Le code utilisé fonctionne pour n'importe quel glacier, avec ses dimensions et ses données (meteoblue). Nous avons mis à disposition les glaciers du Grand désert et de la Corbassière.
	Pour ce rapport, nous allons uniquement présenter les résultats et analyses du glacier d'Aletsch.</p>
    <div><img alt="photo du glacier d'Aletsch en été" src="glacier_aletsch.jpg"></div>
    <div style="color: #666; font-size: 13px">
        Source: <a href="http://urlr.me/ygzDp">outdooractive</a>,
        Glacier d'Aletsch
    </div>
	<p><br /><br /></p>
    <h4>Modèle pour simulation</h4>
    <p>Dans notre modèle de base, nous allons simplifier la forme du glacier par un parallélépipède. Nous gardons quand même les dimensions réelles, pour plus de réalisme. 
    La zone d'accumulation de neige transformée en glace se trouve au dessus de la zone d'ablation où la glace fond plus vite qu'il neige.
    La longueur des zones dépent de la température. Notre simulation dure 5 ans et prend des données de précipitation par jour. Nous reinitialisons la hauteur avec l'enneigement ou la pluie.
    Le modèle de fonte est appliqué et nous recommençons chaque jours.</p>
    <div><img alt="dessin" src="IMG_glacier2.jpg"height="350" width="500" align="right"></div>
    <div><img alt="dessin" src="IMG_glacier1.jpg"height="350" width="500"></div>
    
    
    <p><br /><br /></p>
    
    <h2>Résultats et analyses</h2>
    <h3>Etude de la vitesse du glacier</h3>
    <p>Les glaciers avancent selon la pente de la roche en dessous. Ces graphiques sont pertinents pour n'importe quel glacier. 
    Le mode interactif directement sur le code python permet de choisir la pente et de voir la différence sur 5 ans.
    Cette simulation de la vitesse dépend uniquement de la proximité avec la roche. 
    Nous allons négliger le contact sur les bords du glacier. La vitesse est égale sur la largeur et la longueur.
    Ainsi, la vitesse change selon la distance par rapport à la roche.
    C'est pourquoi, nous pouvons mettre notre glacier à plat.<br /></p>
    <p></p>
    <p>La vitesse de la glace pour chaque hauteur z, est donnée par :<br /></p>
    <center><strong>v(z) = V0 + 3600*24*b*z(2*H-z)</strong></center> 
    <p>Avec : <br /><strong>V0 = 10 [m/an]<br />H = 900 [m] </strong>(épaisseur du glacier)<br /><strong> b = p*g*sin(alpha)/(2*n)<br />
    p = 917 [kg/m^3]</strong> (masse volumique de la glace)<br /><strong>g = 9.81 [m/s^2]</strong> (accélération de pesenteur)<br /><strong>
    alpha = pente </strong>(radians)<br /><strong>n = 10^14 [Pascal*s]</strong>(viscosité de la glace)
    </p>
    <p>On remarque que la vitesse ne dépend pas directement de la température. 
    Cependant, l'épaisseur se modifie avec la chaleur et change par la suite sa vitesse.</p>
   
    <div><img alt="vitesse glacier avec une pente de 0.2" src="Figure_pente3.png" height="200" width="700"></div>
    <p>Dans ce premier graphique, on voit en vert l'avancée de notre glacier selon la vitesse de base et la pente du glacier d'Aletsch.
    En rouge, on observe l'état du glacier quand la glace s'effondre, lorsqu'elle ne peut plus être soutenue par la glace ou la roche en dessous.
    Finalement, les deux courbes sont simplement inversement proportionnel. Ainsi, nous considérons cette forme comme étant la forme finale après 5 années.  
    La courbe bleue serait l'avancement du glacier avec une pente de 0.2 qui est une pente moyenne de certains passsages clé pour un glacier typique.
    <br /><br />Avec une pente nulle, le glacier avance avec une vitesse d'environ 10m/an. 
    Ceci est due à la fonte de la glace sur le sol rocheux qui la fait avancer. On peut donc deviner son deplacement au pied du trait vert.</p>

    <div><img alt="vitesse glacier avec une pentede 0.5" src="Figure_pente2.png" height="200" width="700"></div>
    <p>Ici, nous observons une pente que nous considérons comme très élevée et presque maximum. 
    Elle a une valeur de 0.5 et uniquement peu de passages de la glace ont cette pente</p> 
	<p></p>
    <p>Grâce à ces graphique nous anaylsons comment la pente affecte l'avancée du glacier. 
    Dans un cas où la pente est plus forte sur le front du glacier, l'épaisseur sera plus fine et fondra plus vite.
    Ce modèle de vitesse montre la distance (en abscisse) où le glacier s'étend selon la pente (finale) et les années.
    Selon le terrain sous le glacier et la distance avec la roche, les champs de vitesse inégales provoqueraient des crevasses.
    Si nous connaissons précisément ces données, nous pouvons estimer l'endroit où les crevasses se formeraient.<br /><br /></p>
    
    
    
    
    
    
    
    
    
    
    <h3>Différence des températures entre 1985 et 2016</h3>
    <p>Mettre en graphique les deux époques permet de comparer l'évolution de température en 30 ans et ensuite comprendre d'où vient la fonte.
    Pour plus de visibilité, nous faisons une moyenne par mois sur les cinq années. 
    Pour garder un esprit critique et plus de précision, nous avons en dessous le graphique avec une moyenne par semaine.
    De ce faite, nous voyons que les températures peuvent monter au dessus du 0°C.
    En abscisse nous avons les jours et en ordonnée la température du sommet du glacier, donné en Kelvin.</p>
    
    <div><img alt="différences de temprératures entre 1985 et 2016" src="Figure_temperatures.png" height="300" width="700"></div>
    <div><img alt="différences de temprératures entre 1985 et 2016" src="Figure_temperatures2.png" height="300" width="700"></div>
    <p>On observe des différences trop légères pour présenter une réelle augmentation de la température. 
    Or, le graphique en 2016 montre des température plus ample donc plus extrême. 
    On remarque que les étés en 2016 sont plus long, car en général, au niveau de la courbe en été, la courbe bleue est plus large que celle en orange.
    Selon les températures et les précipitation nous pouvons facilement voir l'évolution à l'aide des graphiques suivant.</p>
    
    
    
    
    
    
    
    <h3>Modèle d'accumulation et d'ablation</h3>
    <p>Grâce à ce graphique, nous pouvons observer un maximum d'éléments et comprendre certains phénomènes.
    Nous pouvons voir le glacier avec sa taille réelle le long du lit rocheux.</p>
    <div><img alt="" src="Figure_glacier1.png" height="100" width="800"></div>
    <p>Si on agrandit l'image, on voit l'évolution du glacier avec sa vitesse selon sa pente moyenne.
    La courbe rouge montre le glacier qui s'est effondré sous le poids de la glace.
    Le volume perdu dans le décalage du glacier est gagné avec l'accumulation en haut du glacier.
	Ainsi, si on calcul le volume totale sous la courbe rouge (avec la largeur) on ne ferait que d'augementer.
	Nous ne considérons pas le modèle de fonte sur la partie effondrée.
    L'effondremet est très imprévisible et selon sa forme,
	trop de paramètres entrent en jeu pour réussir à faire une approximation.
	Ici, notre modèle veut surtout étudier la différence de volume le long du glacier.
	</p>
	<div><img alt="" src="Figure_glacier2.png" height="300" width="300"></div>
	<p>La courbe bleue claire représente le glacier initial. Nous observons la courbe bleue et rouge qui se superposent.
	La bleue vient de la fonte à cause des présipitation lorsque la température est au dessus de 0°C.
	On voit une différence d'environ 4 mètres sur la partie basse du glacier.</p>
	<div><img alt="" src="Figure_glacier3.png" height="200" width="600"></div>
	<p>Tandis que nous avons uniquement une augementation de 10 mètres au point le plus haut.</p>
	<div><img alt="" src="Figure_glacier5.png" height="300" width="400"></div>
	<p>Pour ce dernier graphique, nous avons utilisé une formule physique pour analyser le comportement de la fonte selon la chaleur en surface.
	La formule qui suit prend les données d'enneigements à chaque colonne d'épaisseur, applique le modèle de fonte et recommence chaque jour pour les cinq années.
	<center><strong>H(x, t) = (H(x, t-1)^2 - (2*Ct*(Ts(x, t-1)-Tr)*3600*24)/(p*CL))^(1/2)</strong></center>
	<p>Avec: <br /><strong>H</strong> (hauteur en mètre d'une colonne x au temps t)<br /><strong>Ts</strong> (température en Kelvin d'une colonne x au temps t)<br />
	<strong>Ct = 2.215 [W/(m*K)] </strong>(conductivité thermique)<br /><strong>Tr = 273 [K]<br />p = 917 [kg/m^3]<br />
	Cl = 3.3 * 10^5 [J/g] </strong>(chaleur latente)<br /></p>
	On voit une légère fonte, négligeable à côté de celle des pluies, dû à la faible quantité de température au dessus de 0°C.
	On le remarque sur le graphique des températures(1.2). La fonte est de l'ordre de 0.005 mètre au maximum, soit à la base du glacier</p>
    <div><img alt="" src="Figure_glacier4.png" height="300" width="900"></div>


    <h4>Visualisation en 3D</h4>
	<div><img alt="glacier 3D initial" src="Figure_glacier3d2.png" height="450" width="900"></div>
    <div><img alt="glacier 3D après 5 ans" src="Figure_glacier3d1.png" height="450" width="900"></div>
    <h2>Calculs et scénario</h2>
    <p>Selon le modèle de base sur python, le volume du glacier diminue de: 2.703 * 10^10 [m^3] à 2.699 * 10^10 [m^3], 
    la masse totale du glacier diminue de: 2.479 3 10^13 [Kg] à 2.475 * 10^13 [Kg], ce qui correspond à une baisse de: 0.00162% sur 5 années.
	Nous avons fait un scénario qui simule aussi la fonte du même glacier environ 80 ans plus tard, 
	avec des températures qui devrait augementer et des vagues de chaleurs plus longues et plus intenses
	Le débit moyen est de: 29581.28 [m^3/j], le débit moyen de notre scénario est de: 148967.72 [m^3], 
	soit une augementation de 5.03%.
	Mettons un lac de 15,5m de profondeur et une superficie de 3,2 [km^2], ces dimensions sont prises pour un lac moyen de glacier.
	En considérant comme unique affluent l'eau venant des glaciers, le niveau du lac augemente de 6.81 [m] au bout de 5ans.
	Le volume final, avec le scénario est de: 2.677 * 10^10 [m^3], il y a donc une diminution de 0.00968%. 
	Soit 2.18 * 10^10 [m^3] de moins que selon notre modèle de base</p>
    
    
    <h2>Conclusion</h2>
    <p>Nous avons pu observer...</p>
    
    <p>Van der Bruggen Gaetan, Bojaly Alberto</p>
</body>

</html>































































































#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// lire les donnees du fichier csv (jour, precipitation, temperature), et les mettre dans un tableau malloc
_Bool lire_donnees_csv(char * nom_csv, double * values, int colonnes, int lignes){
    FILE * file = fopen(nom_csv, "r");
    if (file == NULL){
        fprintf(stderr, "File %s not found.\n", nom_csv);
        return false;
    }
    int y = 0;
    char buffer[100000];
    //while(fgets(buffer, 100000, file));
    while (fgets(buffer, 100000, file) != NULL){
        int x=0;
        char * start = buffer;
        while (true) {
            values[y*colonnes + x]= atof(start);
            start=strchr(start, ',');
            if (start == NULL) break;
            start+=1;
            x+=1;
            if (x>= colonnes) break;
        } 
        y+=1;
        if (y>= lignes) break;
    }
    fclose(file);
    return true;
}

double debit(double differences[], int largeur, int l_incr, int longueur){
    // calcul du débit pour chaque position x,y, sachant que nous considérons que 
	//l'altitude pour chaque couple (x,y) est la même, (L correspond a la 
	//longueur du glacier choisie dans le code python)
    double debit_total = 0.0;
    for(int i=0; i<l_incr; i++){
        double diff = 0; 
        diff = differences[3*l_incr + i]*largeur*longueur/l_incr;
        debit_total += diff;
    } 
    return debit_total;
}

double scenario_debit(int l_incr, int largeur, double * Hauteur, double * hauteur, double * Variation_neige, int temps, int longueur){
	double Debit_scenario = 0.0;
    for(int i=0; i<l_incr; i++){
		if ( Variation_neige[i] > 0 ){
			Debit_scenario += (Hauteur[i] + Variation_neige[i] - hauteur[i])*largeur*longueur/l_incr;
		}else{
			Debit_scenario += (Hauteur[i]-hauteur[i])*largeur*longueur/l_incr;
		}
    }
    return Debit_scenario/temps;
}

_Bool scenario_temp(double * values, double * temperature_scenario, int temps){
    //scenario 1: effet d'un changement de temperature sur chaque jours
    //selon des articles les vagues de chaleures seront plus intense et plus longues.
	//fonction qui fait un changement de la temperature d'une valeur aleatoire entre -1 et +3 par jour
    srand(time(NULL));
    for(int i = 0; i<temps; i++){
        double randDomain = RAND_MAX + 1.0;
        int ajout = (int) (rand() / randDomain * 5 - 1);
        temperature_scenario[i] = values[2*i+3] + 273.0 + ajout;
	}
	return true;
}

_Bool scenario_temp2(double * temperature_scenario, int tps){
	//on considere une vague de chaleur à partir de 0 degré pour une altitude a plus de 3600m
	//pour pas avoir des mesures exagérées nous faisons deux niveau de vague de chaleur
	srand(time(NULL));
	int temps = tps;
	while (temps > 1){	
		double randDomain = RAND_MAX + 1.0;
        int ajout = (int) (rand() / randDomain * 5);
		if (temperature_scenario[tps-temps] > 273 && temperature_scenario[tps-temps] < 276){
			for (int j = 0; j<10; j++){
				if (temperature_scenario[tps-temps+j]<273){ 
					temperature_scenario[tps-temps+j] = temperature_scenario[tps-temps] + 0.5*ajout;
				}else{ 
					temperature_scenario[tps-temps+j] = temperature_scenario[tps-temps+j] + 0.5*ajout;
				}
			}
        }
        temps -= 10;
        if (temperature_scenario[tps-temps] > 276){
			for (int j = 0; j<10; j++){
				if (temperature_scenario[tps-temps+j]<276){ 
					temperature_scenario[tps-temps+j] = temperature_scenario[tps-temps] + 0.5*ajout;
				}else{ 
					temperature_scenario[tps-temps+j] = temperature_scenario[tps-temps+j] + 0.5*ajout;
				}
			}
			temps -= 10;
        }
        temps -= 1;
    }
    
    return true;
}

_Bool modele_fonte(double * hauteur, double * temperature, int time, int l_incr, double pente, double * Hauteur, int H, double * precipitation){
	// l'enneigement initial ne change pas lorsque la temperature change 
	// donc on prend les valeurs de la hauteur calculee dans le code python 
	// auquel on applique le modele de fonte pour calculer la hauteur finale
	// calcul de la fonte avec les nouvelles valeurs de la temperature
    double Tr = 273.0; //temperature de la roche
    double Dt = 2.215; // diffusion thermique 
    double p = 917.0; // masse volumnique glace
    double cl = 0.33*pow(10,6); // chaleur latente de fusion de la glace 
    double temperature_h[l_incr];
    for (int i = 0; i<time; i++){
		for (int j = 0; j<l_incr; j++){
            temperature_h[j]=temperature[i]+(6.5*pente*j)/1000;
            if (temperature_h[j]>Tr){
				double pluie = precipitation[i];
                double diff= pow((hauteur[j]-pluie),2)-(2*Dt*(temperature_h[j]-Tr)*3600*24)/(p*cl);
                double fonte = sqrt(diff);
                hauteur[j]=fonte;
            }
        }
    }
    for (int i = 0; i<l_incr; i++){
		hauteur[i] = hauteur[i] - H + Hauteur[i];
	}
    return true;
}

double VolumeInitial(int longueur, int largeur, double pente, int epaisseur){
	double alpha = atan(pente/100);
	double longueurglace = longueur/cos(alpha);
	double volume = longueurglace * epaisseur * largeur;
	return volume;
}

double MasseInitiale(int longueur, int largeur, double pente, int epaisseur){
	double massevolumique = 917;
	return massevolumique*VolumeInitial(longueur, largeur, pente, epaisseur);
}

double VolumeFinal(double tableau[], int l_incr, int longueur, int largeur, double pente, int epaisseur){
	double resultat = 0.0;
	for (int i = 0; i<l_incr; i++){
		double x = tableau[i];
		resultat += x*largeur*longueur/l_incr;
	}
	return VolumeInitial(longueur, largeur, pente, epaisseur)-resultat;
}

double MasseFinale(double tableau[], int l_incr, int longueur, int largeur, double pente, int epaisseur){
	double massevolumique = 917;
	return massevolumique*VolumeFinal(tableau, l_incr, longueur, largeur, pente, epaisseur);
}

void Fichiercsv(char * fichier, double tableau[], int colonnes, int lignes){
	FILE * file = fopen(fichier, "w+");
		for (int y = 0; y < lignes; y++){
			for (int x = 0; x < colonnes; x++){
				double p = tableau[y*colonnes+x];
				if (x > 0) fprintf(file, ", ");
					fprintf(file, "%f", p);
			}
			fprintf(file, "\n");
		}
	fclose(file);
}

double niveau_lac(int jours, double debit, double surface, double niveau_i, int tps){
    // evolution par jour du niveau du lac
    // nous avons mis des dimensions au hazard
    double niveau = niveau_i;
    double affluent = debit/(3600*24);
    for(int i=0; i< tps; i++){
        double effluent = pow(niveau-15.01,2)*250*3600*24; //m^3/j
        double diff_vol = (affluent-effluent);
        niveau = niveau + diff_vol/surface;
    }
	return niveau;
}

_Bool Masse_norm(){
	for(int i=0; i< 100; i++){
	}
	return true;
}	

int main(){
	int L = 20000;
	int L_incr = 2000;
    int Larg = 1500;
    int P = 5;
    int H = 900;
    int tps = 1827;
		
	//tableau des données du code python
	double * Valeursdiff = malloc(4 * L_incr * sizeof (double));
    lire_donnees_csv("donnees_fonte.csv", Valeursdiff, L_incr, 4);
    
    //tableau avec les donnees meteoblue
    double * values = malloc(2*tps * sizeof (double));
    lire_donnees_csv("donnees_finales.csv", values, 2, tps);
	printf("%f", values[3]);
	double V = VolumeInitial(L, Larg, P, H);
	double M = MasseInitiale(L, Larg, P, H);
	double Vf = VolumeFinal(Valeursdiff, L_incr, L, Larg, P, H);
	double Mf = MasseFinale(Valeursdiff, L_incr, L, Larg, P, H);
	double pourcentageM = 1-Mf/M;
	
	printf("Selon le modele de base sur python, le volume du glacier diminue de: %.3em^3, a: %.3em^3, ", V, Vf);
	printf("la masse totale du glacier diminue de: %0.3eKg3, a: %0.3eKg, ce qui correspond a une baisse de: %f%c sur 5 annees.\n\n", M, Mf, pourcentageM, 37);
    printf("Nous avons fait un scenario qui simule aussi la fonte du meme glacier environ 80 ans plus tard,");
    printf(" avec des temperatures qui devrait augementer et des vagues de chaleurs plus longues et plus intenses\n\n");
    // somme des diff et diviser par nb de jours donne la moyenne
    float moyenne_debit_jours = debit(Valeursdiff, Larg, L_incr, L)/(tps);
    printf("Le debit moyen par jour est de: %0.2f m^3, ",moyenne_debit_jours);
    
    double temperature_scenario[tps];
    double precipitation[tps];
	for (int i=0;i<tps;i++){
		precipitation[i] = values[2*i+2]/1000;
	}
    scenario_temp(values, temperature_scenario, tps);
    scenario_temp2(temperature_scenario, tps);
    
    // faire un tableau avec la hauteur initiale et l'enneigement
    double Variation_neige[L_incr];
    double hauteur[L_incr];
    double Hauteur[L_incr];
    for(int i=0;i<L_incr;i++){
		Variation_neige[i]=Valeursdiff[2*L_incr+i];
        hauteur[i]= H;
		Hauteur[i]=Valeursdiff[1*L_incr+i];
    }
    // calculer la hauteur apres avoir applique le modele de fonte
    modele_fonte(hauteur, temperature_scenario, tps, L_incr, P, Hauteur, H, precipitation);
    
	double debit_scen = scenario_debit(L_incr, Larg, Hauteur, hauteur, Variation_neige, tps, L);
	double pourcentageD = debit_scen/moyenne_debit_jours;
    printf("le debit moyen de notre scenario par jour est de: %0.2f m^3, soit une augementation de %f%c.\n\n", debit_scen, pourcentageD, 37);
    
    //tableau des differences avec le nouveau modele de fonte
    double difference_scen[L_incr];
    for (int i=0; i<L_incr; i++){
		difference_scen[i] = Hauteur[i] - hauteur[i];
	}
    double VolumeFinal_scen = VolumeFinal(difference_scen, L_incr, L, Larg, P, H);
    double pourcentageVs = 1-VolumeFinal_scen/V;
    double Vfs = Vf - VolumeFinal_scen;
    printf("Le volume final, avec le scenario augementant la temperature et avec des vagues de chaleur, est de: %0.3em^3, il y a donc une diminution de %f%c.", VolumeFinal_scen, pourcentageVs, 37);
    printf(" Soit %0.2em^3 de moins que selon notre modele de base\n", Vfs);
    
    Fichiercsv("Vale.csv", difference_scen, L_incr, 1);
	free(Valeursdiff);
    
    return 0;
}








































#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// lire les donnees du fichier csv (jour, precipitation, temperature), et les mettre dans un tableau malloc
_Bool lire_donnees_csv(char * nom_csv, double * values, int colonnes, int lignes){
    FILE * file = fopen(nom_csv, "r");
    if (file == NULL){
        fprintf(stderr, "File %s not found.\n", nom_csv);
        return false;
    }
    int y = 0;
    char buffer[100000];
    //while(fgets(buffer, 100000, file));
    while (fgets(buffer, 100000, file) != NULL){
        int x=0;
        char * start = buffer;
        while (true) {
            values[y*colonnes + x]= atof(start);
            start=strchr(start, ',');
            if (start == NULL) break;
            start+=1;
            x+=1;
            if (x>= colonnes) break;
        } 
        y+=1;
        if (y>= lignes) break;
    }
    fclose(file);
    return true;
}

double debit(double differences[], int largeur, int l_incr, int longueur){
    // calcul du débit pour chaque position x,y, sachant que nous considérons que 
	//l'altitude pour chaque couple (x,y) est la même, (L correspond a la 
	//longueur du glacier choisie dans le code python)
    double debit_total = 0.0;
    for(int i=0; i<l_incr; i++){
        double diff = 0; 
        diff = differences[3*l_incr + i]*largeur*longueur/l_incr;
        debit_total += diff;
    } 
    return debit_total;
}

double scenario_debit(int l_incr, int largeur, double * Hauteur, double * hauteur, double * Variation_neige, int temps, int longueur){
	double Debit_scenario = 0.0;
    for(int i=0; i<l_incr; i++){
		if ( Variation_neige[i] > 0 ){
			Debit_scenario += (Hauteur[i] + Variation_neige[i] - hauteur[i])*largeur*longueur/l_incr;
		}else{
			Debit_scenario += (Hauteur[i]-hauteur[i])*largeur*longueur/l_incr;
		}
    }
    return Debit_scenario/temps;
}

_Bool scenario_temp(double * values, double * temperature_scenario, int temps){
    //scenario 1: effet d'un changement de temperature sur chaque jours
    //selon des articles les vagues de chaleures seront plus intense et plus longues.
	//fonction qui fait un changement de la temperature d'une valeur aleatoire entre -1 et +3 par jour
    srand(time(NULL));
    for(int i = 0; i<temps; i++){
        double randDomain = RAND_MAX + 1.0;
        int ajout = (int) (rand() / randDomain * 5 - 1);
        temperature_scenario[i] = values[2*i+3] + 273.0 + ajout;
	}
	return true;
}

_Bool scenario_temp2(double * temperature_scenario, int tps){
	//on considere une vague de chaleur à partir de 0 degré pour une altitude a plus de 3600m
	//pour pas avoir des mesures exagérées nous faisons deux niveau de vague de chaleur
	srand(time(NULL));
	int temps = tps;
	while (temps > 1){	
		double randDomain = RAND_MAX + 1.0;
        int ajout = (int) (rand() / randDomain * 5);
		if (temperature_scenario[tps-temps] > 273 && temperature_scenario[tps-temps] < 276){
			for (int j = 0; j<10; j++){
				if (temperature_scenario[tps-temps+j]<273){ 
					temperature_scenario[tps-temps+j] = temperature_scenario[tps-temps] + 0.5*ajout;
				}else{ 
					temperature_scenario[tps-temps+j] = temperature_scenario[tps-temps+j] + 0.5*ajout;
				}
			}
        }
        temps -= 10;
        if (temperature_scenario[tps-temps] > 276){
			for (int j = 0; j<10; j++){
				if (temperature_scenario[tps-temps+j]<276){ 
					temperature_scenario[tps-temps+j] = temperature_scenario[tps-temps] + 0.5*ajout;
				}else{ 
					temperature_scenario[tps-temps+j] = temperature_scenario[tps-temps+j] + 0.5*ajout;
				}
			}
			temps -= 10;
        }
        temps -= 1;
    }
    
    return true;
}

_Bool modele_fonte(double * hauteur, double * temperature, int time, int l_incr, double pente, double * Hauteur, int H, double * precipitation){
	// l'enneigement initial ne change pas lorsque la temperature change 
	// donc on prend les valeurs de la hauteur calculee dans le code python 
	// auquel on applique le modele de fonte pour calculer la hauteur finale
	// calcul de la fonte avec les nouvelles valeurs de la temperature
    double Tr = 273.0; //temperature de la roche
    double Dt = 2.215; // diffusion thermique 
    double p = 917.0; // masse volumnique glace
    double cl = 0.33*pow(10,6); // chaleur latente de fusion de la glace 
    double temperature_h[l_incr];
    for (int i = 0; i<time; i++){
		for (int j = 0; j<l_incr; j++){
            temperature_h[j]=temperature[i]+(6.5*pente*j)/1000;
            if (temperature_h[j]>Tr){
				double pluie = precipitation[i];
                double diff= pow((hauteur[j]-pluie),2)-(2*Dt*(temperature_h[j]-Tr)*3600*24)/(p*cl);
                double fonte = sqrt(diff);
                hauteur[j]=fonte;
            }
        }
    }
    for (int i = 0; i<l_incr; i++){
		hauteur[i] = hauteur[i] - H + Hauteur[i];
	}
    return true;
}

double VolumeInitial(int longueur, int largeur, double pente, int epaisseur){
	double alpha = atan(pente/100);
	double longueurglace = longueur/cos(alpha);
	double volume = longueurglace * epaisseur * largeur;
	return volume;
}

double MasseInitiale(int longueur, int largeur, double pente, int epaisseur){
	double massevolumique = 917;
	return massevolumique*VolumeInitial(longueur, largeur, pente, epaisseur);
}

double VolumeFinal(double tableau[], int l_incr, int longueur, int largeur, double pente, int epaisseur){
	double resultat = 0.0;
	for (int i = 0; i<l_incr; i++){
		double x = tableau[i];
		resultat += x*largeur*longueur/l_incr;
	}
	return VolumeInitial(longueur, largeur, pente, epaisseur)-resultat;
}

double MasseFinale(double tableau[], int l_incr, int longueur, int largeur, double pente, int epaisseur){
	double massevolumique = 917;
	return massevolumique*VolumeFinal(tableau, l_incr, longueur, largeur, pente, epaisseur);
}

double niveau_lac(int jours, double debit, double debit_scen, double surface, double niveau_i){
    // evolution par jour du niveau du lac
    // nous avons mis des dimensions au hazard
    double niveau = niveau_i;
    double affluent = debit_scen;
    double effluent = debit;
    double diff_volume = (affluent-effluent)*jours;
    return niveau + diff_volume/surface;
}

int main(){
	int L_incr = 2000;

	//tableau des données du code python
	double * Valeursdiff = malloc(5 * L_incr * sizeof (double));
    lire_donnees_csv("donnees_fonte.csv", Valeursdiff, L_incr, 5);
    
    int L = Valeursdiff[4*L_incr+1];
    int Larg = Valeursdiff[4*L_incr+2];
    int P = Valeursdiff[4*L_incr+3]*100;
    int H = Valeursdiff[4*L_incr];
    int tps = 1827;
    
    //tableau avec les donnees meteoblue
    double * values = malloc(2*tps * sizeof (double));
    lire_donnees_csv("donnees_finales.csv", values, 2, tps);
	double V = VolumeInitial(L, Larg, P, H);
	double M = MasseInitiale(L, Larg, P, H);
	double Vf = VolumeFinal(Valeursdiff, L_incr, L, Larg, P, H);
	double Mf = MasseFinale(Valeursdiff, L_incr, L, Larg, P, H);
	double pourcentageM = 1-Mf/M;
	float moyenne_debit_jours = debit(Valeursdiff, Larg, L_incr, L)/(tps);
	
	printf("Selon le modele de base sur python, le volume du glacier diminue de: %.3em^3, a: %.3em^3, ", V, Vf);
	printf("la masse totale du glacier diminue de: %0.3eKg3, a: %0.3eKg, ce qui correspond a une baisse de: %f%c sur 5 annees.\n\n", M, Mf, pourcentageM, 37);
    printf("Nous avons fait un scenario qui simule aussi la fonte du meme glacier environ 80 ans plus tard,");
    printf(" avec des temperatures qui devrait augementer et des vagues de chaleurs plus longues et plus intenses\n\n");
    printf("Le debit moyen par jour est de: %0.2f m^3, ",moyenne_debit_jours);
    
    double temperature_scenario[tps];
    double precipitation[tps];
	for (int i=0;i<tps;i++){
		precipitation[i] = values[2*i+2]/1000;
	}
    scenario_temp(values, temperature_scenario, tps);
    scenario_temp2(temperature_scenario, tps);
    
    // faire un tableau avec la hauteur initiale et l'enneigement
    double Variation_neige[L_incr];
    double hauteur[L_incr];
    double Hauteur[L_incr];
    for(int i=0;i<L_incr;i++){
		Variation_neige[i]=Valeursdiff[2*L_incr+i];
        hauteur[i]= H;
		Hauteur[i]=Valeursdiff[1*L_incr+i];
    }
    // calculer la hauteur apres avoir applique le modele de fonte
    modele_fonte(hauteur, temperature_scenario, tps, L_incr, P, Hauteur, H, precipitation);
    
	double debit_scen = scenario_debit(L_incr, Larg, Hauteur, hauteur, Variation_neige, tps, L);
	double pourcentageD = debit_scen/moyenne_debit_jours;
    printf("le debit moyen de notre scenario par jour est de: %0.2f m^3, soit une augementation de %f%c.\n\n", debit_scen, pourcentageD, 37);
    
    // somme des diff et diviser par nb de jours donne la moyenne
    double niveau = niveau_lac(tps, moyenne_debit_jours, debit_scen, 32000000, 15.5);
    double delta_niveau = niveau - 15.5; 
    printf("Admettons qu'il y ai un lac de 15,5m de profondeur et une superficie de 3,2km^2, Ces dimensions sont prises pour un lac moyen de glacier. En considerant comme unique affluent l'eau venant des glaciers, le niveau du lac augemente de %f m en 5ans.\n\n", delta_niveau);
    
    
    //tableau des differences avec le nouveau modele de fonte
    double difference_scen[L_incr];
    for (int i=0; i<L_incr; i++){
		difference_scen[i] = Hauteur[i] - hauteur[i];
	}
    double VolumeFinal_scen = VolumeFinal(difference_scen, L_incr, L, Larg, P, H);
    double pourcentageVs = 1-VolumeFinal_scen/V;
    double Vfs = Vf - VolumeFinal_scen;
    printf("Le volume final, avec le scenario augementant la temperature et avec des vagues de chaleur, est de: %0.3em^3, il y a donc une diminution de %f%c.", VolumeFinal_scen, pourcentageVs, 37);
    printf(" Soit %0.2em^3 de moins que selon notre modele de base\n\n", Vfs);
   	free(Valeursdiff); 
    return 0;
}

