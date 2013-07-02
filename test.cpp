#include <cstdio>
#include "ADKConnection.hpp"
#include "Firmata.hpp"
#include "Arduino.hpp"
#include <iostream>
#include <unistd.h>

using namespace firmata;
int main(){
	Arduino * adk = new Arduino(new ADKConnection(), NULL);
	adk->pinMode(12,1);
	adk->pinMode(13,1);
	adk->digitalWrite(12, true);
	sleep(5);
	adk->digitalWrite(13, true);
}
