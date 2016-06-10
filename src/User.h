//
//  User.hpp
//  Essencia_Milan
//
//  Created by Helena Bantulà i Fonts on 08/03/16.
//
//

#ifndef User_h
#define User_h

#include <stdio.h>
#include "ofMain.h"
#include "Light.h"


#endif /* User_hpp */

enum userStateType{
    
    STATE_INITIAL,
    STATE_WARMING,
    STATE_PLAY,
    STATE_STOP,
    STATE_WAIT
};

class User{
    
public:
    
    int sensorID;
    //int pars[2];
    bool active;
    ofSoundPlayer soundUp;
    ofSoundPlayer soundDown;
    char heartBeat;
    
    //TIME MANAGEMENT
    int timeSinceUser;
    int timeSinceLeft;
    int timeSincePlay;
    int timeSinceStop;
    int playTime = 40000;
    int warmingTime = 2000; //canviat
    int stopTime = 4000;
    int waitTime = 1000;
    int maxErrorTime = 4000;
    
    //PERIOD COMPUTATION
    int timeSinceH = 0;
    //int timeSinceL;
    vector<int> period;
    int tempPeriod;
    int periodMean;
    int periodMeanInit = 857;   // 70  bpm
    int maxPeriod = 1000;       // 60  bpm
    int minPeriod = 300;        // 200 bpm
    
    //PHASE COMPUTATION
    int phaseMean;
    bool playPhase = true;
    
    
    //CONTROL STATE
    void update();
    userStateType userState;
    
    //CONTROL ACTiVE/INACTIVE
    bool isActive = true;
    bool isTransition = false;
    bool isAnotherUser = false;
    
    
    
    //INHERENT
    User(string soundfile1, string soundfile2, int position);
    
    
    //STATES
    void initial();
    void warming();
    void play();
    void stop();
    void wait();
    void setHeartBeat(char value);
    
    void changeActivity();
    
    void setActive(bool activity){ isActive = activity;};
    void setTransition(bool transition){ isTransition = transition;};
    void setAnotherUser(bool isAnother){ isAnotherUser = isAnother;};
    
    int getTimeSinceUser(){return timeSinceUser;};
    userStateType getUserState(){return userState;};
    void setTimeSinceUser(int time){timeSinceUser = time;};
    
    


};