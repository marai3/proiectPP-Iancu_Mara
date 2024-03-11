#include<stdio.h>

struct Sala {
    int nrSala;
    int capacitate;
    int rezervata;
    char facilitati[100];
};

void afisareSali(struct Sala sali[10], int len) {
    int x;
     printf("0.Inapoi\n");
     printf("1.Sali disponibile\n");
     scanf("%d", &x);
     if(x==1){
    printf("%d\n", len);
    printf("Acestea sunt salile disponibile:\n");
    for (int i = 0; i < len; i++) {
        printf("Sala %d: Capacitate %d, Rezervata: %d, Facilitati: %s\n",sali[i].nrSala, sali[i].capacitate, sali[i].rezervata, sali[i].facilitati);
    }}
    
}
void afisareSaliDisponibile(struct Sala sali[3], int len) {
    int x;
    printf("0.Inapoi\n");
    printf("1.Sali disponibile");
    scanf("%d", &x);
    if(x==1){
    printf("Acestea sunt salile disponibile:\n");
    for (int i = 0; i < len; i++) {
        if  (sali[i].rezervata == 0) {
            printf("Sala %d: Capacitate %d, Rezervata: %d, Facilitati: %s\n",sali[i].nrSala, sali[i].capacitate, sali[i].rezervata, sali[i].facilitati);
        }
    }   
    }
}

int main() {  
    struct Sala s1 = {1, 100, 1, "Parcare"};
    struct Sala s2 = {2, 50, 0, "Wifi"};
    struct Sala s3 = {3, 150, 1, "Iluminat"};

    struct Sala vectorSaliDisponibile[3] = {s1, s2, s3};
    int len = sizeof(vectorSaliDisponibile) / sizeof(vectorSaliDisponibile[0]);
    int choice;
    do {
        printf("\n1. Afisare sali\n");
        printf("2. Afisare sali disponibile\n");
        printf("3. Rezervare sala\n");
        printf("4. Anularea rezervare\n");
        printf("5. Iesire\n");
        printf("Introduceti optiunea dorita: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                afisareSali(vectorSaliDisponibile, len);
                break;
            case 2:
                afisareSaliDisponibile(vectorSaliDisponibile, len);
                break;
            case 3:
                
                printf("Functia de rezervare sala nu este implementata inca.\n");
                break;
            case 4:
                
                printf("Functia de anulare rezervare nu este implementata inca.\n");
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
