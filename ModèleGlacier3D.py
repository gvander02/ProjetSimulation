'''
Un glacier est une accumulation de glace le long d'une vallée.
Il se forme grâce aux couches de neige qui viennent se superposer.
Écrasée sous son propre poids, la neige se compact et rejete les substances gazeuses.
Ainsi, elle se soude en une masse dense et homogène pour se transformer en glace.

Les glaciers sont des reserves primordiales en eau douce. De nos jours, ils sont le sujet de nombreuses discussions.
C'est pourquoi, nous avons décidé de vérifier, à l'aide de nos connaissances en informatique, comment et à quelle vitesse
le plus grand glacier des alpes devrait fondre...

Nous voulons aussi que ce code soit les plus facilement modifiable pour s'appliquer sur d'autre glacier avec des données météo
et des tailles différentes.
'''

import numpy as np
import matplotlib.pyplot as plt
import math
import json
import csv

# recuperer les donnees necessaires a notre simulation du fichier meteoblue json
fichiers = ["aletsch2016.json", "aletsch2017.json","aletsch2018.json", "aletsch2019.json", "aletsch2020.json"]

def telecharger(dossier):
    with open(dossier, "r") as donnees_initiales:
        donnees_initiales = json.load(donnees_initiales)
        donnees_utiles = {"altitude": donnees_initiales.get(
            "metadata").get("height")}
        donnees_utiles.update(
            {'time': donnees_initiales.get("history_1h").get("time")})
        donnees_utiles.update(
            {'precipitation': donnees_initiales.get("history_1h").get("precipitation")})
        donnees_utiles.update(
            {'temperature': donnees_initiales.get("history_1h").get("temperature")})
    return donnees_utiles

Calculalt = telecharger(fichiers[0])

donnees_finales = {'time': []}
donnees_finales.update({'precipitation': []})
donnees_finales.update({'temperature': []})

def dictionnaire(dossier):
    doss = telecharger(dossier)
    # Somme des precipitations par jours
    for i in range(0, len(doss["precipitation"]), 24):
        somme = 0
        for j in range(i, i+24):
            somme += doss["precipitation"][j]
        donnees_finales["precipitation"].append(somme)
    # Moyenne des temperatures par jour
    for i in range(0, len(doss["temperature"]), 24):
        somme = 0
        for j in range(i, i+24):
            somme += doss["temperature"][j]
        donnees_finales["temperature"].append(somme/24)
    return donnees_finales

for i in range(len(fichiers)):
    dictionnaire(fichiers[i])

# Dimension glacier----------------------------------------------------------//
H = 900  # float(input("Quelle est l'épaisseur du glacier?"))
P = 0.05# float(input("Quelle est la pente moyenne du glacier? En pourcentage"))/100
L_incr = 2000 #précision de nos calcul -> pour une précision maximum L_incr = L (trop de calculs)
L = 20000  # int(input("Quelle est la longueur du glacier?"))#distance entre le sommet et le front du glacier sans compter l'altitud
Larg = 1500  # int(input("Quelle est la largeur du glacier?"))
tps = len(donnees_finales['temperature'])  # float(input("Temps de la simulation (en jour)"))
Alt = Calculalt["altitude"]# on suppose que le point des données est le sommet du glacier
alpha = math.atan(P)  # radians
BaseG = Alt - P*L
BaseR = Alt - P*L - H #base de la roche au niveau du glacier
delta = int(L/L_incr)

# Constantes-----------------------------------------------------------------//
V = 10/365  # m/jour
p = 917  # kg/m^3
g = 9.81  # constante pesanteur m/s^2
n = math.pow(10, 14)  # viscosité en Pascal*s
k = p*g/n
b = k*math.sin(alpha)/2
#mu = 2*b*H*n/(Patm+p*g*H)
Ct = 2.215  #Conductivité thermique
CL = 0.33*(10**6)  # Chaleur Latente
Tr = 273 + 0.0  # Température à la roche

# Modèle de vitesse, Déformation interne-------------------------------------//
'''
 la vitesse dépend uniquement de la proximité avec la roche.
 Pour notre simulation nous allons négliger le contact sur les bords du glacier.
 la vitesse est égale sur la largeur et la longueur.
 Ainsi, la vitesse change selon la distance par rapport à la roche.
 Nous pouvons mettre notre glacier à plat donc notre L devient différent
'''
L1 = L/math.cos(alpha) #longueur sur le glacier
xx = np.linspace(0, L1, L_incr)
yy = np.linspace(0, H, H)

def vitesse(y):
    v = V + b*y*(2*H-y)*3600*24
    return v

W = [0]
for i in range(L_incr-2):
    W.append(H)
W.append(0)
w = np.array(W)

vv = []
for i in range(H):
    vv.append(vitesse(i))

deplacement = []
deplacement2 = []
for i in range(H-1):
    deplacement.append(L+ tps*vv[i])
    deplacement2.append(L1 + tps*vv[i])
deplacement2.append(L1)

vv1 = []
for i in range(H):
    vv1.append(vitesse(H-i))

deplacement1 = []
deplacement12 = []
for i in range(H-1):
    deplacement1.append(L + tps*vv1[i])
    deplacement12.append(L1 + tps*vv1[i])
