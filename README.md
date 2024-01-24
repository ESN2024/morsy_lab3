# ESN 12 : ACCELOROMETER

## Introduction 
Le but de cette séance de laboratoire est de récupérer les valeurs de l'accéléromètre ADXL345 qui mesure les accélérations sur les axes X, Y et Z dans l'espace. Ces valeurs seront affichées sur l'afficheur 7-segment de la carte DE10-Lite. Lorsque le bouton poussoir est appuyé, les valeurs affichées sur l'afficheur 7-segments seront respectivement l'accélération sur l'axe X, l'accélération sur l'axe Y et l'accélération sur l'axe Z.
## Architecture
![image](https://github.com/ESN2024/morsy_lab3/assets/74385250/5083f219-717c-436a-8ce0-875d8b8eb02e)

## QSYS
Tout d'abord, j'ai inclus les composants nécessaires pour reconstruire mon système de base.

* RAM
* NIOS-2
* CLOCK
* JTAG
* opencore_i2c
* timer
  
Apres, on ajoute :

* 6xPIO 4 bits pour l'aficheur 7 segment
* PIO pour le boutton_poussoir "BTN"

  ![image](https://github.com/ESN2024/morsy_lab3/assets/74385250/4c58a4cc-d4df-46b1-ba62-c3260fa16ef3)

  ![image](https://github.com/ESN2024/morsy_lab3/assets/74385250/b199ca2f-dcd0-40a4-89a2-37d9be5314ed)

## Code VHDL

Pour contrôler les afficheurs 7 segments, une méthode utilisée est d'utiliser un 6xPIO programmé en sortie de 4 bits chacccun donc en total est 24 bits. Cette approche permet de contrôler 6 afficheurs au total.

Dans cette implémentation, nous allons d'abord concevoir l'architecture matérielle du système en utilisant Platform Designer. Nous allons ensuite éditer le code VHDL pour appliquer les signaux appropriés sur les broches de la carte afin d'afficher les valeurs souhaitées sur le 7-segments. Sur QUARTUS, nous allons créer un composant appelé "BIN_2-7SEG" qui se chargera de convertir les valeurs binaires en les valeurs souhaitées à afficher sur le 7-segments (en utilisant la convertion BCD). Ce composant prend en entrée la valeur à convertir en binaire et génère en sortie la combinaison équivalente pour allumer les LEDS du 7-segments afin d'afficher le nombre souhaité. Ensuite, dans le fichier "virgule.vhd" qui chargera pour ajouter le point a l'afficheur numero 4 comme ca on peut voir la valeur du aceeleration en g et pas en mg.En outre, dans le fichier "signe.vhd" qui chargera pour gerer le signe "-" quand c'est ecrit a l'entree 1 et il affiche rien du tout quand c'ecrit a l'entree 0 comme valeur .
Enfin,dans le fichier "morsy_lab3.vhd" nous allons connecter les digits du 7-segments au composant "bin_2_7SEG","virgule","signe", permettant ainsi d'afficher les nombres souhaités sur chaque digit du 7-segments configuré.

## La recuperation des donnees du ADXL345

Avant de pouvoir accéder aux données de l'accéléromètre, il est nécessaire de configurer certains paramètres de l'ADXL345 en écrivant dans les régistres suivants:
* #define power_data 0x2c             //configuration of power_mode and transfer
* #define data_format 0x31            //configuration of data_format

Afin de récupérer des valeurs exactes des accélérations sur les axes X, Y et Z, il est nécessaire d'ajouter des valeurs d'offset aux valeurs des accélérations. En effet, les valeurs correctes devraient être exprimées en mg. Pour cela, en se référant à la datasheet du ADXL345, nous devrons:
* recuperer les valeurs du x0,y0,z0 avec un offset =0 dans les 3 registres (offset de x,offset de y , offset de z)
* puisque on est en mode full-resolution donc on peut appliquer la fonction suivante x_offset=-round(x_offset/4),y_offset=-round(y_offset/4),z_offset=-round((z_offset-1000)/4);
* écrire ensuite ces valeurs dans les régistres d'offset
* * #define off_x 0x1E                  // Hexadecimal address for the offset_x internal register
* * #define off_y 0x1F                  // Hexadecimal address for the offset_y internal register
* * #define off_z 0x20                  // Hexadecimal address for the offset_y internal register

Pour finaliser notre système, il est nécessaire d'écrire une fonction d'interruption du Timer qui sera utilisée pour récupérer les valeurs des accélérations sur les axes X, Y et Z depuis l'ADXL345 toutes les 0.5 secondes, puis les afficher sur l'afficheur 7-segments. Cette fonction permettra de mettre à jour les valeurs du x,y,z. De plus, une fonction d'interruption au niveau du boutton_poussoir qui sera également écrite pour gérer le changement des valeurs des accélérations sur l'afficheur 7-segments lorsque l'utilisateur appuie sur le bouton poussoir. Cela permettra de naviguer entre les valeurs des accélérations sur les différents axes facilement.

## Resultats 

* Axe Z

  ![image](https://github.com/ESN2024/morsy_lab3/assets/74385250/62d26f26-4c54-4551-98e9-bdc5c1d6e2b2)

* Axe X

  ![image](https://github.com/ESN2024/morsy_lab3/assets/74385250/7af9f6ce-8d4f-42c4-af69-f7fc61a30739) ![image](https://github.com/ESN2024/morsy_lab3/assets/74385250/539d17d9-dd94-4f43-a123-fcf369074d6e)


* Axe Y

![image](https://github.com/ESN2024/morsy_lab3/assets/74385250/7f6d44fa-531b-4d84-b27b-b662938b1760)

*Video


https://github.com/ESN2024/morsy_lab3/assets/74385250/82c5aa28-1185-4953-b571-7dc230140c6e

## Conclusion 

Avec ce lab, j'ai compris comment communiquer avec l'accéléromètre par le biais du bus I2C, comment le configurer. Ainsi que la réalisation de l'architecture du système.










