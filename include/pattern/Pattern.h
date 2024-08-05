#ifndef PATTERN_H
#define PATTERN_H

class Pattern
{
public:
	virtual void processFrame(uint32_t* colorArray, double dt) = 0;

};