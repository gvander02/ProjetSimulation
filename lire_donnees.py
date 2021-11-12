import json
import csv
fichier = input("Nom fichier: ")
#recuperer les donnees necessaires a notre simulation du fichier meteoblue json
with open(fichier, "r") as donnees_initiales:
    donnees_initiales = json.load(donnees_initiales)
    donnees_utiles = {"altitude": donnees_initiales.get("metadata").get("height")}
    donnees_utiles.update({ 'time': donnees_initiales.get("history_1h").get("time")})
    donnees_utiles.update({'precipitation': donnees_initiales.get("history_1h").get("precipitation")})
    donnees_utiles.update({'temperature': donnees_initiales.get("history_1h").get("temperature")})
print(donnees_utiles['altitude'])
#changement d'unite temporelle, passer d'heures en jour, et du coup ne garder que la date 
donnees_finales={'time':[]}
for i in range(0, len(donnees_utiles["time"]), 24):
    donnees_finales['time'].append(donnees_utiles["time"][i][:10])

#faire la somme des precipitations par jours
donnees_finales.update({'precipitation':[]})
for i in range(0, len(donnees_utiles["precipitation"]), 24):
    somme = 0
    for j in range(i,i+24):
        somme += donnees_utiles["precipitation"][j]
    donnees_finales["precipitation"].append(somme)

#moyenne des temperatures par jours

donnees_finales.update({'temperature': []})
for i in range(0, len(donnees_utiles["temperature"]), 24):
    somme = 0
    for j in range(i, i+24):
        somme += donnees_utiles["temperature"][j]
    donnees_finales["temperature"].append(somme/24)

#print(donnees_finales)

#convertir le dictionnaire python en fichier csv
# header du fichier csv (nom de chaque colonnes)
csv_columns= ['time', 'precipitation', 'temperature']
#lignes du ficher: date, precipitation e temperature moyenne par jour
dict_data = []
for i in range(len(donnees_finales["temperature"])):
    dict_data.append([donnees_finales["time"][i], donnees_finales["precipitation"][i], donnees_finales["temperature"][i]])


with open('donnees_finales.csv', 'w') as csv_file:
    writer = csv.writer(csv_file)
    writer.writerow(csv_columns)
    writer.writerows(dict_data)
    
        








