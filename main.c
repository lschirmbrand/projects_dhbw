/*
Programm öffnet die CSV-Datei, im Falle eines Fehlschlags wird ein NULL-Pointer zurück gegeben, in diesem Fall wird eine Fehlermeldung ausgegeben.
Im Falle eines erfolgreichen Öffnens wird der Filestream aktiviert: Zuerst wird der Zeiger in der Datei auf den Anfang gesetzt, sodass er immer am Anfang ist.
Danach wird ein char c genutzt, um Char für Char den Filestream zu überprüfen.
Falls der Char ein Komma oder 13/10/1310 einliest, wird ein Eintrag hinzugefügt, beziehungsweise die Einträge um 1 erhöht.
Im nachfolgenden Schritt werden die Ausmaße der Gesamtmatrix erfasst.
Die Gesamtmatrix setzt sich aus einer quadratischen Matrix und einem Vektor b, der neben der quadratischen Matrix steht, zusammen.
Um das Verhältnis der Matrix auszurechnen, machen wir uns genau diese Information zu Nutzen, es resultiert:
Einträge = Zeilen*(Zeilen+1) = Zeilen * Spalten,
Folgend wird ein CharArray initialisiert, das einen String speichern soll. Zusätzlich wird ein tempchar initialisiert der das gelesene Element temporär speichert.
Bei jeder Zeigerbewegung wird der String um temp erweitert, temp variiert und ist mal das Zeichen, mal die Terminierung des Strings, also das Stringende.
Wenn der numberstring voll ist, bzw endet, weil er terminiert wird, wird der string zu double konvertiert und number zugeschrieben.
Direkt darauf wird der Wert einer Zelle der GMatrix zugewiesen, danach wird der Index des Arrays/der Matrix verschoben, in den dann wieder die neue number abgespeichert wird.
Im Falle eines Zeilenumbruchs wird der Index des Array in der Zeile um 1 erhöht und in der Spalte auf 0 gesetzt.
Dadurch wird das gesamte Array richtig abgespeichert, mit allen aus der csv-Datei übergeben Werten.
Im nächsten Schritt sollen alle Nullzeilen ignoriert werden, das heißt wir schieben sie an das Zeilenende des Arrays und ignorieren sie später bei der Übergabe an die
richtige Matrix und den Vektor, beziehungsweise passen die Arraygröße auf Zeilen-Nullzeilen an.
Danach wird die (neue&richtige) Matrix mit allen Werten der GMatrix von 0<=x<Spalten-1 besetzt für alle Zeilen abzüglich der Nullzeilen.
Der Vektor wird aus allen Werten der Spalte in Reihenfolge der Zeilen, wieder ohne Nullzeilen, deklariert.
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


int main() {
        FILE *fp;
        fp = fopen("konv3.csv", "r");








        if (fp == NULL)
            { printf("Die Datei konnte nicht geöffnet werden."); }
        if (fp!=NULL)
            {
                printf("Die Datei konnte geöffnet werden.\n\n\n");
                unsigned int eintraege = 0;             //Nutzung der insgesamten Einträge, um Zeilen/Spalten auszurechnen
                const int length = 30;
                char c = 0;                             //Einführung der "Zeiger"-Variable, wird zur Abfrage verwendet, was an Stelle x steht
                long double number = 0;                 //Einführung number, hat Wert der einzelnen Zellen der Matrix
                char temp = 0;                          //Einführung temp: Je nachdem was "char c" einliest, wird temp dessen Wert zugewiesen
                char numberstring[length];              //Einführung numberstring, CharArray dem "temp" immer wieder hinzugefügt wird, gibt Zahl einer Zelle im Stringformat aus, Array = (gebrauchte Größe + 1), weil terminierende 0
                numberstring[0] = '\0';                 //Das CharArray wird leer initialisiert, die terminierende 0 am steht am Ende
                bool prevcharbreak = false;             //Boolean um das vorgehende Zeichen zu prüfen --> Plattformunabhängig (Unix, Linux, Android, macOS, AmigaOS, BSD, Windows, DOS, OS/2, CP/M, TOS (Atari), Mac OS Classic, Apple II, C64)
                //numberstring = (char *) malloc(size * sizeof(char));  //Dynamische Speicherbereitstellung für Matrix
                fseek(fp, 0, SEEK_SET);                 //Dateizeiger auf Anfang setzen

                  while((c=fgetc(fp)) != EOF) {             //while Dateiende nicht erreicht
                    temp = c;                               //temp = momentanes Zeichen
                    if ((c == '\n' && prevcharbreak==false)|| c == ',') {    //if c = Zeilenumbruch oder Komma
                        eintraege++;

                    }
                    if (c=='\r') {                          //if c = Zeilenumbruch oder Komma
                        eintraege++;
                        prevcharbreak = true;               //daduch wird im Falle von Windows und co. \r\n umgangen, sodass die Einträge nicht doppelt gezählt werden

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



                    long double GMatrix[zeilen][spalten];
                    int is = 0;                     //Variable, die Spalte in Matrix beschreibt
                    int iz = 0;                     //Variable die Zeile in Matrix beschreibt
                    int ncounter = 0;               //Variable zur Protokollierung der insgesamten Nullzeilen

                    fseek(fp, 0, SEEK_SET);         //Dateizeiger auf Anfang setzen
                    while((c=fgetc(fp)) != EOF) {                   //Schleife, s.o
                        temp = c;
                        if (c == '\n'||c=='\r') {                   //falls Zeilenumsprung gelesen wird
                            temp = '\0';
                            strncat(numberstring, &temp, 1);        //CharArray wird verkettet
                            number = strtold(numberstring,'\0');    //Numbertostring wird zu double konvertiert und in Number gespeichert
                            GMatrix[iz][is] = number;

                            iz++;                                   //Sprung in nächste Zeile
                            is = 0;                                 //Spalte wird wieder auf 0 gesetzt
                            numberstring[0] = '\0';                 //CharArray wird gecleared

                        }

                        if (c == ',') {                             //Falls Komma gelesen wird
                            temp = '\0';
                            strncat(numberstring, &temp, 1);        //CharArray wird verkettet
                            number = strtold(numberstring,'\0');    //Numbertostring wird zu double konvertiert und in Number gespeichert
                            GMatrix[iz][is] = number;
                            is++;                                   //Spalte wird um eins erhöht, also nach rechts geschoben
                            numberstring[0] = '\0';                 //CharArray wird gecleared


                        }

                        if (c!='\n' && c != ',' && c != '\r'){      //Wenn kein Zeilenumbruch oder Komma gelesen wird -> Zahl wird als Char eingelesen
                            strncat(numberstring, &temp, 1);        //Verkettung zu CharArray/String
                        }

                      }

                      /*For Schleife die komplettes Array durchläuft, 0-Zeilen löscht
                        bzw die Zeilen darunter verschiebt um die Lücke zu füllen*/


                        for (int z = 0; z<zeilen; z++)              //Durchlaufen aller Zeilen
                        {
                            for (int i = 0; i<spalten; i++)         //Durchlaufen aller Spalten
                            {
                                if(GMatrix[z][i]!=0)                 //Wenn Eintrag in Zelle !=0
                                {
                                    break;                          //sobald ein Zeichen !=0 gelesen wird , wird Suche der Zeile abgebrochen, kann keine Nullzeile sein
                                }
                                else{
                                        if (i==spalten-1 && GMatrix[z][i] == 0)      //Wenn in letzter Spalte der Zeile 0 und noch kein Break erfolgt -> Alle Einträge der Zeile gleich 0
                                        {
                                            ncounter++;                             //Protokolierung der Nullzeilen
                                            for(int zv=z; zv<zeilen-1; zv++)        //Schleife zur Verschiebung der Elemente, hier Veränderung der Zeile
                                            {
                                                for(int sv = 0; sv<spalten;sv++)    //Schleife zur Verschiebung der Elementem hier Veränderung der Spalte
                                                {
                                                    GMatrix[zv][sv]=GMatrix[zv+1][sv]; //Elemte aus Zusammensetzung [Zeile][Spalte] überschreiben Nullzeile
                                                }
                                            }
                                            for(int nz = 0; nz<spalten;nz++)
                                            {
                                                GMatrix[zeilen-1][nz] = 0;           //Überschriebene Nullzeilen werden an Matrix unten wieder angehängt
                                            }

                                        }

                                    }
                            }

                        }

                        /*Überprüfung der Matrix

                        for(int r = 0; r<zeilen-1; r++)
                        {
                            for(int z = 0; z<spalten-1; z++)
                            {
                                if(r!=spalten){
                                printf("%lf ",Matrix[r][z]);
                                }
                                if(r==spalten){
                                printf("%lf\n",Matrix[r][z]);
                                }

                            }
                        }
                        */


                ncounter = (ncounter/2);
                printf("Nullzeilen: %d\n", ncounter);
                const int  maxsteps = 10000;
                long double Vector[zeilen];                      //Einführung des Vektors b, also dem Ergebnis der Matrix
                long double Matrix[zeilen][spalten-1];           //Einführung der tatsächlichen quadratischen Matrix
                for(int vz = 0; vz<zeilen-ncounter; vz++)
                {
                    Vector[vz] = GMatrix[vz][spalten-1];    //Vektor = GMatrix-Matrix
                }

                for(int mz=0; mz<zeilen-ncounter; mz++)
                {
                    for(int ms=0; ms<spalten-1; ms++)
                    {
                        Matrix[mz][ms]=GMatrix[mz][ms];     //Quadratische Matrix = Gmatrix -Vektor
                    }
                }

                long double xstart[zeilen-ncounter];
                for (int j = 0; j<zeilen-ncounter; j++)
                {
                    xstart[j] = 0;
                }
                printf("\n%d\n", zeilen-ncounter);



    long double xneu[zeilen-ncounter];
    unsigned int schritte = 100;
    double diff = 1;
    double limit = 0.00000001;



        /*Berechnungansatz Jacobi-Verhalten*/



                for (int k = 0; k<schritte; k++)
                {
                    for(int ce1 = 0; ce1 < zeilen-ncounter; ce1++)
                    {
                        xneu[ce1] = Vector[ce1];
                    }

                    for(int i = 0;i<zeilen-ncounter; i++)
                    {
                        for(int j = 0;j<zeilen-ncounter; j++)
                        {
                            if(i!=j)
                            {
                                 xneu[i] = xneu[i]-Matrix[i][j]*xstart[j];
                            }
                        }
                        xneu[i] = xneu[i]/Matrix[i][i];
                    }
                    for(int ce2 = 0; ce2 < zeilen-ncounter; ce2++)
                    {
                        xstart[ce2] = xneu[ce2];
                    }
                }


                for (int uuu = 0; uuu<zeilen-ncounter; uuu++)
                {
                    printf("%.10Lf\n", xstart[uuu]);

                }


                /*Berechnungsansatz Gauß Seidel Verfahren*/

               //Iterationsschleife bis Konvergenz erreicht:
               double normNeu;
               double normAlt;
               schritte = 0;

            for (int c = 0; c < schritte; c++)
            {
                /* Normwerte := 0 damit Differenz bei jedem
                 * neuen Schritt neu berechnet werden kann:*/
                normNeu = 0;
                normAlt = 0;

                //Falls Grenzwert kleiner Differenzwert:

                if (limit < diff)
                {
                    //Zeilenschleife:
                    for (int i = 0; i < zeilen - ncounter; i++)
                    {
                        xneu[i] = 0;

                        //Spaltenschleifen:
                        for (int j = 0; j < i; j++)
                        {
                            xneu[i] = xneu[i] + (Matrix[i][j] * xneu[j]);
                        }
                        for (int j = i + 1; j < zeilen-ncounter; j++)
                        {
                            xneu[i] = xneu[i] + (Matrix[i][j] * xstart[j]);
                        }
                        xneu[i] = (Vector[i] - xneu[i]) / Matrix[i][i];
                    }

                }

                //Berechnung NormNeu&NormAlt:
                for (int k = 0; k < zeilen - ncounter; k++)
                {
                    normNeu += abs(xneu[k] * xneu[k]);
                    normAlt += abs(xstart[k] * xstart[k]);
                }

                //BerechDifferenz:
                diff = normNeu - normAlt;



                for (int copy = 0; copy < zeilen-ncounter; copy++)
                {
                    xstart[copy] = xneu[copy];
                }

                schritte++;

            }
                for(int p = 0; p < zeilen - ncounter; p++)
                {
                    printf("%.10Lf\n", xneu[p]);
                }
                printf("schritte: %d",schritte);
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



