#include "euter/celltypes.h"

#include <boost/preprocessor/seq/transform.hpp>

#define CELL_TYPE_TO_STRING(s, data, elem) { BOOST_PP_STRINGIZE(elem) }

#define CELL_TYPE_TO_MAP(s, data, elem) { BOOST_PP_STRINGIZE(elem), CellType:: elem },

namespace euter {

const std::map<std::string, CellType> nameToCellType = {
    BOOST_PP_SEQ_FOR_EACH(CELL_TYPE_TO_MAP, ~, CELL_TYPE_SEQ)
};

std::string getCellTypeName(CellType t)
{
	static std::vector<std::string> names = {
	    BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM(CELL_TYPE_TO_STRING, ~, CELL_TYPE_SEQ)) };
	return names[static_cast<size_t>(t)];
}

} // namespace euter
