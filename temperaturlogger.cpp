#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

class TemperaturLogger {
private:
    double messwerte[24];  // 24 Stunden = 24 Messwerte
    int anzahlMessungen;
    string standort;
    double warnGrenze;
    
public:
    // Konstruktor
    TemperaturLogger(string ort, double grenze = 35.0) {
        standort = ort;
        warnGrenze = grenze;
        anzahlMessungen = 0;
        cout << "Temperaturlogger gestartet für: " << standort << "\n";
        cout << "Warngrenze: " << warnGrenze << "°C\n\n";
    }
    
    // Messwert hinzufügen
    void messungHinzufuegen(double temperatur) {
        if (anzahlMessungen >= 24) {
            cout << "Speicher voll! Maximal 24 Messungen pro Tag.\n";
            return;
        }
        
        messwerte[anzahlMessungen] = temperatur;
        anzahlMessungen++;
        
        cout << "Messung " << anzahlMessungen << ": " << temperatur << "°C";
        
        // Sofortige Warnung bei Überschreitung
        if (temperatur > warnGrenze) {
            cout << " WARNUNG! Grenzwert überschritten!";
        }
        cout << "\n";
    }
    
    // Analyse aller Messwerte
    void analysiereDaten() {
        if (anzahlMessungen == 0) {
            cout << "Keine Messdaten vorhanden.\n";
            return;
        }
        
        double summe = 0;
        double maxTemp = messwerte[0];
        double minTemp = messwerte[0];
        int ueberschreitungen = 0;
        
        for (int i = 0; i < anzahlMessungen; i++) {
            summe += messwerte[i];
            
            if (messwerte[i] > maxTemp) maxTemp = messwerte[i];
            if (messwerte[i] < minTemp) minTemp = messwerte[i];
            if (messwerte[i] > warnGrenze) ueberschreitungen++;
        }
        
        double durchschnitt = summe / anzahlMessungen;
        
        // Ergebnisse anzeigen
        cout << "\nANALYSEBERICHT für " << standort << "\n";
        cout << "========================================\n";
        cout << "Messungen heute: " << anzahlMessungen << "\n";
        cout << "Durchschnitt: " << fixed << setprecision(1) << durchschnitt << "°C\n";
        cout << "Maximum: " << maxTemp << "°C\n";
        cout << "Minimum: " << minTemp << "°C\n";
        cout << "Grenzwertüberschreitungen: " << ueberschreitungen << "\n";
        cout << "========================================\n";
    }
    
    // Daten in Datei speichern
    void speichereDaten() {
        string dateiname = "temperatur_" + standort + ".txt";
        ofstream datei(dateiname);
        
        if (!datei) {
            cout << "Fehler beim Speichern!\n";
            return;
        }
        
        datei << "TEMPERATURLOGGER - " << standort << "\n";
        datei << "Datum: 2026-03-06\n";
        datei << "================================\n";
        
        for (int i = 0; i < anzahlMessungen; i++) {
            datei << "Messung " << (i+1) << ": " << messwerte[i] << "°C\n";
        }
        
        datei.close();
        cout << "Daten gespeichert in: " << dateiname << "\n";
    }
    
    // Prüfen auf kritische Temperaturänderungen
    void prüfeTemperaturTrend() {
        if (anzahlMessungen < 3) {
            cout << "Zu wenig Daten für Trendanalyse.\n";
            return;
        }
        
        double letzteDrei[3];
        for (int i = 0; i < 3; i++) {
            letzteDrei[i] = messwerte[anzahlMessungen - 3 + i];
        }
        
        // Steigt die Temperatur schnell?
        if (letzteDrei[2] > letzteDrei[1] && letzteDrei[1] > letzteDrei[0]) {
            double anstieg = letzteDrei[2] - letzteDrei[0];
            if (anstieg > 5.0) {
                cout << "KRITISCH: Temperatur steigt schnell! +" << anstieg << "°C in 3 Messungen\n";
            } else {
                cout << "Trend: Temperatur steigt langsam\n";
            }
        }
        // Sinkt die Temperatur?
        else if (letzteDrei[2] < letzteDrei[1] && letzteDrei[1] < letzteDrei[0]) {
            cout << "Trend: Temperatur fällt\n";
        } else {
            cout << "Trend: Stabil oder schwankend\n";
        }
    }
};

int main() {
    cout << "\n========================================\n";
    cout << "   TECHNISCHES TEMPERATUR-MONITORING\n";
    cout << "========================================\n\n";
    
    // Logger für Serverraum erstellen
    TemperaturLogger serverraum("Serverraum RZ-1", 32.0);
    
    // Simulierte Messungen (Technik-Umgebung)
    double messreihe[] = {23.5, 24.1, 25.8, 27.2, 29.5, 31.8, 33.2, 34.0, 33.5, 32.1};
    
    for (int i = 0; i < 10; i++) {
        serverraum.messungHinzufuegen(messreihe[i]);
    }
    
    // Analyse durchführen
    serverraum.analysiereDaten();
    serverraum.prüfeTemperaturTrend();
    serverraum.speichereDaten();
    
    cout << "\nMonitoring abgeschlossen.\n";
    return 0;
}