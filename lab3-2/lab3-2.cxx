// C++ libraries
#include <algorithm>
#include <iostream>
#include <cmath>

// ROOT libraries
#include "TMath.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TCanvas.h"

void setstyle() {
    gROOT->SetStyle("Modern");
    gStyle->SetOptFit(1111);
}

// * Ricordarsi che tutti i valori di voltaggio e corrente sono negativi, ma scritti qui in valore assoluto
// TODO Segnare negli assi che i valori sono in -V e -mA
void correnteTransistor() {
    // essendo la corrente misurata col multimetro il valore non cambia
    Double_t erroreCorrente = 0.01;

    // ------------ //
    //     200mA    //
    // ------------ //

    // Valori ottenuti con una corrente sulla base del transistor di 200 mA
    // Voltaggio letto tra Collettore ed Emettitore in configurazione ad emettitore comune
    Double_t voltaggio200[18] {4.0, 3.5, 3.0, 2.5, 2.0, 1.5, 1.0, 0.8, 0.6, 0.5, 0.4, 0.3, 0.275, 0.25, 0.2, 0.15, 0.1, 0.05};
    // la scala del voltaggio letto sull'oscilloscopio è indicata in Volt e rappresenta la corrispondenza in Volt di un quadretto sull'Oscilloscopio
    Double_t scalaVoltaggio200[18] {1.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.2, 0.2, 0.2, 0.1, 0.1, 0.1, 0.05, 0.05, 0.05, 0.05, 0.02, 0.02};
    Double_t corrente200[18] {36.9, 36.12, 35.16, 34.20, 33.20, 31.45, 30.91, 30.18, 29.20, 27.85, 26.64, 23.51, 20.37, 19.44, 19.40, 9.90, 4.38, 0.67};
    Double_t erroreVoltaggio200[18] {}; 
    for(int i=0; i<18; i++){
        erroreVoltaggio200[i] = scalaVoltaggio200[i]/5;
    }
    // inserimento dei valori per corrente in B = 200 nel TGraphErrors
    TGraphErrors * studio200 = new TGraphErrors(18);
    for (int i=0; i<18; i++) {
        studio200->SetPoint(i, voltaggio200[i], corrente200[i]);
        studio200->SetPointError(i, erroreVoltaggio200[i], erroreCorrente);
    }
    TF1 * funzione200 = new TF1("funzione200", "[0]+x*[1]");
    // impostare il range per un fit lineare così che non comprenda la decrescita esponenziale
    funzione200->SetRange(1, 4.0);
    // imboccare root che se no fa i capricci
    funzione200->SetParameters(25, 0.05);
    // fit della funzione con limite uguale al range della funzione
    studio200->Fit("funzione200", "R");
    
    // ------------ //
    //     100mA    //
    // ------------ //

    // Valori ottenuti con una corrente sulla base del transistor di 100 mA 
    // Voltaggio letto tra Collettore ed Emettitore in configurazione ad emettitore comune
    Double_t voltaggio100[18] {4.0, 3.5, 3.0, 2.5, 2.0, 1.5, 1.0, 0.8, 0.6, 0.5, 0.4, 0.3, 0.275, 0.25, 0.2, 0.15, 0.1, 0.05};
    // la scala del voltaggio letto sull'oscilloscopio è indicata in Volt e rappresenta la corrispondenza in Volt di un quadretto sull'Oscilloscopio
    Double_t scalaVoltaggio100[18] {1.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.2, 0.2, 0.2, 0.1, 0.1, 0.1, 0.05, 0.05, 0.05, 0.05, 0.02, 0.02};
    Double_t corrente100[18] {18.18, 18.06, 17.83, 17.48, 17.05, 16.57, 16.06, 15.80, 15.48, 15.16, 14.68, 13.41, 12.67, 11.86, 9.28, 5.63, 2.05, 0.27};
    Double_t erroreVoltaggio100[18] {};
    for(int i=0; i<18; i++){
        erroreVoltaggio100[i] = scalaVoltaggio100[i]/5;
    }
    // inserimento dei valori per corrente in B = 100 nel TGraphErrors
    TGraphErrors * studio100 = new TGraphErrors(18);
    for (int i=0; i<18; i++) {
        studio100->SetPoint(i, voltaggio100[i], corrente100[i]);
        studio100->SetPointError(i, erroreVoltaggio100[i], erroreCorrente);
    }
    TF1 * funzione100 = new TF1("funzione100", "[0]+x*[1]");
    // impostare il range per un fit lineare così che non comprenda la decrescita esponenziale
    funzione100->SetRange(1, 4.0);
    // imboccare root che se no fa i capricci
    funzione100->SetParameters(15, 0.05);
    // fit della funzione con limite uguale al range della funzione
    studio100->Fit("funzione100", "R");

    // ------------ //
    //    Grafica   //
    // ------------ //

    TCanvas * canvas = new TCanvas(); // costruttore di base perché chi se ne frega
    // TODO distinguere i due grafici in maniera visuale, con colori diversi del fit magari
    // impostazione grafica per distinguere i due TGraph e rispettivi fit
    //studio200->;
    //studio100->;
    studio200->SetTitle("Curve caratteristiche Vce-Ic; -Vce (Volt); -Ic (mA)");
    // disegno dei grafici
    studio200->Draw("APE");
    studio100->Draw("PE");
    // stampa canvas in formato pdf
    canvas->Print("correnticaratteristiche.pdf");
}

void analisi() {
    /* impostazione grafica, con stile "Modern" e opzioni di visualizzazione del fit
       1111 (non mi ricordo cosa voglia dire e non ho voglia di controllare)*/
    setstyle();
    /* corenteTransistor analisi curve caratteristiche per IB1 ed IB2 della corrente
       passante per il circuito a diverse differenze di potenziale*/
    correnteTransistor();
}