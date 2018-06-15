#include "euter/cellparameters.h"

#ifndef PYPLUSPLUS
#include "pyparameteraccess.h"

template <typename CellParameters>
struct CellParametersConstructors
{
	static boost::shared_ptr<CellParameters> from_dict(boost::python::dict parameters)
	{
		namespace bp = boost::python;
		boost::shared_ptr<CellParameters> p(new CellParameters);
		ParameterProxy proxy = getParameterProxy(*p);
		bp::list keys = parameters.keys();
		for (int ii = 0; ii < bp::len(keys); ii++) {
			bp::object key = keys[ii];
			proxy.set(bp::extract<std::string>(key), parameters[key]);
		}
		return p;
	}
};
#endif
