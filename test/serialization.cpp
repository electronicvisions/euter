#include <iostream>
#include <sstream>
#include <random>
#include <numeric>

#include <gtest/gtest.h>

#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/for_each_product.hpp>
#include <boost/preprocessor/seq/pop_back.hpp>
#include <boost/preprocessor/variadic/to_seq.h>

#include <boost/make_shared.hpp>

#include "boost/serialization/serialization.h"
#include "boost/serialization/unique_ptr.h"

#include "euter/alltoallconnector.h"
#include "euter/connector.h"
#include "euter/nativerandomdistributions.h"
#include "euter/fixednumberpreconnector.h"
#include "euter/fixedprobabilityconnector.h"
#include "euter/fixednumberpostconnector.h"
//#include "euter/distancedependentprobabilityconnector.h"
#include "euter/fromlistconnector.h"
#include "euter/nativerandomgenerator.h"
#include "euter/objectstore.h"
#include "euter/onetooneconnector.h"
#include "euter/projection.h"
#include "test_utils.h"

using namespace euter;
using boost::serialization::make_nvp;

typedef std::tuple<boost::archive::xml_iarchive, boost::archive::xml_oarchive> XMLArchives;
typedef std::tuple<boost::archive::text_iarchive, boost::archive::text_oarchive> TextArchives;
typedef std::tuple<boost::archive::binary_iarchive, boost::archive::binary_oarchive> BinaryArchives;

#define ARCHIVES BOOST_PP_VARIADIC_TO_SEQ(XMLArchives, BinaryArchives)
#define CONNECTORS BOOST_PP_VARIADIC_TO_SEQ( \
	AllToAllConnector, FixedProbabilityConnector, FixedNumberPreConnector, \
        FixedNumberPostConnector, OneToOneConnector, FromListConnector)
#define RANDOM_DISTS BOOST_PP_VARIADIC_TO_SEQ( \
	NativeRandIntDistribution, NativeUniformDistribution, \
	NativeBinomialDistribution, NativePoissonDistribution, \
	NativeExponentialDistribution)


#define F_COMBINE(r, product) std::tuple< BOOST_PP_SEQ_ENUM(product) >,
#define AC_COMBINED BOOST_PP_VARIADIC_TO_SEQ(BOOST_PP_SEQ_FOR_EACH_PRODUCT(F_COMBINE, (ARCHIVES)(CONNECTORS) ) void)
#define RD_COMBINED BOOST_PP_VARIADIC_TO_SEQ(BOOST_PP_SEQ_FOR_EACH_PRODUCT(F_COMBINE, (ARCHIVES)(RANDOM_DISTS) ) void)

typedef ::testing::Types< BOOST_PP_SEQ_ENUM(ARCHIVES) > ArchiveTypeList;
typedef ::testing::Types< BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_POP_BACK(AC_COMBINED)) > ConnectorTypeList;
typedef ::testing::Types< BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_POP_BACK(RD_COMBINED)) > RandomDistTypeList;


template <typename T>
class SerializationTest;

template <typename I, typename O>
class SerializationTest<std::tuple<I, O> > : public ::testing::Test
{
public:
	typedef I IArchive;
	typedef O OArchive;

	SerializationTest() :
	    rng(boost::make_shared<NativeRandomGenerator>(22949142706))
	{}

protected:
	ObjectStore store;
	std::stringstream archive;
	std::unique_ptr<OArchive> oa;
	std::unique_ptr<IArchive> ia;
	boost::shared_ptr<RandomGenerator> rng;

	template <typename T>
	std::string transfer(T & in, T & out)
	{
		if (!oa) {
			oa.reset(new OArchive(archive));
		}
		(*oa) << make_nvp("object", in);
		oa.reset(); // flush
		archive.flush();
		std::string ret = archive.str();
		if (!ia) {
			ia.reset(new IArchive(archive));
		}
		(*ia) >> make_nvp("object", out);
		ia.reset();
		return ret;
	}

