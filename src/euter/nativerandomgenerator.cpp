#include "euter/nativerandomgenerator.h"

#include <sstream>

std::string NativeRandomGenerator::getState() const
{
	std::stringstream stream;
	stream << mRng;
	return stream.str();
}

void NativeRandomGenerator::setState(const std::string & state)
{
	std::stringstream stream(state);
	stream >> mRng;
}

BOOST_CLASS_EXPORT_IMPLEMENT(NativeRandomGenerator)
