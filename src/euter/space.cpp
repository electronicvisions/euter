#include "euter/space.h"

#include <algorithm>
#include <cmath>
#include <random>

namespace euter {

Space::Space() : Space(
		std::bitset<3>(Axis::X | Axis::Y | Axis::Z),
		1.0,
		static_cast<coord_type>(ublas::zero_vector<distance_type>(3)),
		Boundaries()
		)
{
}

Space::Space(std::bitset<3> axes, double scale_factor, coord_type offset, Boundaries boundaries)
{
    // Create the projection matrix from the axes bitset.
    // Refer to Space::mask for more information.
    ublas::vector<int> d(3);
    for(auto i=0u; i<d.size(); i++)
        d(i) = axes[i];
    mProj = ublas::diagonal_matrix<int>(3, d.data());

	mScaleFactor = scale_factor;
	mOffset = offset;
	mBoundaries = boundaries;
}

// Apply the space's projection matrix (e.g. diag([1, 1, 0])) to the given vector.
Space::coord_type Space::project(coord_type &v)
{
    return ublas::prod(mProj, v);
}

// Calculate the distance between two coordinates according to the metric of the
// space object.
Space::distance_type Space::distance(coord_type &v, coord_type &w)
{
	coord_type w_tmp = mScaleFactor * (w + mOffset);
    coord_type d = project(v) - project(w_tmp);

	for(size_t i=0; i<=2; ++i)
	{
		d[i] = std::min(std::abs(d[i]), (mBoundaries[i].second - mBoundaries[i].first) - std::abs(d[i]));
	}

    return sqrt(ublas::inner_prod(d, d));
}

// Calculate the distance matrix for two given sets of vectors.
ublas::matrix<Space::distance_type> Space::distances(ublas::vector<coord_type> &a, ublas::vector<coord_type> &b)
{
    size_t const m = a.size();
    size_t const n = b.size();

    ublas::matrix<distance_type> distance_matrix(m, n);

    for(size_t i=0; i<m; i++)
    {
        for(size_t j=0; j<n; j++)
        {
            distance_matrix(i, j) = distance(a(i), b(j));
        }
    }

    return distance_matrix;
}

Structure::Structure() :
	offset(),
	fillOrder()
{}

Structure::Structure(coord_type const& offset, FillOrder const fillOrder) :
	offset(offset),
	fillOrder(fillOrder)
{}

Structure::~Structure() {}

ublas::vector<SpatialTypes::coord_type> Structure::generatePositions(size_t n) const
{
    ublas::vector<coord_type> positions = _generatePositions(n);
    switch(fillOrder)
    {
        case FillOrder::Sequential:
            break;
        case FillOrder::Random:
			std::shuffle(positions.begin(), positions.end(), m_rng);
		default:
            break;
    }
    return positions;
}


Line::Line() :
	Line(1.0, ublas::zero_vector<distance_type>(3), FillOrder::Sequential)
{}


Line::Line(distance_type marginX, coord_type offset, FillOrder fillOrder) :
	Structure(offset, fillOrder),
	marginX(marginX)
{}

ublas::vector<SpatialTypes::coord_type> Line::_generatePositions(size_t n) const
{
    ublas::vector<coord_type> positions(n);
    for(size_t i=0; i<n; i++)
        positions(i) = i * marginX * ublas::unit_vector<distance_type>(3, 0) + offset;

    return positions;
}


Grid2D::Grid2D() :
Grid2D(1.0, 1.0, 1.0, ublas::zero_vector<distance_type>(3), FillOrder::Sequential)
{}

Grid2D::Grid2D(distance_type ratioY) :
Grid2D(1.0, 1.0, ratioY, ublas::zero_vector<distance_type>(3), FillOrder::Sequential)
{}

Grid2D::Grid2D(distance_type marginX, distance_type marginY, distance_type ratioY, coord_type offset, FillOrder fillOrder) :
	Structure(offset, fillOrder),
	marginX(marginX), marginY(marginY), ratioY(ratioY)
{}

ublas::bounded_vector<size_t, 3> Grid2D::calculateSize(size_t n) const
{
    if(sqrt(n*ratioY) != floor(sqrt(n*ratioY)))
        throw "Invalid size, stupid!";

    ublas::bounded_vector<size_t, 3> size;
    size(0) = sqrt(n*ratioY);
    size(1) = n/size(0);
    size(2) = 0;
    return size;
}

ublas::vector<SpatialTypes::coord_type> Grid2D::_generatePositions(size_t n) const
{
    auto const size = calculateSize(n);

    ublas::vector<coord_type> positions(n);
    for(size_t i=0; i<size(0); i++)
    {
        for(size_t j=0; j<size(1); j++)
        {
            positions(i*size(1)+j) =
                    i * marginX * ublas::unit_vector<distance_type>(3, 0) +
                    j * marginY * ublas::unit_vector<distance_type>(3, 1) +
                    offset;
        }
    }

    return positions;
}


Grid3D::Grid3D(distance_type ratioY, distance_type ratioZ) :
	Grid3D(1.0, 1.0, 1.0, ratioY, ratioZ, ublas::zero_vector<distance_type>(3), FillOrder::Sequential)
{}

Grid3D::Grid3D() :
	Grid3D(1.0, 1.0, 1.0, 1.0, 1.0, ublas::zero_vector<distance_type>(3), FillOrder::Sequential)
{}

Grid3D::Grid3D(distance_type marginX, distance_type marginY, distance_type marginZ,
            distance_type ratioY, distance_type ratioZ,
            coord_type offset, FillOrder fillOrder) :
	Structure(offset, fillOrder),
	marginX(marginX),
	marginY(marginY),
	marginZ(marginZ),
	ratioY(ratioY),
	ratioZ(ratioZ)
{}

ublas::bounded_vector<size_t, 3> Grid3D::calculateSize(size_t n) const
{
    if(pow(n*ratioY*ratioZ, 1/3.0) != floor(pow(n*ratioY*ratioZ, 1/3.0)))
        throw "Invalid size, stupid!";

    ublas::bounded_vector<size_t, 3> size;
    size(0) = round(pow(n*ratioY*ratioZ, 1/3.0));
    size(1) = size(0)/ratioY;
    size(2) = size(0)/ratioZ;
    return size;
}

ublas::vector<SpatialTypes::coord_type> Grid3D::_generatePositions(size_t n) const
{
    auto const size = calculateSize(n);

    ublas::vector<coord_type> positions(n);
    for(size_t i=0; i<size(0); i++)
    {
        for(size_t j=0; j<size(1); j++)
        {
            for(size_t k=0; k<size(2); k++)
            {
                positions(i*size(1)*size(2)+j*size(2)+k) =
                        i * marginX * ublas::unit_vector<distance_type>(3, 0) +
                        j * marginY * ublas::unit_vector<distance_type>(3, 1) +
                        k * marginZ * ublas::unit_vector<distance_type>(3, 2) +
                        offset;
            }
        }
    }

    return positions;
}

/*
 * The following code is not intended to be pretty or efficient but to replicate
 * PyNN's Shape code.
 * It would be a lot more beautiful to provide an interface for defining probability
 * densities and calculate a RandomStructure of neurons according to those.
 * Well, here's some shitty code instead. That's how life works, right?
 */

Cuboid::Cuboid(distance_type width, distance_type height, distance_type depth) :
	width(width), height(height), depth(depth)
{}

ublas::vector<SpatialTypes::coord_type> Cuboid::sample(size_t n)
{
    ublas::vector<coord_type> positions(n);

    std::mt19937 gen;
    std::uniform_real_distribution<> dis(-0.5, 0.5);

    for(auto i=positions.begin(); i<positions.end(); ++i)
    {
        (*i)(0) = dis(gen) * width;
        (*i)(1) = dis(gen) * height;
        (*i)(2) = dis(gen) * depth;
    }

    return positions;
}


Sphere::Sphere(distance_type radius) :
	radius(radius)
{}

ublas::vector<SpatialTypes::coord_type> Sphere::sample(size_t n)
{
    ublas::vector<coord_type> positions(n);

    std::mt19937 gen;
    std::uniform_real_distribution<> dis(-1, 1);

    auto i = positions.begin();
    while(i<positions.end())
    {
        distance_type x = dis(gen);
        distance_type y = dis(gen);
        distance_type z = dis(gen);

        if(sqrt(x*x + y*y + z*z) <= 1)
        {
            (*i)(0) = x;
            (*i)(1) = y;
            (*i)(2) = z;
            (*i) *= radius;

            i++;
        }
    }

    return positions;
}

ublas::vector<SpatialTypes::coord_type> RandomStructure::_generatePositions(size_t n) const
{
	ublas::vector<coord_type> positions = mShape->sample(n);

	for(auto i=positions.begin(); i<positions.end(); ++i)
	{
		(*i) += offset;
	}

	return positions;
}

} // namespace euter
