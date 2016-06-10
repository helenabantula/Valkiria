//
//  User.cpp
//  Essencia_Milan
//
//  Created by Helena Bantulà i Fonts on 08/03/16.
//
//

#include "User.h"


User:: User(string soundfile1, string soundfile2, int position){
    
    soundUp.load(soundfile1,true);
    soundDown.load(soundfile2,true);
    
    sensorID=position;
    
    userState = STATE_INITIAL;

}


// Decidir quin estat s'executa
void User::update(){
    
    switch(userState){
        case STATE_INITIAL:
            initial();
            break;
        case STATE_WARMING:
            warming();
            break;
        case STATE_PLAY:
            if (isActive || isTransition)
            play();
            break;
        case STATE_STOP:
            stop();
            break;
        case STATE_WAIT:
            wait();
            break;
    }
    

}

void User::initial(){
    //Light::getInstance().randomPlay(true);
    // posar-se ACTIU
    
}


void User::warming(){
    
    // si han passat dos segons
    if ((ofGetElapsedTimeMillis() - timeSinceUser) > warmingTime) {
        
        if (ofGetElapsedTimeMillis() - timeSinceH < warmingTime) {       //i dins d'aquests 2 segons reps una H, canvia estat
            userState = STATE_PLAY;
            timeSincePlay = ofGetElapsedTimeMillis();
        }
        
        else  {                                                          // si no reps una H, torna a inici
            periodMean = periodMeanInit;
            period.clear();
            userState = STATE_INITIAL;
        }
    }

}


void User::play(){
    
    
    if ((ofGetElapsedTimeMillis() - timeSincePlay) >= periodMean) {
        
        // play light and sound
        soundUp.play();
        Light::getInstance().fadeUserPars(1, 'I', 2, 200, sensorID);       //fadeIn
        Light::getInstance().setUnusedPars(0, sensorID);
        
        // start again
        timeSincePlay = ofGetElapsedTimeMillis();
        playPhase = true;
    }
    
    if (((ofGetElapsedTimeMillis() - timeSincePlay) >= phaseMean) && playPhase){
        soundDown.play();
        Light::getInstance().fadeUserPars(1, 'O', 1, 200, sensorID);       //fadeOut
        Light::getInstance().setUnusedPars(0, sensorID);
        playPhase = false;
    }
    
    if (((ofGetElapsedTimeMillis()-timeSinceUser) > playTime) || (ofGetElapsedTimeMillis() - timeSinceH > maxErrorTime)) {
        //Light::getInstance().fadeUserPars(1, 'O', 1, warmingTime, sensorID); //dos segons de fadeOut
        Light::getInstance().setParState(0, sensorID);

        if(isAnotherUser){
            Light::getInstance().fadeUserPars(0.6, 'O', 0, stopTime, sensorID); //dos segons de fadeOut
        }
        else {
            Light::getInstance().equalFade(0.6, 'I', 0, stopTime);
        
        }
        //Light::getInstance().closeUser(sensorID);
        timeSinceLeft = ofGetElapsedTimeMillis();
        userState = STATE_STOP;
        cout<<"HE PARAT!!!!"<<endl;
    }
    
}


void User::stop(){
    if ((ofGetElapsedTimeMillis()-timeSinceLeft) > stopTime) {
        periodMean = periodMeanInit;
        period.clear();
        isActive = true;
        userState = STATE_WAIT;
        timeSinceStop = ofGetElapsedTimeMillis();

    }
}


void User::wait(){
    
    if ((ofGetElapsedTimeMillis()-timeSinceStop) > waitTime) {
        userState = STATE_INITIAL;
        cout<<"FUNCIONO_OFF"<<endl;
    }
}


void User::setHeartBeat(char value){

    
    if(((userState == STATE_INITIAL) && (value == 'H') && (ofGetElapsedTimeMillis() - timeSinceH) < maxPeriod)){    // asseguro H no falses
        //Light::getInstance().openUser(sensorID);
        
        if (!isAnotherUser)
            Light::getInstance().equalFade(1, 'O', 1, warmingTime);
        
        timeSinceUser = ofGetElapsedTimeMillis();
        //Light::getInstance().fadeUserPars(1, 'O', 1, warmingTime, sensorID);
        userState = STATE_WARMING;
        cout<<"HE ENTRAT"<<endl;
        

    }
    
    if (value == 'H'){
        
        // period
        tempPeriod = ofGetElapsedTimeMillis() - timeSinceH;
        
        if (tempPeriod > minPeriod && tempPeriod < maxPeriod){
            period.push_back(tempPeriod);
        }
        else {
            period.push_back(periodMeanInit);
        
        }
        
        // period mean
        periodMean = 0;
        for( int i = 0; i < period.size(); i++) {
            periodMean += period[i];
        }
        periodMean /= period.size();
        
        
        
        // tornem a començar
        timeSinceH = ofGetElapsedTimeMillis();
    
        // phase computation (always?)
        phaseMean = 0.25*periodMean;
    }
    
}

void User::changeActivity(){
    
    isActive = !isActive;

}



