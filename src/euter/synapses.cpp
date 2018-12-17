#include "euter/synapses.h"

BOOST_CLASS_EXPORT_IMPLEMENT(TsodyksMarkramMechanism)
BOOST_CLASS_EXPORT_IMPLEMENT(AdditiveWeightDependence)
BOOST_CLASS_EXPORT_IMPLEMENT(MultiplicativeWeightDependence)
BOOST_CLASS_EXPORT_IMPLEMENT(AdditivePotentiationMultiplicativeDepression)
BOOST_CLASS_EXPORT_IMPLEMENT(GutigWeightDependence)
BOOST_CLASS_EXPORT_IMPLEMENT(SpikePairRule)

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
