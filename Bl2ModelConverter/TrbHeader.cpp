#include "TrbHeader.h"

TrbHeader::TrbHeader(std::string signature, uint32_t magic, uint16_t flag1, uint16_t flag2, uint32_t dataInfoCount, uint32_t dataInfoSize, uint32_t tagCount, uint32_t tagSize,
					 uint32_t relocationDataOffset, uint32_t relocationDataSize)
{
	setSignature(signature);
	setMagic(magic);
	this->flag1 = flag1;
	this->flag2 = flag2;
	this->dataInfoCount = dataInfoCount;
	this->dataInfoSize = dataInfoSize;
	this->tagCount = tagCount;
	this->tagSize = tagSize;
	this->relocationDataOffset = relocationDataOffset;
	this->relocationDataSize = relocationDataSize;
}

void TrbHeader::setSignature(std::string signature)
{
	if (strcmp(signature.c_str(),"TRB") == 0)
	{
		this->signature = signature;
	}
	else
	{
		throw "Not a TRB file";
	}
}

void TrbHeader::setMagic(uint32_t magic)
{
	if (magic == 2001)
	{
		this->magic = magic;
	}
	else
	{
		throw "Unsupported Magic";
	}
}