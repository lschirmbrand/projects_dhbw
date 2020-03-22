/*

    Am Projekt beteiligt waren:
        
        Name:                   Matrikelnummer:
        Denis Bauer             2832690
        Fabian Lohmüller        9514094
        Luca Schirmbrand        4775194

*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    int n;
    double **data;
} Matrix;

typedef struct{
    int n;
    double *data;
} Vector;

typedef enum{
    JACOBI = 0, GAUSS_SEIDEL = 1
} Method;

int countEintraege(const char filename[]){
    FILE *fp;
    fp = fopen(filename, "r");
    int i = 0;
    char c;
    bool prevcharbreak = false;
        //Dateizeiger auf Anfang setzen
    fseek(fp, 0, SEEK_SET);
        //solange Dateiende nicht erreicht
    while((c = fgetc(fp)) != EOF) {
       if((c == '\n' && prevcharbreak == false)|| c == ',') i++;   //if c = Zeilenumbruch oder Komma
       if(c =='\r'){//if c = Zeilenumbruch oder Komma
           i++;
           prevcharbreak = true; //daduch wird im Falle von Windows und co. \r\n umgangen, sodass die Einträge nicht doppelt gezählt werden
       }
    }
    fclose(fp);
    return i;
}

int getZeilen(int eintraege){//zählt anzahl der Zeilen
    int zeilen = 0;
    if (eintraege > 0){
        for (int i = 0; i<1000; i++)
            if(eintraege == zeilen * (zeilen + 1))
                break;
            else zeilen++;
        return zeilen;
    }
    else return 0;
}

int load (const char *filename, Matrix *A, Vector *b, Vector *x){
    //Datei öffnen
    FILE *fp;
    fp = fopen(filename, "r");
    //wenn datei ungültig ist kann gleich aufgehört werden
    if (fp == NULL) return false;

    //zählen der enträge
    int eintraege = countEintraege(filename);
    printf("Eintraege:\t%d\n", eintraege);
    //ermitteln wie viele zeilen und spalten
    int zeilen = getZeilen(eintraege), spalten = zeilen+1;
    printf("Zeilen:\t\t%d\nSpalten:\t%d\n", zeilen, spalten);
    //parsen der Werte aus der .csv-Datei
    char numberstring[30]; // numberstring, CharArray dem "temp" immer wieder hinzugefügt wird, gibt Zahl einer Zelle im Stringformat aus, Array = (gebrauchte Größe + 1), weil terminierende 0
    long double number = 0; // number, hat Wert der einzelnen Zellen der Matrix
    char temp = 0; //temp: Je nachdem was "char c" einliest, wird temp dessen Wert zugewiesen
    numberstring[0] = '\0'; //Das CharArray wird leer initialisiert, die terminierende 0 am steht am Ende
    long double tempMatrix[zeilen][spalten]; //Temporäre Matrix, enthält noch nullzeilen und den vektor b
    int iSpalte = 0, iZeile = 0, aNullZeilen = 0;
    fseek(fp, 0, SEEK_SET); //Dateizeiger auf Anfang setzen
    char c;
    while((c=fgetc(fp)) != EOF){ //bis Dateiende erreicht wird
        temp = c;
        //falls Zeilenumsprung gelesen wird
        if (c == '\n'||c=='\r'){
            temp = '\0';
            strncat(numberstring, &temp, 1); //CharArray wird verkettet
            number = strtold(numberstring,'\0'); //Numbertostring wird zu double konvertiert und in Number gespeichert
            tempMatrix[iZeile][iSpalte] = number;
            iZeile++; //nächste Zeile
            iSpalte = 0; //erste Spalte
            numberstring[0] = '\0'; //CharArray wieder auf Anfang
        }
        //Falls Komma gelesen wird
        else if (c == ','){
            temp = '\0';
            strncat(numberstring, &temp, 1); //CharArray wird verkettet
            number = strtold(numberstring,'\0'); //Numbertostring wird zu double konvertiert und in Number gespeichert
            tempMatrix[iZeile][iSpalte] = number;
            iSpalte++; //nächste Spalte
            numberstring[0] = '\0'; //CharArray wieder auf Anfang
        }
        //Wenn kein Zeilenumbruch oder Komma gelesen wird -> Zahl wird als Char eingelesen
        else strncat(numberstring, &temp, 1); //Verkettung zu CharArray/String
    }
    fclose(fp);
    //nullzeilen löschen
    for (int z = 0; z < zeilen; z++)
        for (int s = 0; s < spalten; s++)
            if(tempMatrix[z][s] != 0) break; //sobald ein Zeichen !=0 gelesen wird , wird Suche der Zeile abgebrochen, kann keine Nullzeile sein
            else if (s == spalten - 1 && tempMatrix[z][s] == 0){ //Wenn in letzter Spalte der Zeile 0 und noch kein Break erfolgt -> Alle Einträge der Zeile gleich 0
                aNullZeilen++;
                //Elemente werden verschoben
                for(int zv = s; zv < zeilen - 1; zv++)
                    for(int sv = 0; sv < spalten; sv++)
                        //Elemte aus Zusammensetzung [Zeile][Spalte] überschreiben Nullzeile
                        tempMatrix[zv][sv] = tempMatrix[zv+1][sv];
                for(int nz = 0; nz < spalten;nz++)
                    //Überschriebene Nullzeilen werden an Matrix unten wieder angehängt
                    tempMatrix[zeilen-1][nz] = 0;
            }
    printf("Nullzeilen:\t%d\n\n\n", aNullZeilen);
    zeilen -= aNullZeilen;

    // Matrix und Vektoren bekommen anzahl ihrer Daten und Zeilen
    A -> data = (double**)malloc(sizeof(double*) * zeilen);
    for(int x = 0; x < zeilen; x++)
        A -> data[x] = (double*)calloc(zeilen, sizeof(double));
    b -> data = (double*)malloc(sizeof(double*) * zeilen);
    x -> data = (double*)malloc(sizeof(double*) * zeilen);
    A -> n = zeilen;
    b -> n = zeilen;
    x -> n = zeilen;
    // Matrix und Vektoren bekommen ihre Werte
    for(int az = 0; az < zeilen; az++)
        //Matrix besteht aus allen Werten ohne die letzte Spalte
        for(int as = 0; as < spalten -1; as++)
            A -> data[az][as] = tempMatrix[az][as];
    for(int bz = 0; bz < zeilen; bz++)
        //Vektor besteht aus allen Werten in der letzten Spalte
        b -> data[bz] = tempMatrix[bz][spalten-1];
    return true;
}

void solve (Method method, Matrix *A, Vector *b, Vector *x, double e){
    int schritt = 0, maxSchritte = 100;
    //Neuer Vektor zum Vergleich des neuen und alten Wertes
    Vector xAlt;
    xAlt.n = x -> n;
    xAlt.data = (double*)malloc(sizeof(double*) * x -> n);
    double diff = 0;

    if(method == JACOBI)
        do{// solange bis zum letzten wert kleiner oder gleich Fehlerschranke ist
            //Algorithmus - (siehe Pseudocode auf Wikipedia)
            for(int i = 0; i < x -> n; i++){
                x -> data[i] = b -> data[i];
                for(int j = 0; j < x -> n; j++)
                    if(j != i)
                         x -> data[i] -= A -> data[i][j] * xAlt.data[j];
                x -> data[i] /= A -> data[i][i];
            }
            //Überprüfung ob Differenz kleiner als die Fehlerschranke ist
            for(int i = 0; i < x -> n; i++){
                //Berechnung der kleinsten Differenz zwischen x und xAlt
                diff = xAlt.data[i] - x -> data[i];
                if(diff < 0) diff = diff * (-1); //Betrag der Differenz
                //Wenn die Differenz von einem der Werte des Vektors x kleiner oder gleich zur Fehlerschrankeist, kann aufgehört werden
                if(diff <= e) break;
                xAlt.data[i] = x -> data[i];
            }
            schritt++;
        } while(diff > e && schritt < maxSchritte);
    else if(method == GAUSS_SEIDEL)
        do{// solange bis zum letzten wert kleiner oder gleich Fehlerschranke ist
            //Algorithmus - (siehe Pseudocode auf Wikipedia)
            for (int k = 0; k < x -> n; k++){
                x -> data[k] = 0;
                for (int j = 0; j < k; j++)
                    x -> data[k] += (A -> data[k][j] * x -> data[j]);
                for (int j = k + 1; j < x -> n; j++)
                    x -> data[k] += (A -> data[k][j] * xAlt.data[j]);
                x -> data[k] = (b->data[k] - x -> data[k]) / A->data[k][k];
            }
            //Überprüfung ob Differenz kleiner als die Fehlerschranke ist
            for(int i = 0; i < x -> n; i++){
                //Berechnung der kleinsten Differenz zwischen x und xAlt
                diff = xAlt.data[i] - x -> data[i];
                if(diff < 0) diff = diff * (-1); //Betrag der Differenz
                //Wenn die Differenz von einem der Werte des Vektors x kleiner oder gleich zur Fehlerschrankeist, kann aufgehört werden
                if(diff <= e) break;
                xAlt.data[i] = x -> data[i];
            }
            schritt++;
        } while(diff > e && schritt < maxSchritte);
    printf("Nach %d Schritten:\n\n",schritt);
}

int main() {
    Matrix A;
    Vector b;
    Vector x;
    double e;
    Method m;
    //Titel
    printf("\t ----------------------------\n\t|                            |\n\t|   LGS - Lösungsverfahren   |\n\t|    Jacobi / Gauß-Seidel    |\n\t|                            |\n\t ----------------------------\n\n");
    char nochmal;
    do{
        //Fragt Anwender nach .csv-Datei
        char filename[100];
        printf("Dateipfad der .csv-Datei: ");
        scanf("%s",filename);
        //wenn Datei ungültig, wird do-while-Schleife wieder wiederholt
        bool gueltig = load(filename, &A, &b, &x);
        if(gueltig){
            printf("Datei erfolgreich geladen.\n\n");
            char auswahl;
            do{//Fragt Anwender nach Verfahren
                printf("Welches Verfahren?\nSchreibe '0' für Jacobi oder '1' für Gauß-Seidel: ");
                scanf("%s",&auswahl);
                if(auswahl == '0') m = JACOBI;
                else if(auswahl == '1') m = GAUSS_SEIDEL;
                else printf(" - Ungültige Eingabe -\n\n");
            // Wenn Anwender etwas anderes als '0' oder '1' eingibt, wird er erneut nach Verfahren gefragt
            } while(auswahl != '0' && auswahl != '1');
            //Frage nach Fehlerschranke
            printf("\nWie klein soll die Fehlerschranke sein? (Bsp. 10⁻¹⁰ = 1e-10): ");
            scanf("%lf",&e);
            printf("\n");
            solve(m, &A, &b, &x, e);
            //Ausgabe des Vektors x
            for (int i = 0; i < x.n;i++)
                printf("\tx%d = %f\n", i+1, x.data[i]);
            printf("\nNoch eine Datei einlesen? [y/n]: ");
        }
        else printf("Die Datei konnte nicht geladen werden.\n\nNochmal versuchen? [y/n]: ");
        scanf("%s", &nochmal);
        //nuter wird solange gefragt bis er y für Ja oder n für nein eingibt
        while(nochmal != 'y' && nochmal != 'n'){
            printf(" - Ungültige Eingabe -\n\nNochmal versuchen? [y/n]: ");
            scanf("%s", &nochmal);
        }
    } while(nochmal == 'y');
    return 0;
}




