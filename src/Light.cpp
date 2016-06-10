//
//  light.cpp
//  Corasonsitu
//
//  Created by Helena Bantulà i Fonts on 17/11/15.
//
//

#include "Light.h"


void Light::initialize(int numUsersIni){
#if TARGET_RASPBERRY_PI
    artnet.setup("192.168.1.109"); //IP de rPi
#else
    artnet.setup("192.168.1.10"); //IP ordinador
#endif    
 
//    //ofColor  color(200,60,100);
//    ofColor color(0,0,255);
//    colors.push_back(color);
//    
//    
//    
//    
//    //color.set(200, 60, 100);
//    color.set(255,0,0);
//    colors.push_back(color);
//    
//    color.set(255,255,0);
//    colors.push_back(color);
    
    
    ofColor color(255,0,30);
    colors.push_back(color);
    
    color.set(255,20,40);
    colors.push_back(color);
    
    color.set(255,50,60);
    colors.push_back(color);
    
    color.set(255,30,20);
    colors.push_back(color);
    
    color.set(255,28,25);
    colors.push_back(color);
    
    color.set(0,0,0);
    colors.push_back(color);
    
    color.set(255,255,255);
    colors.push_back(color);
    
    
    color.set(255,30,56);
    colors.push_back(color);
    
    color.set(255,28,25);
    colors.push_back(color);
    
    color.set(255,0,0);
    colors.push_back(color);
    
    color.set(255,28,30);
    colors.push_back(color);
    
    color.set(255,28,25);
    colors.push_back(color);
    
    color.set(0,0,0);
    colors.push_back(color);
    
    color.set(255,255,255);
    colors.push_back(color);
    
    
    
        for (int i = 0; i < maxLight; i++){
            Par temPar(colors[i],i);
            leds.push_back(temPar);
        }
    

//    // COLOR 1
//    for (int i = 0; i < maxPar/2; i++){
//        Par temPar(colors[0],i);
//        leds.push_back(temPar);
//    }
//    
//    Par temPar(colors[2],maxPar/2);
//    leds.push_back(temPar);
//    
//    
//    
//    // COLOR 2
//    for (int i = (maxPar/2 + 1); i < maxPar +1; i++){
//        Par temPar(colors[1],i);
//        leds.push_back(temPar);
//    }
//    
//    Par temPar1(colors[2], maxPar +2);
//    leds.push_back(temPar1);
    
    
    
    

    numUsers = numUsersIni;
    parXuser = maxLight/numUsers;
    
    vector<vector<int>> tempMatrix(numUsers, vector<int>(parXuser));
    parUserAssign = tempMatrix;
    
    vector<bool> tempCurrentUsers(numUsers);
    currentUsers = tempCurrentUsers;

    vector<unsigned char> tempPackDMX(maxLight*3);
    packDMX = tempPackDMX;
    
    sendInfo();
    
    for (int i = 0; i < numUsers; i++){
        for (int j = 0; j < (parXuser); j++){
            parUserAssign[i][j] = i*(parXuser) +j;
        }
        
    }
}


void Light::assignPars(){
    
    parXuser = maxLight/numCurrentUsers;
    
    vector<vector<int>> tempMatrix(numCurrentUsers, vector<int>(parXuser));
    parUserAssign = tempMatrix;

    
    for (int i = 0; i < numCurrentUsers; i++){
        for (int j = 0; j < (parXuser); j++){
            parUserAssign[i][j] = i*(parXuser) +j;
        }
        
    }


}


void Light::getInfo(){
    
    int j=0;
    for (int i = 0; i < maxPar/2; i++){
        leds[i].update();
        
        packDMX[j] = leds[i].getColor().r;
        packDMX[j+1] = leds[i].getColor().g;
        packDMX[j+2] = leds[i].getColor().b;
        j=j+3;
        //cout<<maxPar<<endl;
    }
    
    for (int i = (maxPar/2 +1); i < maxPar + 1; i++){
        leds[i].update();
        
        packDMX[j] = leds[i].getColor().r;
        packDMX[j+1] = leds[i].getColor().g;
        packDMX[j+2] = leds[i].getColor().b;
        j=j+3;
        //cout<<maxPar<<endl;
    }
    
    leds[6].update();
    packDMX[36] = leds[6].getColor().r;
    packDMX[37] = leds[6].getColor().g;
    packDMX[38] = leds[6].getColor().b;
    
    leds[13].update();
    packDMX[39] = leds[13].getColor().r;
    packDMX[40] = leds[13].getColor().g;
    packDMX[41] = leds[13].getColor().b;
    
    this->sendInfo();
}


