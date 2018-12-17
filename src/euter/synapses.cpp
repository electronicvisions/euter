#include "euter/synapses.h"

BOOST_CLASS_EXPORT_IMPLEMENT(euter::TsodyksMarkramMechanism)
BOOST_CLASS_EXPORT_IMPLEMENT(euter::AdditiveWeightDependence)
BOOST_CLASS_EXPORT_IMPLEMENT(euter::MultiplicativeWeightDependence)
BOOST_CLASS_EXPORT_IMPLEMENT(euter::AdditivePotentiationMultiplicativeDepression)
BOOST_CLASS_EXPORT_IMPLEMENT(euter::GutigWeightDependence)
BOOST_CLASS_EXPORT_IMPLEMENT(euter::SpikePairRule)

namespace euter {

boost::shared_ptr<ShortTermPlasticityMechanism> SynapseDynamics::fast()
{
	return mFast;
}

boost::shared_ptr<const ShortTermPlasticityMechanism> SynapseDynamics::fast() const
{
	return mFast;
}

boost::shared_ptr<STDPMechanism> SynapseDynamics::slow()
{
	return mSlow;
}

boost::shared_ptr<const STDPMechanism> SynapseDynamics::slow() const
{
	return mSlow;
}

} // namespace euter
