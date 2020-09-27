#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string>

class TrbHeader
{
public:
	std::string signature;
	uint32_t magic;
	uint16_t flag1;
	uint16_t flag2;
	uint32_t dataInfoCount;
	uint32_t dataInfoSize;
	uint32_t tagCount;
	uint32_t tagSize;
	uint32_t relocationDataOffset;
	uint32_t relocationDataSize;

public:
	TrbHeader(std::string signature, uint32_t magic, uint16_t flag1, uint16_t flag2, uint32_t dataInfoCount, uint32_t dataInfoSize, uint32_t tagCount, uint32_t tagSize,
	          uint32_t relocationDataOffset, uint32_t relocationDataSize);
private:
	void setSignature(std::string sig);
	void setMagic(uint32_t magic);


};

