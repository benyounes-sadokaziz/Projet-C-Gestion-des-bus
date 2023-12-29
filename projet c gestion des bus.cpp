#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Voyageur {
    char nom[50];
    char prenom[50];
    int age;
    struct Voyageur *suivant;
} Voyageur;

typedef struct Bus {
    int numero;
    char destination[50];
    int capacite;
    int places_reservees;
    struct Bus *suivant;
} Bus;

typedef struct Reservation {
    int id;
    Bus bus;
    Voyageur voyageur;
    struct Reservation *suivant;
} Reservation;

void afficherReservation(Reservation reservation) {
    printf("\nDétails de la réservation :\n");
    printf("ID : %d\n", reservation.id);
    printf("Bus #%d - Destination : %s - Capacité : %d - Places réservées : %d\n",
           reservation.bus.numero, reservation.bus.destination, reservation.bus.capacite, reservation.bus.places_reservees);
    printf("Voyageur : %s %s - Âge : %d\n", reservation.voyageur.nom, reservation.voyageur.prenom, reservation.voyageur.age);
}

Voyageur *creerVoyageur() {
    Voyageur *nouveauVoyageur = (Voyageur *)malloc(sizeof(Voyageur));
    if (nouveauVoyageur == NULL) {
        perror("Erreur lors de l'allocation mémoire pour le voyageur");
        exit(EXIT_FAILURE);
    }

    printf("Nom du voyageur : ");
    scanf("%s", nouveauVoyageur->nom);
    printf("Prénom du voyageur : ");
    scanf("%s", nouveauVoyageur->prenom);
    printf("Âge du voyageur : ");
    scanf("%d", &nouveauVoyageur->age);
    nouveauVoyageur->suivant = NULL;

    return nouveauVoyageur;
}

Bus *creerBus() {
    Bus *nouveauBus = (Bus *)malloc(sizeof(Bus));
    if (nouveauBus == NULL) {
        perror("Erreur lors de l'allocation mémoire pour le bus");
        exit(EXIT_FAILURE);
    }

    printf("Numéro du bus : ");
    scanf("%d", &nouveauBus->numero);
    printf("Destination : ");
    scanf("%s", nouveauBus->destination);
    printf("Capacité du bus : ");
    scanf("%d", &nouveauBus->capacite);
    nouveauBus->places_reservees = 0;
    nouveauBus->suivant = NULL;

    return nouveauBus;
}

Reservation *creerReservation(int id) {
    Reservation *nouvelleReservation = (Reservation *)malloc(sizeof(Reservation));
    if (nouvelleReservation == NULL) {
        perror("Erreur lors de l'allocation mémoire pour la réservation");
        exit(EXIT_FAILURE);
    }

    nouvelleReservation->id = id;
    nouvelleReservation->bus = *creerBus();
    nouvelleReservation->voyageur = *creerVoyageur();
    nouvelleReservation->suivant = NULL;

    return nouvelleReservation;
}

void ajouterVoyageur(Voyageur **listeVoyageurs) {
    Voyageur *nouveauVoyageur = creerVoyageur();
    nouveauVoyageur->suivant = *listeVoyageurs;
    *listeVoyageurs = nouveauVoyageur;
}

void ajouterBus(Bus **listeBus) {
    Bus *nouveauBus = creerBus();
    nouveauBus->suivant = *listeBus;
    *listeBus = nouveauBus;
}

void ajouterReservation(Reservation **listeReservations) {
    int id = 1;
    if (*listeReservations != NULL) {
        Reservation *derniereReservation = *listeReservations;
        while (derniereReservation->suivant != NULL) {
            derniereReservation = derniereReservation->suivant;
            id++;
        }
    }

    Reservation *nouvelleReservation = creerReservation(id);
    nouvelleReservation->suivant = *listeReservations;
    *listeReservations = nouvelleReservation;
    nouvelleReservation->bus.places_reservees++;
    printf("Place réservée avec succès et l'id est :%d\n", id);
   
}

