# Space Invaders - Christiane Hebey


## Bibliothèques utilisées

Utilisation de la bibliothèque graphique SDL et de l'extension SDL_ttf pour avoir une interface graphique et afficher du texte. Je n'ai pas pu utiliser SDL2 et SDL2_ttf car mon ordinateur refusait d'installer l'extension SDL2_ttf et j'ai donc dû me rabattre sur SDL. Vous trouverez dans ce rendu un fichier Arial.ttf, qui sert à pouvoir afficher la police, qui est donc la police Arial.


## Instructions de compilation

• Taper make (une seule fois suffit tant que vous ne remodifiez pas par la suite le code) 

• Taper ./space_invaders 


## Disclaimer

• Les textes affichés dans ce jeu sont écrits en écriture inclusive. Si jamais vous ne maîtrisez/connaissez pas avec cette pratique, voici un document très intéressant qui résume les tenants, aboutissements et règles de cette écriture https://inrs.ca/wp-content/uploads/2021/03/Guide-redaction-inclusive-inrs-vf.pdf. 

• Soyez prévenu, le jeu peut être légèrement agressif avec vous, et vous traiter notamment de flemmard.e ou de looseu.r.se. C'est là entièrement de la faute du jeu et non la mienne. Cependant le jeu n'hésitera pas à vous féliciter en cas de victoire ! 


## Résumé du projet

• L’objectif de ce projet était de développer un **jeu de type Space Invaders** en  **C**.

• 4 niveaux étaient proposés.

• J'ai réalisé les niveaux 1,2 et 3 entièrement, et ai entammé le niveau 4. 

• En l'état des choses, mon programme est un jeu de space invaders fonctionnel:

- Les ennemis, en nombre de 50 au début du jeu, descendent selon une vitesse verticale. 


- Le joueur tue les ennemis en leur tirant dessus, ce qui se fait en appuyant sur la touche espace. 


- Le joueur peut se déplacer vers la gauche et la droite à l'aide des flèches du clavier. 



- Les ennemis peuvent tirer, et sont répartis en quatre classes: normaux (rouge), snipers (orange), resistants (violets) et rapides (jaunes). Les ennemis resistants doivent être touchés deux fois pour mourir. Les snipers ont une plus grande proba de tirer.


- Le jeu contient un affichage du score: chaque ennemi tué vaut 1 point. Le jeu contient également un affichage des vies. Le joueur commence avec trois vies et en perd dès qu'il est touché par une balle ennemie. Cependant, le joueur a la possibilité de gagner des vies en attrapant les coeurs (carrés roses).


- Le joueur perd s'il n'a plus de vies ou si les ennemis atteignent le bas de l'écran. Un message de défaite s'affiche alors.


- Le joueur gagne s'il a tué tous les ennemis. Un message de victoire s'affiche alors.


- La console d'affichage se ferme quelques secondes après que la partie soit finie.


- Le jeu comprend également un menu proposant trois niveaux de difficulté. Chaque niveau de difficulté correspond à une vitesse de descente des ennemis de plus en plus élevée. Cette vitesse est dans tous les cas progressive au cours de la descente. 


- Il est également possible de mettre le jeu en pause. 

