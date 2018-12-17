#pragma once

#ifndef PYPLUSPLUS
#include <random>
#endif
#include <utility>
#include <bitset>
#include <boost/numeric/ublas/banded.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include "boost/serialization/serialization.h"
#include "boost/serialization/ublas.hpp"


namespace euter {

// FIXME: Space objects need serialization code and serialization EXPORTS,
// otherwise population serialization does not properly work.

namespace ublas = boost::numeric::ublas;

struct SpatialTypes
{
    typedef double                                  distance_type;
    typedef ublas::bounded_vector<distance_type, 3> coord_type;

	struct Boundaries : ublas::bounded_vector<std::pair<double, double>, 3>
	{
		Boundaries()
		{
			double nan = std::numeric_limits<distance_type>::quiet_NaN();
			for(size_t i=0; i<=2; ++i)
				operator[](i) = std::make_pair(nan, nan);
		}
	};
};

class Space : public SpatialTypes
{
public:
    enum Axis
    {
        X = (1 << 0),
        Y = (1 << 1),
        Z = (1 << 2)
    };

    Space();
    Space(std::bitset<3> axes,
		  double scale_factor,
		  coord_type offset,
		  Boundaries boundaries
		  );

    coord_type project(coord_type &v);
    distance_type distance(coord_type &v, coord_type &w);
    ublas::matrix<distance_type> distances(ublas::vector<coord_type> &a,
        ublas::vector<coord_type> &b);
private:
    ublas::diagonal_matrix<int> mProj;
	double mScaleFactor;
	coord_type mOffset;
	Boundaries mBoundaries;
};

class Structure : public SpatialTypes
{
public:
    enum FillOrder
    {
        Sequential,
        Random
    };

    Structure();
    Structure(coord_type const& offset, FillOrder fillOrder);
    virtual ~Structure();

    ublas::vector<coord_type> generatePositions(size_t n) const;
protected:
    virtual ublas::vector<coord_type> _generatePositions(size_t n) const = 0;

    coord_type offset;
    FillOrder fillOrder;

private:
#ifndef PYPLUSPLUS
	mutable std::mt19937 m_rng;
#endif
};

class Line : public Structure
{
public:
    Line();
    Line(distance_type marginX, coord_type offset, FillOrder fillOrder);
private:
    ublas::vector<coord_type> _generatePositions(size_t n) const;
    distance_type marginX;
};

class Grid2D : public Structure
{
public:
    Grid2D();
	Grid2D(distance_type ratioY);
    Grid2D(distance_type marginX, distance_type marginY, distance_type ratioY, coord_type offset, FillOrder fillOrder);
private:
    ublas::vector<coord_type> _generatePositions(size_t n) const;
    ublas::bounded_vector<size_t, 3> calculateSize(size_t n) const;

    distance_type marginX;
    distance_type marginY;
    distance_type ratioY;
};

class Grid3D : public Structure
{
public:
    Grid3D();
	Grid3D(distance_type ratioY, distance_type ratioZ);
    Grid3D(distance_type marginX, distance_type marginY, distance_type marginZ,
            distance_type ratioY, distance_type ratioZ,
            coord_type offset, FillOrder fillOrder);
private:
    ublas::vector<coord_type> _generatePositions(size_t n) const;
    ublas::bounded_vector<size_t, 3> calculateSize(size_t n) const;

    distance_type marginX;
    distance_type marginY;
    distance_type marginZ;
    distance_type ratioY;
    distance_type ratioZ;
};

class Shape : public SpatialTypes
{
public:
    virtual ublas::vector<coord_type> sample(size_t n) = 0;
};

class Cuboid : public Shape
{
public:
    Cuboid(distance_type width, distance_type height, distance_type depth);
    virtual ublas::vector<coord_type> sample(size_t n);
private:
    ublas::vector<coord_type> _sample(size_t n);

    distance_type width;
    distance_type height;
    distance_type depth;
};

class Sphere : public Shape
{
public:
    Sphere(distance_type radius);
    virtual ublas::vector<coord_type> sample(size_t n);
private:
    ublas::vector<coord_type> _sample(size_t n);

    distance_type radius;
};

class RandomStructure : public Structure
{
public:
    RandomStructure(const boost::shared_ptr<Shape>& shape) : mShape(shape)
	{
		this->offset = ublas::zero_vector<distance_type>(3);
	}

    RandomStructure(const boost::shared_ptr<Shape>& shape, coord_type offset) : mShape(shape)
    {
        this->offset = offset;
    }
private:
    ublas::vector<coord_type> _generatePositions(size_t n) const;

	boost::shared_ptr<Shape> mShape;
};

} // namespace euter
