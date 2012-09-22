/*
	-------------------------------------------------------------------------------
	Name			:   CurviloftAlong.rb
	Original Date	:   27 June 2012
	Author			:	Kevin Adinata, Robotics Club @ SJSU
	Version(s)		:	v 1.0 (27 June 2012)
	Description		:   A Library that provides a communication tool for Arduino 
						to control a Pololu Mini Maestro Servo.
	-------------------------------------------------------------------------------
	THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
	IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
	-------------------------------------------------------------------------------
*/

#ifndef MaestroServo_h
#define MaestroServo_h

#include "Arduino.h"

#include "SoftwareSerial.h"

class MaestroServo
{
	public:
		MaestroServo();
		MaestroServo( unsigned char devID );
		MaestroServo( SoftwareSerial& swSerial );
		void setAngle ( unsigned char channel, int angle );
		void setSpeed ( unsigned char channel, int speed );
		void setAcceleration ( unsigned char channel, int accel );
		void goHome();
		void selectProtocol(int p);
		void connect();
		void connect( long baud );
		bool isConnected();
		int getAngle( unsigned char channel );
		
	private:
		SoftwareSerial *softSerial;
		unsigned char device;
		bool connected;
		bool usingSoftSerial;
		int protocol;
		void sendHeader();
		void send( unsigned char sendable );
		int getInt();
};

#endif