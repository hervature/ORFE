#include <orflib/math/matrix.hpp>
#include <xlw/xlw.h>

/** Converts a row or column XlfOper range to an orf::Vector.
    It ignores any leading or trailing cells that are empty.
    The passed-in range must be 1xn or nx1.
*/
orf::Vector xlOperToVector(xlw::XlfOper xlVec);

/** Converts an orf::Matrix to an XlfOper
*/
xlw::XlfOper xlVectorToOper(orf::Vector const & vec);

/** Converts a rectangular XlfOper range to an orf::Matrix.
    It ignores any leading or trailing rows or columns that are empty.
*/
orf::Matrix xlOperToMatrix(xlw::XlfOper xlMat);

/** Converts an orf::Matrix to an XlfOper
*/
xlw::XlfOper xlMatrixToOper(orf::Matrix const & mat);