	// ECM (2018-07-25): Needed for tests needing type registration on living
	// archiver object.
	OArchive & getOA() {
		if (!oa) {
			oa.reset(new OArchive(archive));
		}
		return *oa;
	}
	IArchive & getIA() {
		if (!ia) {
			ia.reset(new IArchive(archive));
		}
		return *ia;
	}
	ObjectStore & getStore() { return store; }
	boost::shared_ptr<RandomGenerator>  & getRng() { return rng; }
};

TYPED_TEST_SUITE(SerializationTest, ArchiveTypeList);

TYPED_TEST(SerializationTest, CellParameterVector)
{
	// ECM(2018-07-25): OArchive only writes upon destruction but we have to register
	// all types first... cf. getOA/IA functions above

	// Register types by hand, otherwise this is done by Population::serialize
	RegisterCellTypeForSerializationVisitor<
	        typename TestFixture::OArchive, TypedCellParameterVector> vo(TestFixture::getOA());
	forallCellTypes(vo);

	RegisterCellTypeForSerializationVisitor<
	        typename TestFixture::IArchive, TypedCellParameterVector> vi(TestFixture::getIA());
	forallCellTypes(vi);

	std::unique_ptr<CellParameterVector> p_in, p_out;
	p_in = CellParameterVector::create(200, CellType::IF_brainscales_hardware);
	TestFixture::transfer(p_in, p_out);
	ASSERT_EQ(p_in->size(), p_out->size());
	ASSERT_EQ(p_out->type(), CellType::IF_brainscales_hardware);
}

TYPED_TEST(SerializationTest, Population)
{
	boost::shared_ptr<Population> p_in, p_out;
	p_in = Population::create(TestFixture::getStore(), 200, CellType::IF_brainscales_hardware);
	TestFixture::transfer(p_in, p_out);
	ASSERT_EQ(p_in->size(), p_out->size());
	ASSERT_EQ(p_out->parameters().type(), CellType::IF_brainscales_hardware);
}

TYPED_TEST(SerializationTest, DCSource)
{
	boost::shared_ptr<CurrentSource> src_in, src_out;
	src_in = DCSource::create(TestFixture::getStore(), 1.0, 2.0, 3.0);
	TestFixture::transfer(src_in, src_out);
	ASSERT_EQ(
			*boost::dynamic_pointer_cast<DCSource>(src_in),
			*boost::dynamic_pointer_cast<DCSource>(src_out)
			);
}

TYPED_TEST(SerializationTest, StepCurrentSource)
{
	boost::shared_ptr<CurrentSource> src_in, src_out;

	ublas::vector<double> times(1337), amplitudes(1337);
	std::iota(times.begin(), times.end(), 0);
	std::iota(amplitudes.begin(), amplitudes.end(), 42);

	src_in = StepCurrentSource::create(TestFixture::getStore(), times, amplitudes);
	TestFixture::transfer(src_in, src_out);
	
	ASSERT_EQ(
			*boost::dynamic_pointer_cast<StepCurrentSource>(src_in),
			*boost::dynamic_pointer_cast<StepCurrentSource>(src_out)
			);
}

TYPED_TEST(SerializationTest, ACSource)
{
	boost::shared_ptr<CurrentSource> src_in, src_out;
	src_in = ACSource::create(TestFixture::getStore(), 1.0, 2.0, 3.0, 4.0, 5.0, 6.0);
	TestFixture::transfer(src_in, src_out);
	ASSERT_EQ(
			*boost::dynamic_pointer_cast<ACSource>(src_in),
			*boost::dynamic_pointer_cast<ACSource>(src_out)
			);
}

TYPED_TEST(SerializationTest, NoisyCurrentSource)
{
	boost::shared_ptr<CurrentSource> src_in, src_out;
	src_in = NoisyCurrentSource::create(TestFixture::getStore(), 1.0, 2.0, 3.0, 4.0, 5.0);
	TestFixture::transfer(src_in, src_out);
	ASSERT_EQ(
			*boost::dynamic_pointer_cast<NoisyCurrentSource>(src_in),
			*boost::dynamic_pointer_cast<NoisyCurrentSource>(src_out)
			);
}

