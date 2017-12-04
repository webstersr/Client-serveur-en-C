#ifndef TYPES_H
#define TYPES_H

// Structure pour les codes promos
typedef enum CodePromo CodePromo;
enum CodePromo {
    REDUC,SUPPL,DEFAULT
};

// Structure de l'heure de voyage
typedef struct {
    int heure;
    int minute;
}horaire;

// Structure du voyage
typedef struct{
    int numero_train;
    char* ville_depart;
    char* ville_arrive;
    horaire heure_depart;
    horaire heure_arrive;
    double prix;
    CodePromo code_promo;

}voyage;


// Liste chainée contenant la liste des voyages
struct TableauVoyage{
    voyage v;
    struct TableauVoyage* suivant;
};
typedef struct TableauVoyage TableauVoyage;



#endif