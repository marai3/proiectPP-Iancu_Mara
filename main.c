#include<stdio.h>
#include<stdlib.h> 
#include<string.h>
#include <stdbool.h>

#define MAX_SALI 10
#define MAX_FACILITATI 100
#define RESET   "\x1B[0m"
#define ROSU    "\x1B[31m"
#define VERDE   "\x1B[32m"
#define ROZ    "\x1B[38;5;206m"
#define ALBASTRU    "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define TURCOAZ "\x1B[38;5;45m"
#define ALBASTRU_DESCHIS   "\x1B[38;5;117m"
#define ROZ_INCHIS  "\x1b[38;2;219;112;147m"

struct Sala {
    int nrSala;
    int capacitate;
    int rezervata;
    char facilitati[MAX_FACILITATI][100];
};

struct Rezervare {
    char nume[50];
    int ora, minute, sala;
    char data[20];
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

void afisareSaliDisponibile(struct Sala sali[MAX_SALI], int len) {
    printf(MAGENTA "Acestea sunt salile disponibile:\n" RESET);
    for (int i = 0; i < len; i++) {
        if (sali[i].rezervata == 0) {
            printf(MAGENTA "Sala %d: Capacitate %d, Rezervata: %d, Facilitati: %s\n" RESET, sali[i].nrSala, sali[i].capacitate, sali[i].rezervata, sali[i].facilitati);
        }
    }
}
  

void rezervareSala(struct Sala sali[MAX_SALI], int len) {
    printf(ROSU "0.Inapoi\n" RESET);
    printf(ROSU "1.Rezerva sala\n" RESET);
    int x;
    scanf("%d", &x);
    if(x==1){
    afisareSaliDisponibile(sali, len);
    printf(ROSU "Alege sala pentru rezervare: " RESET);
    int y;
    scanf("%d", &y);
    getchar(); 
    struct Rezervare rezervare;
    printf(ROSU "Introdu numele: " RESET);
    fgets(rezervare.nume, sizeof(rezervare.nume), stdin);
    strtok(rezervare.nume, "\n");

    char oraa[10];
    printf(ROSU "Introdu ora rezervarii: " RESET);
    fgets(oraa, sizeof(oraa), stdin);
    sscanf(oraa, "%d:%d", &rezervare.ora, &rezervare.minute);

    printf(ROSU "Introdu data rezervarii: " RESET);
    fgets(rezervare.data, sizeof(rezervare.data), stdin);
    strtok(rezervare.data, "\n");

    for (int i = 0; i < len; i++) {
        if (sali[i].nrSala == y) {
            sali[i].rezervata = 1;
        }
    }

    FILE *fisier;
    fisier = fopen("rezervari.txt", "a");
    if (fisier == NULL) {
        printf(ROSU "Eroare la deschiderea fisierului." RESET);
        exit(EXIT_FAILURE);
    }
   fprintf(fisier, "Nume: %s, Ora: %02d:%02d, Data: %s, Sala: %d\n", rezervare.nume, rezervare.ora, rezervare.minute, rezervare.data, y);
    fclose(fisier);

    
    fisier = fopen("sali.txt", "w");
    if (fisier == NULL) {
        printf(ROSU "Eroare la deschiderea fisierului." RESET);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < len; i++) {
        fprintf(fisier, "%d %d %d %s\n", sali[i].nrSala, sali[i].capacitate, sali[i].rezervata, sali[i].facilitati);
    }
    fclose(fisier);
    }
}

void AnulareRezervare(struct Sala sali[MAX_SALI], int len) {
    struct Rezervare anularerez;
    char nume[50];
    int ora, minut, sala;
    char data[20];
    printf(ALBASTRU "Introdu numele: " RESET);
    fflush(stdin);
    fgets(anularerez.nume, sizeof(anularerez.nume), stdin);
    strtok(anularerez.nume, "\n");

    printf(ALBASTRU "Introdu ora rezervarii: " RESET);
    scanf("%d:%d", &anularerez.ora, &anularerez.minute);
    getchar(); 

    printf(ALBASTRU "Introdu data rezervarii: " RESET);
    fgets(anularerez.data, sizeof(anularerez.data), stdin);
    strtok(anularerez.data, "\n");

    printf(ALBASTRU "Alege sala pentru a anula rezervarea: " RESET);
    scanf("%d", &anularerez.sala);

    FILE *fisier = fopen("rezervari.txt", "r");
    FILE *temporar = fopen("temporar.txt", "w");
    if (fisier == NULL || temporar == NULL) {
        printf(ALBASTRU "Eroare la deschiderea fisierului." RESET);
        exit(EXIT_FAILURE);
    }
    int rezervareGasita = 0;
    char line[200]; 

    while (fgets(line, sizeof(line), fisier) != NULL) {
        
        if (strstr(line, anularerez.nume) != NULL && strstr(line, anularerez.data) != NULL &&
            strstr(line, "Ora:") != NULL && strstr(line, "Sala:") != NULL) {
            
            if (strstr(line, anularerez.nume) != NULL && strstr(line, anularerez.data) != NULL &&
                strstr(line, "Ora: ") != NULL && strstr(line, "Sala: ") != NULL) {
                rezervareGasita = 1;
               printf(ALBASTRU "Rezervarea a fost anulata cu succes.\n" RESET);
            } else {
                
                fprintf(temporar, "%s", line);
            }
        } else {
            
            fprintf(temporar, "%s", line);
        }
    }
    fclose(fisier);
    fclose(temporar);

    if (!rezervareGasita) {
        printf(ALBASTRU "Nu s-a gasit nicio rezervare conform cerintelor.\n" RESET);
        return;
    }

    remove("rezervari.txt");
    rename("temporar.txt", "rezervari.txt");

    fisier = fopen("sali.txt", "w");
    if (fisier == NULL) {
        printf(ALBASTRU "Eroare la deschiderea fisierului." RESET);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < len; i++) {
        if (sali[i].nrSala == anularerez.sala) {
            sali[i].rezervata = 0;
        }
        fprintf(fisier, "%d %d %d %s\n", sali[i].nrSala, sali[i].capacitate, sali[i].rezervata, sali[i].facilitati);
    }
    fclose(fisier);

}

void cautaSalaFacilitati();
void cautaSalaCapacitate();

void cautaSala(struct Sala sali[MAX_SALI], int len) {
    struct Sala vectorSaliDisponibile[MAX_SALI];
    printf(TURCOAZ "1. Doriti sa cautati sala dupa capacitate\n" RESET);
    printf(TURCOAZ "2. Doriti sa cautati sala dupa facilitati\n" RESET);
    printf(TURCOAZ "Introduceti optiunea dorita: " RESET);
    int choice;
    scanf("%d", &choice);

    if (choice == 1) {
        cautaSalaCapacitate();
    } else if (choice == 2) {
        cautaSalaFacilitati(vectorSaliDisponibile, len);
    } else {
        printf(TURCOAZ "Optiune invalida. Va rugam sa selectati o optiune valida.\n" RESET);
    }
}
void cautaSalaFacilitati(struct Sala sali[MAX_SALI], int len) {
    char facilitatiCautate[200];
    char alegere;
    
    do {
        printf(TURCOAZ "Introduceti facilitatea cautata: " RESET);
        getchar();
        fgets(facilitatiCautate, sizeof(facilitatiCautate), stdin);
        facilitatiCautate[strcspn(facilitatiCautate, "\n")] = '\0'; 

        FILE *fisier = fopen("sali.txt", "r");
        if (fisier == NULL) {
            printf(TURCOAZ "Eroare la deschiderea fisierului." RESET);
            exit(EXIT_FAILURE);
        }
        
        printf(TURCOAZ "Sali cu facilitatea '%s':\n" RESET, facilitatiCautate);
        char line[200];
        int gasit = 0;
        while (fgets(line, sizeof(line), fisier) != NULL) {
            int salaNr, capacitate, rezervata;
            char facilitati[MAX_FACILITATI][100];
            sscanf(line, "%d %d %d", &salaNr, &capacitate, &rezervata);

            char *token = strtok(line, " ");
            token = strtok(NULL, " "); 
            token = strtok(NULL, " "); 
            token = strtok(NULL, " "); 

            int i = 0;
            while (token != NULL) {
                strcpy(facilitati[i++], token);
                token = strtok(NULL, " ");
            }
            int gasitInSala = 0;
            for (int j = 0; j < i; j++) {
                if (strstr(facilitati[j], facilitatiCautate) != NULL) {
                    gasitInSala = 1;
                    break;
                }
            }
             if (gasitInSala) {
                printf(TURCOAZ "%d %d %d ", salaNr, capacitate, rezervata);
                for (int j = 0; j < i; j++) {
                    if (strlen(facilitati[j]) > 0) {
                        printf("%s", facilitati[j]);
                    }
                }
                printf("\n");
                gasit = 1;
            }
        }
        if (!gasit) {
            printf(TURCOAZ "Nu exista nicio sala cu facilitatea cautata.\n" RESET);
        }

        fclose(fisier);

        printf(TURCOAZ "Doriti sa cautati alta facilitate? (Da - d, Nu - n): " RESET);
        scanf(" %c", &alegere);
    } while (alegere == 'd' || alegere == 'D');
}
void cautaSalaCapacitate() {
    int capacitateCautata;
    printf(TURCOAZ "Introduceti capacitatea cautata: " RESET);
    scanf("%d", &capacitateCautata); 

    FILE *fisier = fopen("sali.txt", "r");
    if (fisier == NULL) {
        printf(TURCOAZ "Eroare la deschiderea fisierului." RESET);
        exit(EXIT_FAILURE);
    }
    
    printf(TURCOAZ "Sali cu capacitatea '%d':\n" RESET, capacitateCautata);
    char line[200];
    int gasit = 0;
    while (fgets(line, sizeof(line), fisier) != NULL) {
        int capacitate;
        sscanf(line, "%*d %d", &capacitate);
        if (capacitate == capacitateCautata) {
            printf(TURCOAZ "%s" RESET, line);
            gasit = 1;
        }
    }
    if (!gasit) {
        printf(TURCOAZ "Nu este sala cu aceasta capacitate\n" RESET);
    }
    fclose(fisier);
}

int main() {
    struct Sala vectorSaliDisponibile[MAX_SALI];
    int len = 0;
    char facilitatiCautate[100];
    citireSali(vectorSaliDisponibile, &len);

    int choice;
    do {
        printf(MAGENTA"\n1. Afisare sali disponibile\n"RESET);
        printf(ROSU "2. Rezervare sala\n" RESET);
        printf(ALBASTRU "3. Anulare rezervare\n" RESET);
        printf(TURCOAZ"4. Cauta sala dupa facilitati\n");
        printf(ALBASTRU_DESCHIS"5. Iesire\n" RESET);
        printf(ROZ_INCHIS "Introduceti optiunea dorita: " RESET);
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                afisareSaliDisponibile(vectorSaliDisponibile, len);
                break;
            case 2:
                rezervareSala(vectorSaliDisponibile, len);
                break;
            case 3:
                AnulareRezervare(vectorSaliDisponibile, len);
                break;
            case 5:
                printf(ALBASTRU_DESCHIS "Iesire din program.\n" RESET);
                break;
            case 4:
                cautaSala(vectorSaliDisponibile, len);
                break;
            default:
                printf("Optiune invalida. Va rugam sa selectati o optiune valida.\n");
        }
    } while(choice != 5);

    return 0;
}