TYPED_TEST(SerializationTest, NativeRandomGenerator)
{
	static const size_t seed = 325131883u;
	static const size_t samples = 623;

	boost::shared_ptr<NativeRandomGenerator> rng_in, rng_out;
	rng_in = boost::make_shared<NativeRandomGenerator>(seed);

	TestFixture::transfer(rng_in, rng_out);

	ASSERT_TRUE(ContainerEqual(rng_in->next(samples), rng_out->next(samples)));
}


TYPED_TEST(SerializationTest, UblasVector)
{
	ublas::vector<double> v_in(125000);
	for (unsigned ii = 0; ii < v_in.size(); ++ ii)
		v_in(ii) = 3 * ii + 2.5;
	v_in(5) = std::numeric_limits<double>::quiet_NaN();

	ublas::vector<double> v_out;
	TestFixture::transfer(v_in, v_out);
	ASSERT_TRUE(ContainerEqual(v_in.data(), v_out.data()));
}

TYPED_TEST(SerializationTest, UblasMatrix)
{
	ublas::matrix<double> m_in(200, 500);
	for (unsigned ii = 0; ii < m_in.size1(); ++ ii)
		for (unsigned jj = 0; jj < m_in.size2(); ++ jj)
			m_in(ii, jj) = 3 * ii + jj;
	m_in(5,25) = std::numeric_limits<double>::quiet_NaN();

	ublas::matrix<double> m_out;
	TestFixture::transfer(m_in, m_out);
	ASSERT_TRUE(ContainerEqual(m_in.data(), m_out.data()));
}

/*
 * Tests for euter/synapse.h
 */

// thingy specifies the struct while param is a random parameter of this struct
#define SERIALIZABLE_PARAMETERIZED_STRUCT_TEST(thingy, param)			\
TYPED_TEST(SerializationTest, thingy)									\
{																		\
	std::mt19937 gen;													\
	thingy in, out;														\
	in.mParams.param = gen();											\
	TestFixture::transfer(in, out);										\
	ASSERT_EQ(in.mParams.param, out.mParams.param);						\
}

SERIALIZABLE_PARAMETERIZED_STRUCT_TEST(TsodyksMarkramMechanism, U)
SERIALIZABLE_PARAMETERIZED_STRUCT_TEST(AdditiveWeightDependence, w_min)
SERIALIZABLE_PARAMETERIZED_STRUCT_TEST(MultiplicativeWeightDependence, w_min)
SERIALIZABLE_PARAMETERIZED_STRUCT_TEST(AdditivePotentiationMultiplicativeDepression, w_min)
SERIALIZABLE_PARAMETERIZED_STRUCT_TEST(GutigWeightDependence, w_min)
SERIALIZABLE_PARAMETERIZED_STRUCT_TEST(SpikePairRule, tau_plus)

TYPED_TEST(SerializationTest, SynapseDynamics)
{
	std::mt19937 gen;

	auto t_dep = boost::make_shared<SpikePairRule>();
	auto w_dep = boost::make_shared<AdditiveWeightDependence>();
	auto slow = boost::make_shared<STDPMechanism>(t_dep, w_dep, gen());

	auto fast = boost::make_shared<TsodyksMarkramMechanism>();
	fast->mParams.U = gen();

	SynapseDynamics in(fast, slow), out;
	
	TestFixture::transfer(in, out);

	ASSERT_NE(nullptr, in.fast());
	ASSERT_NE(nullptr, in.slow());
	ASSERT_EQ(in.fast()->get("U"), out.fast()->get("U"));
	ASSERT_EQ(in.slow()->mWeightDependence->get("w_min"), out.slow()->mWeightDependence->get("w_min"));
	ASSERT_EQ(in.slow()->mTimingDependence->get("tau_plus"), out.slow()->mTimingDependence->get("tau_plus"));
}

/*
 * Tests for connector stuff
 */

template <typename T>
class ConnectSerializationTest
{

};

template <typename Archives, typename C>
class ConnectSerializationTest< std::tuple<Archives, C> > :
        public SerializationTest<Archives>
{
public:
	typedef C ConnectorType;

protected:

};

template <typename T>
struct MakeConnector
{
	static boost::shared_ptr<T> get()
	{ return boost::make_shared<T>(); }
};