deplacement12.append(L1)

fig = plt.figure()
ax = fig.add_subplot(111)
ax.set_aspect('equal', adjustable='box')
ax.set_ylim(0, H + 600)
plt.plot(xx, w, label="Glacier à plat", color = 'lightblue')
plt.plot(deplacement2, yy, ':', label=f"Glacier après {tps} jours sans effondrement", color = 'green')
plt.plot(deplacement12, yy, label=f"Glacier après {tps} jours et après effondrement", color = 'red')
plt.xlabel("longueur [m]")
plt.ylabel("hauteur [m]")
plt.legend()

    
#Modele d'enneigement, Accumulation---------------------------------------------------//
Coeff = 0.0001  # coeff d'enneigement par rapport à l'altitude (plus réaliste)
dp = int(deplacement2[-2])
dp1 = int(deplacement12[-2])
BaseGfinal = BaseG - P*(deplacement12[-1]-L)
BaseRfinal = BaseR - P*(deplacement12[0]-L)
"""
valeurs enneigement par jour:
la hauteur de neige qui tombe a un coefficient de 10 par rapport à la précipitation(eau).
Pour passer de millimètre en mètre nous devons uniquement le diviser par 100
"""
Valeursprecipitation = []
for i in range(len(donnees_finales["precipitation"])):
    Valeursprecipitation.append(donnees_finales["precipitation"][i]/100)

Ts = []  #Température surface
for i in range(tps):
    TempJ = []
    for j in range(L_incr):
        TempJ.append(273 + donnees_finales["temperature"][i] + 6.5*P*j*(L/L_incr)/1000)
    Ts.append(TempJ)

E = []  #on commence l'année avec un enneigement initiale de (2m si en hiver)
for i in range(L_incr):
    E.append(1.0)

nbrsj = 4 #nbrs jour pour transformer en glace:
for i in range(tps-nbrsj):
    for j in range(L_incr):
        if Ts[i][j] <= 273:
             valeur = Valeursprecipitation[i]/10 + E[j] - Coeff*P*j*delta #max de 0.1m pour L = 20000
             if valeur > 0:
                 del E[j]
                 E.insert(j, valeur)
        else:
            valeur = E[j] - Valeursprecipitation[i]/10
            del E[j]
            E.insert(j, valeur)
                    
for i in range(nbrsj):
    for j in range(L_incr):
        if Ts[i][j] <= 273:
            valeur = Valeursprecipitation[tps-nbrsj+i] + E[j] - Coeff*P*j*delta
            if valeur > 0:
                del E[j]
                E.insert(j, valeur)
        else:
            valeur = E[j] - Valeursprecipitation[i] /10
            del E[j]
            E.insert(j, valeur)

#Graphique 2D----------------------------------------------------------------//
'''
Pour voir le rectangle et pas juste la fonction on utilise une liste, pas comme le graph de y
'''
# hauteur glacier au temps t0
Z = [BaseR + P*L]
for i in range(L_incr-2):
    Z.append(P*L-P*i*delta + BaseG)
Z.append(BaseR)

# niveau de la neige
Varneige = []
VraiZ =[]
htt = []
for i in range(L_incr):
    VraiZ.append(P*L-P*i*delta + BaseG)
for i in range(L_incr):
    Varneige.append(E[i]+VraiZ[i])
    htt.append(E[i]+H)

# Modele de fonte, Ablation--------------------------------------------------// 
'''
Nous ne considérons pas le modèle de fonte sur la partie effondrer
le volume est remplacé par l'accumulation en haut du glacier.
L'effondremet est très imprévisible et selon sa forme,
trop de paramètres rentrent en jeu pour réussir à faire une faible approximation.
Notre modèle veut surtout étudier la différence de volume le long du glacier.
'''    
for i in range(tps):
    for j in range(L_incr):
        if Ts[i][j] > Tr:
            hauteur = htt[j]**(2) - (2*Ct*(Ts[i][j]-Tr)*3600*24)/(p*CL)
            fonte = hauteur**(1/2)
            del htt[j]
            htt.insert(j, fonte) 

for i in range(L_incr):
    htt[i] = htt[i] - H + VraiZ[i]

Pentefinal = (htt[-2]-htt[-1])/delta
PenteF = math.atan(Pentefinal) # en radians
Xmin = int(math.cos(PenteF)*(dp1-L1)/delta)
Xmax = int(math.cos(PenteF)*(dp-L1)/delta)

ht = [Z[0]]
for i in range(1,L_incr):
    ht.append(htt[i])
dernier_point = ht[-1]
for i in range(1,Xmin + 1):
    ht.append(dernier_point - Pentefinal*i*delta)
    
pente_v = [] # pour le modele sans effondremment
for i in range(1,Xmax+1):
    pente_v.append(dernier_point - Pentefinal*i*delta)

haut = int(math.sin(alpha)*V*tps/2) #hauteur après un déplacement de vitesse initiale

