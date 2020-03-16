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
        char filename[1000];                                        //Speicherplatz für Namen der Datei, die geöffnet werden soll. Maximale Länge des Dateinamens: 999
        filename[0]= '\0';
        bool gueltig = false;                                       //Allgemein genutzte Variable zur Bestimmung von Bedingungen und Abbruch von Schleifen.
        bool out;                                                   //Bool zur Bestimmung der Darstellung des Ergebnis, auch zur Fehlerbestimmung
        long double fehlerschranke = 0.0000000001;                  //Standard-Fehlerschranke (1-e10)
        char J[]="J"; char j[]="j"; char N[]="N"; char n[]="n";     //Initialsierung von Chars, die zur Bedingungsabfrage genutz werden (Ja&Nein-Antworten)
        char fehlerschrankechar[50];                                //Fehlerschrankechar ist eine Variable, in der die Antwort gespeichert wird und mit der Bedingung verglichen wird
        fehlerschrankechar[0] = '\0';
        char output[50];                                            //Output ist eine Variable, in der die Antwort gespeichert wird und mit der Bedingung verglichen wird
        output[0] = '\0';
        do{                                             //solange nicht richtiges Antwortformat comitted wird
        printf("Wie heißt die zu öffnende CSV-Datei?\nAchtung, die Eingabe muss mit der Dateiendung .csv versehen sein.\n\n-> " );
        scanf("%s",&filename);
            if(strchr('testdateien/',strchr(filename,'.csv'))){                                                                                                 //Wenn im Dateinamen richtiges Dateiformat enthalten ist -> Verlassen der Schleife
                //if(load(filename, )){                                                                           //Wenn Methode load erfolgreich ist, die Datei öffnet und abspeichert
                if(true){
                    printf("Die Datei wurde erfolgreich geladen und die Matrix und Vektoren initialisiert.\n");
                    gueltig = true;
                }
                else{
                    printf("Die Datei konnte nicht geöfnet werden! Existiert die Datei wirklich? Befindet sie sich im richtigen Dateipfad?");
                    gueltig = false;
                }
            }
        } while(!gueltig);
        do{
            printf("Die Standard Fehlerschranke in diesem Programm liegt bei:  %1.e,  soll eine eigene Fehlerschranke eingegeben werden?(J/N)\n-> ", fehlerschranke);           //Standardfehlerschranke oder eigene?
            scanf("%s", &fehlerschrankechar );

            if(strcmp(fehlerschrankechar, J)==0 || strcmp(fehlerschrankechar, j) == 0){                                                                                          //Antwort ja
                printf("Bitte geben sie eine Fehlerschranke (in Dezimalschreibweise) ein: \n-> ");                                                                              //Aufforderung die Fehlerschranke einzugeben
                scanf("%lf", fehlerschranke);                                                                                                                                   //Überschreiben der Fehlerschranke
                gueltig = true;                                                                                                                                                 //Verlassen der Schleife
            }
            else if (strcmp(fehlerschrankechar, N)==0 || strcmp(fehlerschrankechar, n) == 0){                                                                                    //Antwort nein
                gueltig = true;                                                                                                                                                 //Verlassen der Schleife mit Standardfehlerschranke
            }
            else{
                printf("Bitte halten sie sich an die Vorgaben des Antwortens. Entweder die Eingabe 'J' für Ja, oder ’N’ für Nein.");
                gueltig = false;
            }
        } while(!gueltig);

        solve(Matrix A, Vector b, Vector x, fehlerschranke);

        do{
            if(true){// (Berechnung == erfolgreich){                                                                                                                                                               //If Berechnung erfolgreich
                printf("Die Berechnung war erfolgreich. Das Ergebnis kann komplett ausgegeben werden, standardmäßig aber nur der letzte Vektor.\nSoll das ganze Ergebnis ausgegeben werden? (J/N)");        //Ausgabe Ergebnis
                scanf("%s",&output);                                                                                                                                                                        //Output = Antwort
                if(strcmp(output, J)==0 || strcmp(output, j) == 0) {                                                                                     //Bedingung, falls Output "Ja" entspricht
                    printf("Das ganze Ergebnis soll ausgegeben werden:\n\n");                                                                           //Ganzes Ergebnis soll ausgegeben werden
                    out = true;                                                                                                                         //Siehe eine Zeile darüber
                    gueltig = true;                                                                                                                     //Schleifenabbruch
                }
                else if (strcmp(output, N)==0 || strcmp(output, n) == 0){                                                                                //Bedingung, falls Output "Nein" entspricht
                    printf("Nur der letzte Vektor soll als Ergbenis ausgegeben werden:\n\n");                                                           //Nur der letzte Vektor soll ausgegeben werden
                    out = false;                                                                                                                        //Siehe Zeile darüber
                    gueltig = true;                                                                                                                     //Schleifenabbruch
                }
                else{
                    printf("Bitte halten sie sich an die Vorgaben der Antworten. Entweder die Eingabe 'J' für Ja, oder ’N’ für Nein.");
                    gueltig = false;
                }
            }
        } while(!gueltig);

        if(out == true){                                                                                                                                 //If ganzes Ergebnis gewollt

        }
        else if(out == false){                                                                                                                           //If nur der letzte Vektor gewollt

        }
        else{printf("Ein Fehler ist im Programm aufgetreten, die Darstellung des Ergebnis kann nicht geladen werden. Bitte starten sie das Programm neu!\n\n");}        //Ein Fehler ist aufgetreten, Programmabfolge beschädigt
    return 0;
}

