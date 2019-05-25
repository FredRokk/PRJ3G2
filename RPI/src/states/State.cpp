#include <iostream>

#include "states/State.hpp"

/* 
class: State
Implementation of the State interface's member functions
*/

State::~State(){
	std::cout << "Default: dtor" << std::endl;
}

void State::up(){
	std::cout << "Default: up" << std::endl;
}

void State::down(){
	std::cout << "Default: down" << std::endl;
}
void State::left(){
	std::cout << "Default: left" << std::endl;
}
void State::right(){
	std::cout << "Default: right" << std::endl;
}
void State::onePlayer(){
	std::cout << "Default: onePlayer" << std::endl;
}
void State::twoPlayer(){
	std::cout << "Default: twoPlayer" << std::endl;
}
void State::back(){
	std::cout << "Default: back" << std::endl;
	if(returning_){
		std::cout << "MapSelect: return - returning is true, setting returning to false" << std::endl;
		returning_ = false;
	} else {
		std::cout << "Mapselect: return - setting return to true" << std::endl;
		returning_ = true;
	}
}
void State::fire(){
	std::cout << "Default: fire" << std::endl;
}
void State::hit(int field){
	std::cout << "Default: hit" << std::endl;
}

void State::miss(){
	std::cout << "Default: miss" << std::endl;
}

void State::sendShowInd(osapi::MsgQueue* mq, std::string menu){
	GameShowMenuInd* showMsg = new GameShowMenuInd();
	showMsg->menu = menu;
	mq->send(ID_GAME_SHOW_MENU_IND, showMsg);
}

void State::sendCleanInd(osapi::MsgQueue* mq, std::string menu){
	GameCleanMenuInd* cleanMsg = new GameCleanMenuInd();
	cleanMsg->menu = menu;
	mq->send(ID_GAME_CLEAN_MENU_IND, cleanMsg);
}

void State::sendMapInd(osapi::MsgQueue* mq, std::string map){
	GameI2CSendMapInd* mapMsg = new GameI2CSendMapInd();
	mapMsg->map = map;
	mq->send(ID_GAME_I2C_SEND_MAP_IND, mapMsg);
}

void State::sendUARTCommandInd(osapi::MsgQueue* mq, char command){
	GameUARTCommandInd* commandMsg = new GameUARTCommandInd();
	commandMsg->command = command;
	mq->send(ID_GAME_UART_COMMAND_IND, commandMsg);
}

void State::sendHitReq(osapi::MsgQueue* mq){
	GameI2CGetHitReq* hitMsg = new GameI2CGetHitReq();
	mq->send(ID_GAME_I2C_GET_HIT_REQ, hitMsg);
}