x = np.linspace(0, L, L_incr)
xxx = np.linspace(0, L+Xmax*delta + delta, Xmax + L_incr)
yyy = np.linspace(BaseR-haut, pente_v[-1], H-1)
xxx1 = np.linspace(0, L+Xmin*delta, Xmin + L_incr)
yyy1 = np.linspace(BaseRfinal, int(ht[-1]), H-1)
xxx2 = np.linspace(L, L+Xmax*delta, Xmax)

# niveau du sol y
y = P*L-P*xxx + BaseR

# taille égale des axes
fig = plt.figure()
ax = fig.add_subplot(111)
ax.set_aspect('equal', adjustable='box')
plt.plot(x, Varneige, label="Enneigement", color='darkblue')
plt.plot(x, Z, label="Glacier au temps t0", color='lightblue')
plt.plot(xxx, y, label="Roche", color="black")
plt.plot(xxx1, ht, label="Glacier au temps t", color="red")
plt.plot(deplacement1, yyy1, color="red")
plt.plot(deplacement, yyy, ':', label = f'glacier après {tps} jours', color="green")
plt.plot(xxx2, pente_v, ':', color = 'green')
plt.xlabel("longueur [m]")
plt.ylabel("hauteur [m]")
plt.legend()


# Graphique 3D---------------------------------------------------------------//
# Création du graphique 3D---------------------------------------------------//
x1 = np.linspace(-100, L+600, L_incr+70)
x2 = np.linspace(0, Larg+200, Larg+200)
X1, X2 = np.meshgrid(x1, x2)

xx1 = np.linspace(0, L, L_incr)
xx2 = np.linspace(100, Larg+100, Larg)
XX1, XX2 = np.meshgrid(xx1, xx2)

# niveau du sol
def roche(x, y):
    return P*L-P*x + BaseR

# niveau du glacier
def glacier(x, y):
    return Alt - P*x

Zr = roche(X1, X2)
Zg = glacier(XX1, XX2)

#importation du modèle 2D----------------------------------------------------//
for i in range(len(Zg)):
    Zg[i][0] = Zr[i][10]
    Zg[i][-1] = Zr[i][L_incr]

for i in range(len(Zg[1])):
    Zg[0][i] = Zr[0][i]
    Zg[-1][i] = Zr[Larg+100][i]

ZgT = []
for i in range(Larg):
    ZgT.append(ht)
Zgt = np.array(ZgT)

for i in range(len(ZgT)):
    Zgt[i][0] = Zr[i][10]
    
for i in range(L_incr):
    Zgt[0][i] = Zr[0][i]
    Zgt[-1][i] = Zr[0][i]

ZgT2 = []
for i in range(Larg):
    ZgT2.append(deplacement1)
Zgt2 = np.array(ZgT2)


xxx2 = np.linspace(100, Larg+100, Larg)
xxx1 = np.linspace(0, L+Xmin*delta, Xmin + L_incr)
XXX1, XXX2 = np.meshgrid(xxx1, xxx2)

yyy1 = np.linspace(100, Larg+100, Larg)
yyy2 = np.linspace(BaseR-haut, pente_v[-1], H-1)
YYY1, YYY2 = np.meshgrid(yyy2, yyy1)

fig = plt.figure()
ax = plt.axes(projection='3d')
ax.set_xlim(0, L + 500)
ax.set_ylim(-300, Larg + 300)
ax.set_zlim(BaseR-200, Alt+100)
tailleGt = 5
tailleR = 0.5
tailleG = 2
ax.plot_wireframe(X1, X2, Zr, tailleR, color='black')
ax.plot_wireframe(XX1, XX2, Zg, tailleR, color='lightblue')
ax.plot_wireframe(XXX1, XXX2, Zgt, tailleG, color='red')
ax.plot_wireframe(Zgt2, YYY2, YYY1, tailleG, color='red')
plt.show()

# Envoyer les données csv pour le calcul en C.-------------------------------//
liste_difference = []
liste_diffinal = []
for i in range(L_incr):
    diffinal = VraiZ[i] - htt[i]
    liste_diffinal.append(diffinal)
    
    if Z[i]<Varneige[i]:
        diff = Varneige[i] - htt[i]
    else:
        diff = VraiZ[i] - ht[i]
    liste_difference.append(diff)
    
with open('donnees_fonte.csv', 'w') as csv_file:
    writer = csv.writer(csv_file)
    writer.writerow(liste_diffinal)#différance hauteur entre temps t0 et temps t
    writer.writerow(VraiZ)#hauteur du glacier au temps t0
    writer.writerow(E)#accumulation de neige et glace après temps t
    writer.writerow(liste_difference)#Valeurs de ce qui a fondu (ablation et pluie)

#convertir le dictionnaire python en fichier csv
#header du fichier csv (nom de chaque colonnes)
csv_columns= ['precipitation', 'temperature']
#lignes du ficher: date, precipitation e temperature moyenne par jour
dict_data = []
for i in range(len(donnees_finales["temperature"])):
    dict_data.append([donnees_finales["precipitation"][i], donnees_finales["temperature"][i]])
    
with open('donnees_finales.csv', 'w') as csv_file:
    writer = csv.writer(csv_file)
    writer.writerow(csv_columns)
    writer.writerows(dict_data)
