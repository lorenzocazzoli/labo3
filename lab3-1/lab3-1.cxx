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

// ! Vanno rinominati gli assi e cambiato il titolo del grafico
void calibrazione() {
    Double_t calibroMultiV[8] {98.0 , 196.0 , 295.0 , 396.0 , 497.0 , 595.0 , 683.0, 784.0};
    // l'errore sul multimetro è trascurabile rispetto a quello sull'oscilloscopio
    Double_t erroreCalibroMultiV[8] {0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01};
    Double_t calibroOscilloV[8] {100.0, 200.0, 300.0, 400.0, 500.0, 600.0, 700.0, 800.0};
    // l'errore effettivo sull'oscilloscopio segue una formula che non ricordo
    Double_t erroreCalibroOscilloV[8] {0.02, 0.05, 0.05, 0.1, 0.1, 0.1, 0.2, 0.2};
    TGraphErrors * voltaggioVoltaggio = new TGraphErrors();
    for(int i=0; i<8; i++) {
        voltaggioVoltaggio->SetPoint(i, calibroMultiV[i], calibroOscilloV[i]);
        voltaggioVoltaggio->SetPointError(i, erroreCalibroMultiV[i], erroreCalibroOscilloV[i]);
    }
    TF1 * correlazione = new TF1("correlazione", "[0]+[1]*x", 98, 800);
    correlazione->SetParameters(0,1);
    voltaggioVoltaggio->Fit("correlazione", "R"); // R option limits the fit in the range of the function
    // ricordarsi di cambiare i nomi degli assi inserendo le appropriate unità di misura
    // devono essere mV e mV, perché se no si hanno un po di problemini
    TCanvas * ccalibrazione = new TCanvas();
    voltaggioVoltaggio->Draw();
}

// ! Gli errori sono sbagliati, vanno controllate le formule per ottenere gli errori
// ! Vanno rinominati gli assi e cambiato il titolo del grafico
void silicio() {
    // array di valori I
    Double_t correnteMultiS[17] {0.01, 0.02, 0.05, 0.07, 0.11, 0.14, 0.16, 0.25, 0.36, 0.53, 0.76, 0.89, 1.11, 1.65, 1.92, 3.97, 7.99};
    // array di logI          
    Double_t correnteMultiLogS[17];            
    // riempimento array di valori logI
    for(int i=0; i<17; i++) {
        correnteMultiLogS[i] = log(correnteMultiS[i]);
    }
    Double_t erroreCorrenteMultiS[17] {0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01};
    Double_t voltaggioOscilloS[17] {400.0, 450.0, 500.0, 520.0, 540.0, 550.0, 560.0, 580.0, 600.0, 620.0, 640.0, 650.0, 660.0, 680.0, 700.0, 750.0, 800.0};
    Double_t erroreVoltaggioOscilloS[17] {0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.2, 0.2, 0.2};
    TGraphErrors * silicioIV = new TGraphErrors(17);
    for(int i=0; i<17; i++) {
        silicioIV->SetPoint(i, voltaggioOscilloS[i], correnteMultiLogS[i]);
        silicioIV->SetPointError(i, erroreVoltaggioOscilloS[i], erroreCorrenteMultiS[i]);
    }
    TF1 * silicioFunction = new TF1("silicioFunction", "[0]+[1]*x");
    silicioFunction->SetRange(540.0, -1.0, 650.0, -0.06);
    silicioFunction->SetParameters(0, 0.5);
    silicioIV->Fit("silicioFunction", "R"); // R option limits the fit in the range of the function
    // ricordarsi di cambiare i nomi degli assi inserendo le appropriate unità di misura
    // devono essere mV e mA
    TCanvas * cSilicio = new TCanvas(); 
    silicioIV->Draw();
}

// ! Gli errori sono sbagliati, vanno controllate le formule per ottenere gli errori
// ! Vanno rinominati gli assi e cambiato il titolo del grafico
// ! Per qualche motivo il range degli assi è completamente sballato e va a centinaia anche sulle y
void germanio() {
    // array di valori I
    Double_t correnteMultiG[16] {0.01, 0.02, 0.03, 0.04, 0.07, 0.11, 0.18, 0.26, 0.40, 0.58, 0.84, 1.21, 1.70, 2.34, 3.08, 5.17};
    // array di logI          
    Double_t correnteMultiLogG[16];            
    // riempimento array di valori logI
    for(int i=0; i<16; i++) {
        correnteMultiLogG[i] = log(correnteMultiG[i]);
    }
    // TODO controllare il range dell'asse Y!
    Double_t erroreCorrenteMultiG[16] {0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01};
    Double_t voltaggioOscilloG[16] {70, 90, 110, 130, 150, 170, 190, 210, 230, 250, 270, 290, 310, 330, 350, 400};
    Double_t erroreVoltaggioOscilloG[16] {0.05, 0.05, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.2, 0.2, 0.5};
    TGraphErrors * germanioIV = new TGraphErrors(17);
    for(int i=0; i<17; i++) {
        germanioIV->SetPoint(i, voltaggioOscilloG[i], correnteMultiLogG[i]);
        germanioIV->SetPointError(i, erroreVoltaggioOscilloG[i], erroreCorrenteMultiG[i]);
    }
    TF1 * germanioFunction = new TF1("germanioFunction", "[0]+[1]*x");
    germanioFunction->SetRange(100.0, -1.0, 300.0, 0.0);
    germanioFunction->SetParameters(0, 0.5);
    germanioIV->Fit("germanioFunction", "R"); // R option limits the fit in the range of the function
    // ricordarsi di cambiare i nomi degli assi inserendo le appropriate unità di misura
    // devono essere mV e mA
    TCanvas * cGermanio = new TCanvas(); 
    germanioIV->Draw();
}

void analisi() {
    /* impostazione grafica, con stile "Modern" e opzioni di visualizzazione del fit
       1111 (non mi ricordo cosa voglia dire e non ho voglia di controllare)*/
    setstyle();
    /* fit V-V con sull'asse x il voltaggio ottenuto sul multimetro
       e sull'asse y il voltaggio ottunto sull'oscilloscopio     */
    calibrazione();
    /* fit V-I con sull'asse x il voltaggio ottenuto sull'oscilloscopio
       e sull'asse y la corrente ottunta sul multimetro          */
    silicio();
    /* fit V-I con sull'asse x il voltaggio ottenuto sull'oscilloscopio
       e sull'asse y la corrente ottunta sul multimetro          */
    germanio();
}