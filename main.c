#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int n;
    double **data;
} Matrix;
typedef struct
{
    int n;
    double *data;
} Vector;
typedef enum
{
    JACOBI = 0, GAUSS_SEIDEL = 1
} Method;

//#define BETWEEN(x, a,b) (x >= a && x <= b)

int main() {
        FILE *fp;
        fp = fopen("konv4.csv", "r");


        if (fp == NULL)
            { printf("Die Datei konnte nicht geöffnet werden."); }
        if (fp!=NULL)
            {
                printf("Die Datei konnte geöffnet werden.\n\n\n");
                unsigned int eintraege = 0;
                char c = 0;
                int number = 0;
                char temp = 0;
                char numberstring[13];
                numberstring[0] = '\0';
                //numberstring = (char *) malloc(size * sizeof(char));
                fseek(fp, 0, SEEK_SET); //Dateizeiger auf Anfang setzen

                  while((c=fgetc(fp)) != EOF) {
                    temp = c;
                    if (c == '\n'|| c == ','||c=='\r') {
                        eintraege++;

                    } //eventuell noch mit temp Variable den Vorgängerchar

                  }
                    //printf("%d\n", (int)numberstring-'0');
                    printf("%s", numberstring);
                    printf("Eintraege:\t%d\n", eintraege);

                    int zeilen = 0; //Initialisierung der Variable für Zeilen
                    int spalten = 0; //Initialisierung der Variable für spalten

                    if (eintraege > 0){ //wenn mindestens 1 Eintrag in der Datei ist
                        for (int i = 0; i<1000; i++) //Schleife für maximal 1.001.000 Einträge
                        {
                            if(eintraege == zeilen*(zeilen+1)) //wenn spalten * zeilen = einträge, spalten ist richtig und wird gespiechert, genau wie Zeilen
                            {
                                spalten= zeilen + 1; //Spalten = Matrix + Vektor
                                break;
                            }
                            else
                            {
                                zeilen++; //Zeilen werden immer erhöht, bis Schleifenende erreicht oder Einträge = Zeilen * Spalten bzw Zeilen * (Zeilen+1)
                            }
                        }

                        printf("Zeilen:\t\t%d\nSpalten:\t%d\n\n", zeilen, spalten);

                    }

                    while((c=fgetc(fp)) != EOF) {
                        temp = c;
                        if (c == '\n'|| c == ','||c=='\r') {
                            temp = '\0';
                            printf("%s\n", numberstring);
                            number = atof(numberstring);
                            numberstring[0] = '\0';

                        } //eventuell noch mit temp Variable den Vorgängerchar

                        if (c!='\n' || c != ',' || c != '\r'){
                            strncat(numberstring, &temp, 1);
                        }

                      }

            }


        fclose(fp);

    return 0;
}

bool load (const char *konv3, Matrix *A, Vector *b, Vector *x){



return true;

}

int solve (Method method, Matrix *A, Vector *b, Vector *x, double e){

return 0;
}



