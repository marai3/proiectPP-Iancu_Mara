#include<stdio.h>
#include<stdlib.h> 

#define MAX_SALI 10
#define MAX_FACILITATI 100

struct Sala {
    int nrSala;
    int capacitate;
    int rezervata;
    char facilitati[MAX_FACILITATI];
};

void citireSali(struct Sala sali[MAX_SALI], int *len) {
    FILE *fisier;
    fisier = fopen("sali.txt", "r");
    if (fisier == NULL) {
        printf("Eroare la deschiderea fisierului.");
        exit(EXIT_FAILURE); 
    }

    *len = 0;
    while ((*len < MAX_SALI) && (fscanf(fisier, "%d %d %d %[^\n]", &sali[*len].nrSala, &sali[*len].capacitate, &sali[*len].rezervata, sali[*len].facilitati) == 4)) {
        (*len)++;
    }

    fclose(fisier);
}


void afisareSali(struct Sala sali[MAX_SALI], int len) {
    printf("Acestea sunt salile:\n");
    for (int i = 0; i < len; i++) {
        printf("Sala %d: Capacitate %d, Rezervata: %d, Facilitati: %s\n", sali[i].nrSala, sali[i].capacitate, sali[i].rezervata, sali[i].facilitati);
    }
}


void afisareSaliDisponibile(struct Sala sali[MAX_SALI], int len) {
    printf("Acestea sunt salile disponibile:\n");
    for (int i = 0; i < len; i++) {
        if (sali[i].rezervata == 0) {
            printf("Sala %d: Capacitate %d, Rezervata: %d, Facilitati: %s\n", sali[i].nrSala, sali[i].capacitate, sali[i].rezervata, sali[i].facilitati);
        }
    }
}


void rezervareSala(struct Sala sali[MAX_SALI], int len) {
    afisareSaliDisponibile(sali, len);
    printf("Alege sala pentru rezervare: ");
    int y;
    scanf("%d", &y);
    for (int i = 0; i < len; i++) {
        if (sali[i].nrSala == y) {
            sali[i].rezervata = 1;
        }
    }

   
    FILE *fisier;
    fisier = fopen("sali.txt", "w");
    if (fisier == NULL) {
        printf("Eroare la deschiderea fisierului.");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < len; i++) {
        fprintf(fisier, "%d %d %d %s\n", sali[i].nrSala, sali[i].capacitate, sali[i].rezervata, sali[i].facilitati);
    }
    fclose(fisier);
}
void AnulareRezervare(struct Sala sali[MAX_SALI], int len) {
    printf("Alege sala pentru a anula rezervarea: ");
    int z;
    scanf("%d", &z);
    for (int i = 0; i < len; i++) {
        if (sali[i].nrSala == z) {
            sali[i].rezervata = 0;
        }
    }

   
    FILE *fisier;
    fisier = fopen("sali.txt", "w");
    if (fisier == NULL) {
        printf("Eroare la deschiderea fisierului.");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < len; i++) {
        fprintf(fisier, "%d %d %d %s\n", sali[i].nrSala, sali[i].capacitate, sali[i].rezervata, sali[i].facilitati);
    }
    fclose(fisier);
}

int main() {

    struct Sala vectorSaliDisponibile[MAX_SALI];
    int len = 0;
    citireSali(vectorSaliDisponibile, &len);

    int choice;
    do {
        printf("\n1. Afisare sali\n");
        printf("2. Afisare sali disponibile\n");
        printf("3. Rezervare sala\n");
        printf("4. Anulare rezervare\n");
        printf("5. Iesire\n");
        printf("Introduceti optiunea dorita: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                afisareSali(vectorSaliDisponibile, len);
                break;
            case 2:
                afisareSaliDisponibile(vectorSaliDisponibile, len);
                break;
            case 3:
                rezervareSala(vectorSaliDisponibile, len);
                break;
            case 4:
                AnulareRezervare(vectorSaliDisponibile, len);
                break;
            case 5:
                printf("Iesire din program.\n");
                break;
            default:
            printf("Optiune invalida. Va rugam sa selectati o optiune valida.\n");
        }
    } while(choice != 5);

    return 0;
}

