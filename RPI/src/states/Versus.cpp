#include <iostream>
#include <unistd.h>
#include "states/Versus.hpp"

/* 
class: Versus
Implementation of the Versus State's member functions
*/

Versus::Versus(GameThreadFunctor* gameTF, int selectedMap){
	selectedMap_ = selectedMap;
	this->gameTF = gameTF;
	playerOneShots_ = NUMBER_OF_VERSUS_SHOTS/2;
	playerTwoShots_ = NUMBER_OF_VERSUS_SHOTS/2;

	menuName_ = "versus";
	map_ = Gamerules::generateMap(selectedMap_);

	sendMapInd(gameTF->getCommMq(), map_.getMapString());
	sendUARTCommandInd(gameTF->getCommMq(), '+');
	sendArmedInd(gameTF->getPrintMq(), true, menuName_);
	sendShowInd(gameTF->getPrintMq(), menuName_);
	std::cout << "Versus: ctor - fire to enter new state" << std::endl;
}

Versus::~Versus(){
	std::cout << "Versus: dtor" << std::endl;
	sendCleanInd(gameTF->getPrintMq(), menuName_);
}

void Versus::up(){
	std::cout << "Versus: up" << std::endl;
}

void Versus::down(){
	std::cout << "Versus: down" << std::endl;
}

void Versus::left(){
	std::cout << "Versus: left" << std::endl;
}

void Versus::right(){
	std::cout << "Versus: right" << std::endl;
}

void Versus::fire(){
	if (returning_) {
		std::cout << "Versus: fire - Returning was true, returning to Idle" << std::endl;
		gameTF->setCurrent(new Idle(this->gameTF));
		delete this;
	} else {
		sendArmedInd(gameTF->getPrintMq(), false, menuName_);
		sendHitReq(gameTF->getCommMq());
	}
}

void Versus::hit(int field){
	std::cout << "Versus: hit" << std::endl;

	int pointsGained = Gamerules::getPoints(map_, field);

	if (pointsGained == 0){
		sendMissInd(gameTF->getPrintMq(), menuName_);
	} else {
		map_.disableField(field);
		sendMapInd(gameTF->getCommMq(), map_.getMapString());
	}

	if(currentPlayer_ == 1){
		//Player 1 was the shooter
		playerOnePoints_ += pointsGained;
		sendPointsInd(gameTF->getPrintMq(), currentPlayer_, playerOnePoints_);
		playerOneShots_--;
		sendShotsInd(gameTF->getPrintMq(), currentPlayer_, playerOneShots_);
		currentPlayer_ = 2;
	} else {
		//Player 2 was the shooter
		playerTwoPoints_ += pointsGained;
		sendPointsInd(gameTF->getPrintMq(), currentPlayer_, playerTwoPoints_);
		playerTwoShots_--;
		sendShotsInd(gameTF->getPrintMq(), currentPlayer_, playerTwoShots_);
		currentPlayer_ = 1;
	}

	/*
		HUSK BESKED MED RETURN POPUP OG NOGET MED AT HEJSE/SÆNKE FLAG PÅ PSOC VED RETURN
	*/


	if (playerTwoShots_ == 0){		
		sleep(3);
		gameTF->setCurrent(new Results(this->gameTF, selectedMap_, playerOnePoints_, playerTwoPoints_));
		delete this;
		//End game stuff
	} else {
		sendNextPlayerInd(gameTF->getPrintMq(), currentPlayer_);
		sendUARTCommandInd(gameTF->getCommMq(), '+');
		sendArmedInd(gameTF->getPrintMq(), true, menuName_);
	}
}