void afficherDetailsBus(Bus *listeBus) {
    printf("\nDétails des bus :\n");
    while (listeBus != NULL) {
        printf("Bus #%d - Destination : %s - Capacité : %d - Places réservées : %d\n",
               listeBus->numero, listeBus->destination, listeBus->capacite, listeBus->places_reservees);
        listeBus = listeBus->suivant;
    }
}

void afficherEtModifierReservation(Reservation **listeReservations) {
    int id;
    printf("Entrez l'ID de la réservation à afficher et modifier : ");
    scanf("%d", &id);

    Reservation *reservation = *listeReservations;
    while (reservation != NULL && reservation->id != id) {
        reservation = reservation->suivant;
    }

    if (reservation == NULL) {
        printf("Aucune réservation trouvée avec l'ID %d\n", id);
        return;
    }


    afficherReservation(*reservation);


    printf("Nouvelle destination pour le bus : ");
    scanf("%s", reservation->bus.destination);

    printf("La réservation a été modifiée avec succès.\n");
}

void afficherToutesReservations(Reservation *listeReservations) {
    printf("\nToutes les réservations :\n");
    while (listeReservations != NULL) {
        afficherReservation(*listeReservations);
        listeReservations = listeReservations->suivant;
    }
}

void libererMemoire(Voyageur *listeVoyageurs, Bus *listeBus, Reservation *listeReservations) {
    while (listeVoyageurs != NULL) {
        Voyageur *temp = listeVoyageurs;
        listeVoyageurs = listeVoyageurs->suivant;
        free(temp);
    }

    while (listeBus != NULL) {
        Bus *temp = listeBus;
        listeBus = listeBus->suivant;
        free(temp);
    }

    while (listeReservations != NULL) {
        Reservation *temp = listeReservations;
        listeReservations = listeReservations->suivant;
        free(temp);
    }
}

void sauvegarderVoyageurs(Voyageur *listeVoyageurs) {
    FILE *file = fopen("voyageurs.txt", "w");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier voyageurs.txt");
        return;
    }

    while (listeVoyageurs != NULL) {
        fprintf(file, "%s %s %d\n", listeVoyageurs->nom, listeVoyageurs->prenom, listeVoyageurs->age);
        listeVoyageurs = listeVoyageurs->suivant;
    }

    fclose(file);
}

void chargerVoyageurs(Voyageur **listeVoyageurs) {
    FILE *file = fopen("voyageurs.txt", "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier voyageurs.txt");
        return;
    }

    char nom[50], prenom[50];
    int age;

    while (fscanf(file, "%s %s %d", nom, prenom, &age) == 3) {
        Voyageur *nouveauVoyageur = (Voyageur *)malloc(sizeof(Voyageur));
        if (nouveauVoyageur == NULL) {
            perror("Erreur lors de l'allocation mémoire pour le voyageur");
            exit(EXIT_FAILURE);
        }

        strcpy(nouveauVoyageur->nom, nom);
        strcpy(nouveauVoyageur->prenom, prenom);
        nouveauVoyageur->age = age;

        nouveauVoyageur->suivant = *listeVoyageurs;
        *listeVoyageurs = nouveauVoyageur;
    }

    fclose(file);
}

void sauvegarderBus(Bus *listeBus) {
    FILE *file = fopen("bus.txt", "w");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier bus.txt");
        return;
    }

    while (listeBus != NULL) {
        fprintf(file, "%d %s %d %d\n", listeBus->numero, listeBus->destination, listeBus->capacite, listeBus->places_reservees);
        listeBus = listeBus->suivant;
    }

    fclose(file);
}

