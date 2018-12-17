#include "euter/nativerandomgenerator.h"

#include <sstream>

namespace euter {

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

} // namespace euter

BOOST_CLASS_EXPORT_IMPLEMENT(euter::NativeRandomGenerator)
