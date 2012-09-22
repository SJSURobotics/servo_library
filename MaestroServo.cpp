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

#include "Arduino.h"
#include "MaestroServo.h"
#include "SoftwareSerial.h"

#define COMMAND_BYTE  0xAA
#define SERVO_MIN	  1000
#define SERVO_MAX	  2000

#define DEFAULT_BAUD 57600

// constructor, using default device ID
MaestroServo::MaestroServo()	{
	device = 0x0C;
	connected = false;
	usingSoftSerial = false;
	protocol = 0;
}
///*
// constructor, using user-specified device ID
MaestroServo::MaestroServo( unsigned char devID )	{
	device = devID;
	usingSoftSerial = false;
	connected = false;
	protocol = 0;
}
//*/
MaestroServo::MaestroServo( SoftwareSerial& swSerial )	{
	device = 0x0C;
	connected = false;
	usingSoftSerial = true;
	protocol = 0;
	softSerial = &swSerial;
}

void MaestroServo::send( unsigned char sendable ) {
	if ( usingSoftSerial ) {
		softSerial -> write( sendable );
	} else {
		Serial.write( sendable );
	}
}

// initiate serial communication using user-specified baudrate
void MaestroServo::connect( long baud )	{
	if (usingSoftSerial)	{
		softSerial -> begin( baud );
		connected = true;
	}	else	{
		Serial.begin( baud );
		connected = true;
	}
}

// initiate serial communication using default baudrate
void MaestroServo::connect()	{
	connect( DEFAULT_BAUD );
}

void MaestroServo::selectProtocol(int p)	{
	protocol = p;	// 0 -> pololu, 1 -> compact
}

// Send command byte and device ID (pololu protocol)
void MaestroServo::sendHeader()	{
	send( COMMAND_BYTE );
	send( device );
}

// set the angle of the servo connected to the specified channel
void MaestroServo::setAngle ( unsigned char channel, int angle )	{
	int us =  4 * map( angle, 0, 180, SERVO_MIN, SERVO_MAX );		// convert unit from degrees to quarter-microseconds
	noInterrupts();
	if ( protocol != 0 ) {
		send( 0x84 );
	} else {
		sendHeader();
		send( 0x04 );
	}
	send( channel );
	send( us & 0x7F );
	send( (us >> 7) & 0x7F );
	interrupts();
}

// set the speed limit of the servo connected to the specified channel
void MaestroServo::setSpeed ( unsigned char channel, int speed )	{
	int spd = 0;		// @TODO: convert speed from degree/second into (0.25 µs)/(10 ms)
	//noInterrupts();
	if ( protocol != 0 ) {
		send( 0x87 );
	} else {
		sendHeader();
		send( 0x07 );
	}
	send( channel );
	send( spd & 0x7F );
	send( (spd >> 7) & 0x7F );
	//interrupts();
}

// set the acceleration limit of the servo connected to the specified channel
void MaestroServo::setAcceleration ( unsigned char channel, int accel )	{
	int acc = 0;		// @TODO: convert accel from degree/second/second into (0.25 µs)/(10 ms)/(80 ms)
	//noInterrupts();
	if ( protocol != 0 ) {
		send( 0x89 );
	} else {
		sendHeader();
		send( 0x09 );
	}
	send( channel );
	send( acc & 0x7F );
	send( (acc >> 7) & 0x7F );
	//interrupts();
}

void MaestroServo::goHome()	{
	if ( protocol != 0 ) {
		send( 0xA2 );
	} else {
		sendHeader();
		send( 0x22 );
	}
}

// read the angle of the servo connected to the specified channel
int MaestroServo::getAngle( unsigned char channel )	{
	noInterrupts();
	if ( protocol != 0 ) {
		send( 0x90 );
		//send( 0xA1 );
	} else {
		sendHeader();
		send( 0x10 );
		//send ( 0x21 );
	}
	send( channel );
	interrupts();
	return getInt();
}

// parse integers coming from the servo controller
int MaestroServo::getInt()	{
	int temp = 0;
	if ( usingSoftSerial ) {
		// SoftwareSerial is used.
		while ( !softSerial -> available() );	// Wait to receive LSB.
		temp = softSerial -> read();
		while ( !softSerial -> available() );	// Wait to receive MSB.	
		temp = ((( softSerial -> read() ) << 8 ) | temp );
	} else {
		while ( !Serial.available() );	// Wait to receive LSB.
		temp = Serial.read();
		while ( !Serial.available() );	// Wait to receive MSB.	
		temp = ((( Serial.read() ) << 8 ) | temp );
	}
	return temp;
}

// connection status. true = Serial has been initiated, false otherwise.
bool MaestroServo::isConnected()	{
	return connected;
}