bool load (const char *filename, Matrix *A, Vector *b, Vector *x){
        FILE *fp;
        fp = fopen(filename, "r");
        if (fp == NULL) printf("Die Datei konnte nicht geöffnet werden.");
        else {
            printf("Die Datei konnte geöffnet werden.\n\n\n");
            unsigned int eintraege = 0;             //Nutzung der insgesamten Einträge, um Zeilen/Spalten auszurechnen
            char c = 0;                             //Einführung der "Zeiger"-Variable, wird zur Abfrage verwendet, was an Stelle x steht
            long double number = 0;                 //Einführung number, hat Wert der einzelnen Zellen der Matrix
            const int length = 30;                  //konstantes Maximum
            char temp = 0;                          //Einführung temp: Je nachdem was "char c" einliest, wird temp dessen Wert zugewiesen
            char numberstring[13];                  //Einführung numberstring, CharArray dem "temp" immer wieder hinzugefügt wird, gibt Zahl einer Zelle im Stringformat aus, Array = (gebrauchte Größe + 1), weil terminierende 0
            numberstring[0] = '\0';                 //Das CharArray wird leer initialisiert, die terminierende 0 am steht am Ende
            bool prevcharbreak = false;             //Boolean um das vorgehende Zeichen zu prüfen --> Plattformunabhängig (Unix, Linux, Android, macOS, AmigaOS, BSD, Windows, DOS, OS/2, CP/M, TOS (Atari), Mac OS Classic, Apple II, C64)
            //numberstring = (char *) malloc(size * sizeof(char));  //Dynamische Speicherbereitstellung für Matrix
            fseek(fp, 0, SEEK_SET);                 //Dateizeiger auf Anfang setzen
            while((c=fgetc(fp)) != EOF) {             //while Dateiende nicht erreicht
                temp = c;                               //temp = momentanes Zeichen
                if ((c == '\n' && prevcharbreak==false)|| c == ',') eintraege++;   //if c = Zeilenumbruch oder Komma
                if (c=='\r') {                          //if c = Zeilenumbruch oder Komma
                    eintraege++;
                    prevcharbreak = true;               //daduch wird im Falle von Windows und co. \r\n umgangen, sodass die Einträge nicht doppelt gezählt werden
                }
            }
            printf("Eintraege:\t%d\n", eintraege);
            int zeilen = 0, spalten = 0;
            if (eintraege > 0){
                for (int i = 0; i<1000; i++){
                    //wenn spalten * zeilen = einträge, spalten ist richtig und wird gespiechert, genau wie Zeilen
                    if(eintraege == zeilen*(zeilen+1))  {
                        spalten= zeilen + 1;            //Spalten = Matrix + Vektor
                        break;
                    }
                    else{
                        zeilen++;               //Zeilen werden immer erhöht, bis Schleifenende erreicht oder Einträge = Zeilen * Spalten bzw Zeilen * (Zeilen+1)
                    }
                }
                printf("Zeilen:\t\t%d\nSpalten:\t%d\n\n", zeilen, spalten);
            }
            long double GMatrix[zeilen][spalten];
            int is = 0;                             //Variable, die Spalte in Matrix beschreibt
            int iz = 0;                             //Variable die Zeile in Matrix beschreibt
            int ncounter = 0;                       //Variable zur Protokollierung der insgesamten Nullzeilen
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

                        //Durchlaufen aller Zeilen
                for (int z = 0; z<zeilen; z++){
                        //Durchlaufen aller Spalten
                    for (int i = 0; i<spalten; i++){
                        //Wenn Eintrag in Zelle !=0
                        if(GMatrix[z][i]!=0) break;//sobald ein Zeichen !=0 gelesen wird , wird Suche der Zeile abgebrochen, kann keine Nullzeile sein
                        else{
                            if (i==spalten-1 && GMatrix[z][i] == 0){      //Wenn in letzter Spalte der Zeile 0 und noch kein Break erfolgt -> Alle Einträge der Zeile gleich 0
                                ncounter++;                             //Protokolierung der Nullzeilen
                                for(int zv=z; zv<zeilen-1; zv++){      //Schleife zur Verschiebung der Elemente, hier Veränderung der Zeile
                                    for(int sv = 0; sv<spalten;sv++){    //Schleife zur Verschiebung der Elementem hier Veränderung der Spalte
                                        GMatrix[zv][sv]=GMatrix[zv+1][sv]; //Elemte aus Zusammensetzung [Zeile][Spalte] überschreiben Nullzeile
                                    }
                                }
                                for(int nz = 0; nz<spalten;nz++){
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
        long double Vector[zeilen];                      //Einführung des Vektors b, also dem Ergebnis der Matrix
        long double Matrix[zeilen][spalten-1];           //Einführung der tatsächlichen quadratischen Matrix
        for(int vz = 0; vz<zeilen-ncounter+1; vz++){
            Vector[vz] = GMatrix[vz][spalten-1];    //Vektor = GMatrix-Matrix
        }
        for(int mz=0; mz<zeilen-ncounter+1; mz++){
            for(int ms=0; ms<spalten-1; ms++){
                Matrix[mz][ms]=GMatrix[mz][ms];     //Quadratische Matrix = Gmatrix -Vektor
            }
        }



        //Abfrage der Werte der neuen Matrix und des Vektors zur Kontrolle:

        /*for(int r = 0; r<zeilen-ncounter+1; r++)
        {
            printf("%lf\n",Vector[r]);
        }*/
        int test = 0;
        for (int t = 0; t<zeilen-(ncounter); t++){
            for(int x = 0; x<spalten-1; x++){
                printf("%Lf ",Matrix[t][x]);
                test++;
                if(x==spalten-2){printf("\n");}
            }
        }
        printf("%d",test);
    }
    fclose(fp);
    if(fp!=NULL) return true;
    else return false;
}

int solve (Method method, Matrix *A, Vector *b, Vector *x, double e){

return 0;
}