template <>
struct MakeConnector<FixedProbabilityConnector>
{
	typedef FixedProbabilityConnector T;
	static boost::shared_ptr<T> get()
	{ return boost::make_shared<T>(0.5); }
};

template <>
struct MakeConnector<FromListConnector>
{
	typedef FromListConnector T;
	static boost::shared_ptr<T> get()
	{
		std::vector<typename T::Connection> connections =
		{{0,1},{1,0},{3,5}};
		return boost::make_shared<T>(std::move(connections));
	}
};


TYPED_TEST_SUITE(ConnectSerializationTest, ConnectorTypeList);

TYPED_TEST(ConnectSerializationTest, Connector)
{
	typedef typename TestFixture::ConnectorType ConnectorType;

	auto source_ptr = MakeConnector<ConnectorType>::get();
	decltype(source_ptr) target_ptr;
	boost::shared_ptr<Connector> source_base_ptr = source_ptr;
	boost::shared_ptr<Connector> target_base_ptr;

	ASSERT_NO_THROW(TestFixture::transfer(source_ptr, target_ptr));
	// Serialize using the base type
	ASSERT_NO_THROW(TestFixture::transfer(source_base_ptr, target_base_ptr));

	// Test if printOn leads to the same strings
	std::stringstream source_name, target_name, base_target_name;
	source_ptr->printOn(source_name);
	target_ptr->printOn(target_name);
	target_base_ptr->printOn(base_target_name);
	ASSERT_EQ(source_name.str(), target_name.str());
	ASSERT_EQ(source_name.str(), base_target_name.str());

	// Check if we can cast back
	ASSERT_TRUE(static_cast<bool>(boost::dynamic_pointer_cast<ConnectorType>(target_base_ptr)));
}


TYPED_TEST(ConnectSerializationTest, Projection)
{
	typedef typename TestFixture::ConnectorType ConnectorType;

	boost::shared_ptr<Population> p1, p2;
	p1 = Population::create(TestFixture::store, 10, CellType::IF_brainscales_hardware);
	p2 = Population::create(TestFixture::store, 10, CellType::IF_brainscales_hardware);
	auto c = MakeConnector<ConnectorType>::get();
	auto p_in = Projection::create(TestFixture::store, p1, p2, c, TestFixture::getRng(), "", "");
	decltype(p_in) p_out;

	TestFixture::transfer(p_in, p_out);
	ASSERT_EQ(p_in->getWeights().elements(), p_in->getWeights().elements());
	ASSERT_EQ(p_in->getWeights().get().size1(), p_in->getWeights().get().size2());
	ASSERT_EQ(p_in->getWeights().get().size1(), p_in->getWeights().get().size2());
	ASSERT_TRUE(ContainerEqual(p_in->getWeights().get().data(),
	                           p_out->getWeights().get().data()));
	// FIXME test on equalty ASSERT_THAT(p_in->getWeights().data(), ContainerEq(p_out->getWeights().data()));
}

template <typename T>
class RandomDistributionSerializationTest {};

template <typename Archives, typename RandDist>
class RandomDistributionSerializationTest< std::tuple<Archives, RandDist> > :
        public SerializationTest<Archives>
{
public:
	typedef RandDist RandDistType;
};

TYPED_TEST_SUITE(RandomDistributionSerializationTest, RandomDistTypeList);

TYPED_TEST(RandomDistributionSerializationTest, NativeRandomGeneratorDistributions)
{
	typedef typename TestFixture::RandDistType RandDistType;
	typedef typename RandDistType::result_type value_type;

	const size_t seed = 325131883u;
	const size_t samples = 623;

	boost::shared_ptr<NativeRandomGenerator> rng;
	rng = boost::make_shared<NativeRandomGenerator>(seed);

	boost::shared_ptr<RandDistType> dist_in, dist_out;
	dist_in = boost::make_shared<RandDistType>(rng);

	TestFixture::transfer(dist_in, dist_out);

	std::vector<value_type> data1(samples);
	std::vector<value_type> data2(samples);
	dist_in->next(data1);
	dist_out->next(data2);

	ASSERT_EQ(data1, data2);
}
