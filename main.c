/*To dos:
-Plattformunabhängigkeit bewerkstelligen durch \n\r || \n || \r
-
-
-
*/

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
        fp = fopen("konvnulltest.csv", "r");


        if (fp == NULL)
            { printf("Die Datei konnte nicht geöffnet werden."); }
        if (fp!=NULL)
            {
                printf("Die Datei konnte geöffnet werden.\n\n\n");
                unsigned int eintraege = 0;             //Nutzung der insgesamten Einträge, um Zeilen/Spalten auszurechnen
                char c = 0;                             //Einführung der "Zeiger"-Variable, wird zur Abfrage verwendet, was an Stelle x steht
                double number = 0;                      //Einführung number, hat Wert der einzelnen Zellen der Matrix
                char temp = 0;                          //Einführung temp: Je nachdem was "char c" einliest, wird temp dessen Wert zugewiesen
                char numberstring[13];                  //Einführung numberstring, CharArray dem "temp" immer wieder hinzugefügt wird, gibt Zahl einer Zelle im Stringformat aus, Array = (gebrauchte Größe + 1), weil terminierende 0
                numberstring[0] = '\0';                 //Das CharArray wird leer initialisiert, die terminierende 0 am steht am Ende
                //numberstring = (char *) malloc(size * sizeof(char));  //Dynamische Speicherbereitstellung für Matrix
                fseek(fp, 0, SEEK_SET);                 //Dateizeiger auf Anfang setzen

                  while((c=fgetc(fp)) != EOF) {             //while Dateiende nicht erreicht
                    temp = c;                               //temp = momentanes Zeichen
                    if (c == '\n'|| c == ','||c=='\r') {    //if c = Zeilenumbruch oder Komma
                        eintraege++;

                    }

                  }


                    printf("Eintraege:\t%d\n", eintraege);

                    int zeilen = 0;                     //Initialisierung der Variable für Zeilen
                    int spalten = 0;                    //Initialisierung der Variable für spalten

                    if (eintraege > 0){                 //wenn mindestens 1 Eintrag in der Datei ist
                        for (int i = 0; i<1000; i++)    //Schleife für maximal 1.001.000 Einträge
                        {
                            if(eintraege == zeilen*(zeilen+1))  //wenn spalten * zeilen = einträge, spalten ist richtig und wird gespiechert, genau wie Zeilen
                            {
                                spalten= zeilen + 1;            //Spalten = Matrix + Vektor
                                break;
                            }
                            else
                            {
                                zeilen++;               //Zeilen werden immer erhöht, bis Schleifenende erreicht oder Einträge = Zeilen * Spalten bzw Zeilen * (Zeilen+1)
                            }
                        }

                        printf("Zeilen:\t\t%d\nSpalten:\t%d\n\n", zeilen, spalten);

                    }



                    double Matrix[zeilen][spalten];
                    int is = 0;                     //Variable, die Spalte in Matrix beschreibt
                    int iz = 0;                     //Variable die Zeile in Matrix beschreibt
                    //bool null = false;

                    fseek(fp, 0, SEEK_SET);         //Dateizeiger auf Anfang setzen
                    while((c=fgetc(fp)) != EOF) {                   //Schleife, s.o
                        temp = c;
                        if (c == '\n'){//}||c=='\r') {                   //falls Zeilenumsprung gelesen wird
                            temp = '\0';
                            strncat(numberstring, &temp, 1);        //CharArray wird verkettet
                            number = atof(numberstring);            //Numbertostring wird zu double konvertiert und in Number gespeichert
                            Matrix[iz][is] = number;

                            iz++;                                   //Sprung in nächste Zeile
                            is = 0;                                 //Spalte wird wieder auf 0 gesetzt
                            numberstring[0] = '\0';                 //CharArray wird gecleared

                        }

                        if (c == ',') {                             //Falls Komma gelesen wird
                            temp = '\0';
                            strncat(numberstring, &temp, 1);        //CharArray wird verkettet
                            number = atof(numberstring);            //Numbertostring wird zu double konvertiert und in Number gespeichert
                            Matrix[iz][is] = number;
                            is++;                                   //Spalte wird um eins erhöht, also nach rechts geschoben
                            numberstring[0] = '\0';                 //CharArray wird gecleared


                        }

                        if (c!='\n' && c != ',' && c != '\r'){      //Wenn kein Zeilenumbruch oder Komma gelesen wird -> Zahl wird als Char eingelesen
                            strncat(numberstring, &temp, 1);        //Verkettung zu CharArray/String
                        }

                      }

                      /*For Schleife die komplettes Array durchläuft, 0-Zeilen löscht
                        bzw die Zeilen darunter verschiebt um die Lücke zu füllen*/
                        int x = 0;

                        for (int z = 0; z<zeilen; z++)
                        {
                            for (int i = 0; i<spalten; i++)
                            {
                                if(Matrix[z][i]!=0)
                                {
                                    //printf("%d\n", x);
                                    //x++;
                                    break;
                                }
                                else{
                                    printf("%d, %d\n",z,i );
                                    //null = true;
                                    printf("Spalte max: %d\n", spalten-1);
                                    if (i==spalten-1 && Matrix[z][i] == 0)
                                    {
                                        printf("Nullzeile bei Zeile: %d\n", z+1);
                                        for(int zv=z; zv<zeilen-1; zv++)
                                        {
                                            for(int sv = 0; sv<spalten;sv++)
                                            {
                                                printf("%d=zv\n", zv);
                                                Matrix[zv][sv]=Matrix[zv+1][sv];
                                            }
                                        }
                                    }

                                }
                            }

                        }

                        printf("%lf ",Matrix[0][0]);
                        printf("%lf ",Matrix[0][1]);
                        printf("%lf ",Matrix[0][2]);
                        printf("%lf\n",Matrix[0][3]);
                        printf("%lf ",Matrix[1][0]);
                        printf("%lf ",Matrix[1][1]);
                        printf("%lf ",Matrix[1][2]);
                        printf("%lf\n",Matrix[1][3]);
                        printf("%lf ",Matrix[2][0]);
                        printf("%lf ",Matrix[2][1]);
                        printf("%lf ",Matrix[2][2]);
                        printf("%lf\n",Matrix[2][3]);



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