void chargerBus(Bus **listeBus) {
    FILE *file = fopen("bus.txt", "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier bus.txt");
        return;
    }

    int numero, capacite, places_reservees;
    char destination[50];

    while (fscanf(file, "%d %s %d %d", &numero, destination, &capacite, &places_reservees) == 4) {
        Bus *nouveauBus = (Bus *)malloc(sizeof(Bus));
        if (nouveauBus == NULL) {
            perror("Erreur lors de l'allocation mémoire pour le bus");
            exit(EXIT_FAILURE);
        }

        nouveauBus->numero = numero;
        strcpy(nouveauBus->destination, destination);
        nouveauBus->capacite = capacite;
        nouveauBus->places_reservees = places_reservees;

        nouveauBus->suivant = *listeBus;
        *listeBus = nouveauBus;
    }

    fclose(file);
}

void sauvegarderReservations(Reservation *listeReservations) {
    FILE *file = fopen("reservations.txt", "w");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier reservations.txt");
        return;
    }

    while (listeReservations != NULL) {
        fprintf(file, "%d %d %s %d %s %s %d\n", listeReservations->id,
                listeReservations->bus.numero, listeReservations->bus.destination,
                listeReservations->bus.capacite, listeReservations->voyageur.nom,
                listeReservations->voyageur.prenom, listeReservations->voyageur.age);
        listeReservations = listeReservations->suivant;
    }

    fclose(file);
}

void chargerReservations(Reservation **listeReservations) {
    FILE *file = fopen("reservations.txt", "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier reservations.txt");
        return;
    }

    int id, numero, capacite, age;
    char destination[50], nom[50], prenom[50];

    while (fscanf(file, "%d %d %s %d %s %s %d", &id, &numero, destination, &capacite, nom, prenom, &age) == 7) {
        Reservation *nouvelleReservation = (Reservation *)malloc(sizeof(Reservation));
        if (nouvelleReservation == NULL) {
            perror("Erreur lors de l'allocation mémoire pour la réservation");
            exit(EXIT_FAILURE);
        }

        nouvelleReservation->id = id;
        nouvelleReservation->bus.numero = numero;
        strcpy(nouvelleReservation->bus.destination, destination);
        nouvelleReservation->bus.capacite = capacite;
        strcpy(nouvelleReservation->voyageur.nom, nom);
        strcpy(nouvelleReservation->voyageur.prenom, prenom);
        nouvelleReservation->voyageur.age = age;

        nouvelleReservation->suivant = *listeReservations;
        *listeReservations = nouvelleReservation;
    }

    fclose(file);
}

int main() {
	FILE *fichier;
	fichier=fopen("voyageurs.txt","w");
	fichier=fopen("bus.txt","w");
	fichier=fopen("reservations.txt","w");
    Voyageur *listeVoyageurs = NULL;        
    Bus *listeBus = NULL;                   
    Reservation *listeReservations = NULL;  

    chargerVoyageurs(&listeVoyageurs);
    chargerBus(&listeBus);
    chargerReservations(&listeReservations);

    while (1) {
        printf("\nMenu :\n");
        printf("1. Créer un nouveau compte voyageur\n");
        printf("2. Créer un nouveau bus\n");
        printf("3. Créer une nouvelle réservation\n");
        printf("4. Afficher les détails des bus\n");
        printf("5. Afficher et modifier les détails d'une réservation\n");
        printf("6. Afficher toutes les réservations\n");
        printf("7. Quitter\n");

        int choix;
        printf("Choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                ajouterVoyageur(&listeVoyageurs);
                break;
            case 2:
                ajouterBus(&listeBus);
                break;
            case 3:
                ajouterReservation(&listeReservations);
                break;
            case 4:
                afficherDetailsBus(listeBus);
                break;
            case 5:
                afficherEtModifierReservation(&listeReservations);
                break;
            case 6:
                afficherToutesReservations(listeReservations);
                break;
            case 7:
                sauvegarderVoyageurs(listeVoyageurs);
                sauvegarderBus(listeBus);
                sauvegarderReservations(listeReservations);
                libererMemoire(listeVoyageurs, listeBus, listeReservations);
                printf("Au revoir !\n");
                exit(0);
            default:
                printf("Choix invalide. Veuillez réessayer.\n");
        }}}

