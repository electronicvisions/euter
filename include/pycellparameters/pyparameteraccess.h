#pragma once

#include <vector>

#include <boost/python.hpp>

struct ParameterAccess
{
	typedef boost::python::object object;

	virtual ~ParameterAccess() = default;

	virtual void get(const std::string & name, object & obj) const = 0;
	virtual void set(const std::string & name, const object & obj) const = 0;
};


struct GetPythonObject
{
	GetPythonObject(boost::python::object & obj) : obj(obj) {}

	template <typename Parameter>
	void operator()(const Parameter & p) const
	{
		static_assert(!std::is_same<Parameter, GetPythonObject>::value, "");
		obj = boost::python::object(p);
	}

	boost::python::object & obj;
};


struct SetPythonObject
{
	SetPythonObject(const boost::python::object & obj) : obj(obj) {}

	template <typename Parameter>
	void operator()(Parameter & p) const
	{
		static_assert(!std::is_same<Parameter, SetPythonObject>::value, "");
		p = boost::python::extract<Parameter>(obj);
	}

	template <typename Parameter>
	void operator()(const Parameter & ) const
	{
		throw std::runtime_error("Accessing read only object");
	}

	const boost::python::object & obj;
};

template <typename Parameters>
class ParameterAccessImpl : public ParameterAccess
{
public:
	typedef boost::python::object object;

	ParameterAccessImpl(Parameters & p) : parameters(p) {}

	virtual void set(const std::string & name, const object & obj) const
	{
		parameters.get(name, SetPythonObject(obj));
	}

	virtual void get(const std::string & name, object & obj) const
	{
		parameters.get(name, GetPythonObject(obj));
	}

private:
	Parameters & parameters;
};

class ParameterProxy
{
public:
	typedef boost::python::object object;

	ParameterProxy() {}
	ParameterProxy(std::unique_ptr<ParameterAccess> && data) :
	    access(std::move(data)) {}

	void get(const std::string & name, boost::python::object & obj) const
	{
		access->get(name, obj);
	}

	void set(const std::string & name, const boost::python::object & obj) const
	{
		access->set(name, obj);
	}

protected:
	std::unique_ptr<ParameterAccess> access;
};

template <typename CellParameters>
ParameterProxy getParameterProxy(CellParameters & p)
{
	std::unique_ptr<ParameterAccess> proxy(
	            new ParameterAccessImpl<CellParameters>(p));
	return ParameterProxy(std::move(proxy));
}

namespace euter {
class PopulationView;
} // namespace euter

ParameterProxy getPyParameter(euter::PopulationView & p, size_t index);
const ParameterProxy getPyParameter(const euter::PopulationView & p, size_t index);

std::vector<ParameterProxy> getPyParameterVector(euter::PopulationView & p);
const std::vector<ParameterProxy> getPyParameterVector(const euter::PopulationView & p);