void Light::sendInfo(){
    
    artnet.sendDmx("192.168.1.151",&packDMX[0], packDMX.size());
}


void Light::equalFade(float k, char fade, int type, int step){
    

    for (int i=0; i<leds.size(); i++){
        leds[i].topFade = k;
        
        if (fade=='I'){
            leds[i].triggerFadeIn(type, step);
        }
        else if (fade == 'O') {
            leds[i].triggerFadeOut(step);
        }
    }
}

void Light::fadeUserPars(float k, char fade, int type, int step, int user){
    
    vector<int> usePars = parUserAssign[user];      //vector de pars de lusuari, si parUserAssign és fixe!
    
    
    for (int i = 0; i < usePars.size(); i++){
        leds[i].topFade = k;
        
        if (fade=='I'){
            leds[usePars[i]].triggerFadeIn(type, step);
        }
        else if (fade == 'O') {
            leds[usePars[i]].triggerFadeOut(step);
        }
    }
    
}


void Light::fadeUnusedPars(float k, char fade, int type, int step, int users){
    
    
    vector<int> noUsers(numUsers - 1);                      // non-user indexs
    vector<int> unusedPars(noUsers.size()*(parXuser));      // non-used pars
    
    int j = 0;
    
    for (int i = 0; i < numUsers; i++){
            if (i != users) {
                noUsers[j] = i;
                j++;
            }
    }
    
    int z = 0;
    for (int i = 0; i < noUsers.size(); i++) {
        for (int j = 0; j < parXuser; j++){
            unusedPars[z] = parUserAssign[noUsers[i]][j];
            z ++;
        }
    }

    
    for (int i = 0; i < unusedPars.size(); i++){
        leds[unusedPars[i]].topFade = k;
        
        if (fade == 'I'){
            leds[unusedPars[i]].triggerFadeIn(type, step);
        }
        else if (fade == 'O') {
            leds[unusedPars[i]].triggerFadeOut(step);
        }
    }


}

void Light::randomPlay(bool state){

    for (int i=0; i<leds.size(); i++){
        leds[i].isRandom = state;
    }
}



void Light::openUser(int userID){
    
    currentUsers[userID] = true;
    numCurrentUsers ++;
    //fadeUnusedPars(1, 'O', 1, 2000, userID);        //tots els usuaris que estiguin funcionant
    
//    numCurrentUsers = 0;
//    for (int i = 0; i < currentUsers.size(); i++){
//        if (currentUsers[i])
//            numCurrentUsers ++;
//    }
//    
    //assignPars();
}

void Light::closeUser(int userID){
    
    currentUsers[userID] = false;
    
//    int auxCurrentUsers = 0;
//    for (int i = 0; i < currentUsers.size(); i++){
//        if (currentUsers[i])
//            auxCurrentUsers ++;
//    }
    
    numCurrentUsers --;
    
    //assignPars();
    
}


bool Light::isUserOpened(int userID){
    
    if (currentUsers[userID])
        return true;
    else
        return false;
}


void Light::setParState(float k, int user){
    
    vector<int> usePars = parUserAssign[user];      //vector de pars de lusuari, si parUserAssign és fixe!
    
    
    for (int i = 0; i < usePars.size(); i++){
        leds[i].setColor(k);

    }


}

void Light::setUnusedPars(float k, int user){
    
    
    vector<int> noUsers(numUsers - 1);                      // non-user indexs
    vector<int> unusedPars(noUsers.size()*(parXuser));      // non-used pars
    //vector<int> unusedPars;
    int j = 0;
    
    for (int i = 0; i < numUsers; i++){
        if (i != user) {
            noUsers[j] = i;
            j++;
        }
    }
    
    int z = 0;
    for (int i = 0; i < noUsers.size(); i++) {
        for (int j = 0; j < parXuser; j++){
            unusedPars[z] = parUserAssign[noUsers[i]][j];
            z ++;
        }
    }
    
    
    for (int i = 0; i < unusedPars.size(); i++){
        leds[unusedPars[i]].setColor(k);

    }

}


