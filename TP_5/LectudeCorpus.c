/**
 *  University File
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>

#define TAILLE 1024
#define TAILLE_NB_MOT 30

/* Fonction d'écriture de la répartition dans un fichier */
void ecrireFichier(char *nomFichier, long int cpt, int r[]) {

  FILE *fichOut ;
  int j ;

  
  fichOut = fopen(nomFichier, "w") ;
  if (!fichOut) {
    printf("erreur l'ouverture du fichier %s\n", nomFichier) ;
    exit(2) ;
  }
  
  //printf("Ecriture du fichier %s\n", nomFichier) ;
  fprintf(fichOut, "nombre de mots lus : %ld\n", cpt) ;
  for (j = 0 ; j < TAILLE_NB_MOT ; j++) {
    fprintf(fichOut, "%d : %d\n", j, r[j]) ;
  }
  fclose(fichOut) ;
}

/********************
* 
* Calcul de la répartition du nombre de mot dans un phrase dans un fichier texte.
* (génération de fichiers de progression)
*
******************/


int main(int argc, char* argv[]) 
{
  FILE *fichierCorpus ;
  char nomFichierGenere[100] ;
  int i, car, nbCar, nbMot, nbLignes, cptFichier ;
  long int nbMotTotal ;
  char chaine[TAILLE] ;
  int nbLignesFichier=0;

  /* Tableau pour la repartition */
  int repartition[TAILLE_NB_MOT] = {0} ;
  
  
  if (argc != 3) {
    printf("usage : %s <fichier à traiter> <nom générique fichiers résultats>\n", argv[0]) ;
    exit(1) ;
  }

  fichierCorpus = fopen(argv[1], "r") ;
  if (!fichierCorpus) {
    printf("erreur à l'ouverture du fichier %s\n", argv[1]) ;
    exit(2) ;
  }

  /* calcul du nombre de lignes total du fichier */ 
  while (fgets(chaine, TAILLE, fichierCorpus)) {
    nbLignesFichier++;
  } 
 
  fseek(fichierCorpus, 0, SEEK_SET); //on se replace au début du fichier
  
  nbCar = 0 ;
  nbMot = 0 ;
  nbMotTotal = 0;
  nbLignes = 0 ;
  cptFichier = 0 ;
  /* Traitement ligne par ligne */
  while (fgets(chaine, TAILLE, fichierCorpus)) {
    
    /* Décompte des mots */
    for (i = 0 ; i < strlen(chaine) ; i++) {
        //si c'est un caractére de fin de mot, on incrémente (sauf si successif).
        if(strchr(" ,;:.!?\n",chaine[i])!=NULL){
            if(nbCar!=0 ){
                nbCar=0;
                nbMot++;
            }
        }
        else 
            nbCar++;
    }
    //dernier mot si la ponctuation est manquante
    if(nbCar!=0)
        nbMot++;
    //printf("ligne %d : %d mots\n", nbLignes, nbMot); //pour debug
    if(nbMot<=TAILLE_NB_MOT-1)
            repartition[nbMot] ++;
    else    
            repartition[TAILLE_NB_MOT-1]++;
    nbMotTotal+=nbMot;
    nbCar=0;
    nbMot=0;
    
    /* Génération du fichier de décompte */
    nbLignes++ ;
    if (nbLignes % 10000 == 0) {
        sprintf(nomFichierGenere, "%s_%d_%d.txt", argv[2], getpid(), cptFichier) ;
        ecrireFichier(nomFichierGenere, nbMotTotal, repartition) ;  
        printf(".");fflush(stdout);
        cptFichier++ ;
    }
  }
  printf("\n");
  
  /* Ecriture dans le dernier fichier */
  sprintf(nomFichierGenere, "%s_%d_final.txt", argv[2], getpid()) ;
  ecrireFichier(nomFichierGenere, nbMotTotal, repartition) ;
  printf("[%d] Dernier fichier généré : %s [nombre de mot traités : %ld, nb lignes traitées : %d sur %d]\n", getpid(),nomFichierGenere, nbMotTotal, nbLignes, nbLignesFichier) ;

   
  fclose(fichierCorpus) ;
  return(0) ;
}
