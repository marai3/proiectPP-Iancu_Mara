#include<stdio.h>
#include<stdlib.h> 
#include<string.h>

#define MAX_SALI 10
#define MAX_FACILITATI 100

struct Sala {
    int nrSala;
    int capacitate;
    int rezervata;
    char facilitati[MAX_FACILITATI];
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

void afisareSali(struct Sala sali[MAX_SALI], int len) {
    printf("Acestea sunt salile:\n");
    for (int i = 0; i < len; i++) {
        printf("Sala %d: Capacitate %d, Rezervata: %d, Facilitati: %s, Nume rezervare: %s, Ora rezervare: %d:%02d, Data rezervare: %s\n", 
               sali[i].nrSala, sali[i].capacitate, sali[i].rezervata, sali[i].facilitati);
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
    printf("0.Inapoi\n");
    printf("1.Rezerva sala\n");
    int x;
    scanf("%d", &x);
    if(x==1){
    afisareSaliDisponibile(sali, len);
    printf("Alege sala pentru rezervare: ");
    int y;
    scanf("%d", &y);
    getchar(); 
    struct Rezervare rezervare;
    printf("Introdu numele: ");
    fgets(rezervare.nume, sizeof(rezervare.nume), stdin);
    strtok(rezervare.nume, "\n");

    char oraa[10];
    printf("Introdu ora rezervarii: ");
    fgets(oraa, sizeof(oraa), stdin);
    sscanf(oraa, "%d:%d", &rezervare.ora, &rezervare.minute);

    printf("Introdu data rezervarii: ");
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
        printf("Eroare la deschiderea fisierului.");
        exit(EXIT_FAILURE);
    }
   fprintf(fisier, "Nume: %s, Ora: %02d:%02d, Data: %s, Sala: %d\n", rezervare.nume, rezervare.ora, rezervare.minute, rezervare.data, y);
    fclose(fisier);

    
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
}

void AnulareRezervare(struct Sala sali[MAX_SALI], int len) {
    struct Rezervare anularerez;
    char nume[50];
    int ora, minut, sala;
    char data[20];
    printf("Introdu numele: ");
    fflush(stdin);
    fgets(anularerez.nume, sizeof(anularerez.nume), stdin);
    strtok(anularerez.nume, "\n");

    printf("Introdu ora rezervarii: ");
    scanf("%d:%d", &anularerez.ora, &anularerez.minute);
    getchar(); 

    printf("Introdu data rezervarii: ");
    fgets(anularerez.data, sizeof(anularerez.data), stdin);
    strtok(anularerez.data, "\n");

    printf("Alege sala pentru a anula rezervarea: ");
    scanf("%d", &anularerez.sala);

    FILE *fisier = fopen("rezervari.txt", "r");
    FILE *temporar = fopen("temporar.txt", "w");
    if (fisier == NULL || temporar == NULL) {
        printf("Eroare la deschiderea fisierului.");
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
               printf("Rezervarea a fost anulata cu succes.\n");
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
        printf("Nu s-a gasit nicio rezervare conform cerintelor.\n");
        return;
    }

    remove("rezervari.txt");
    rename("temporar.txt", "rezervari.txt");

    fisier = fopen("sali.txt", "w");
    if (fisier == NULL) {
        printf("Eroare la deschiderea fisierului.");
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
