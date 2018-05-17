#include <orflib/math/matrix.hpp>
#include <xlw/xlw.h>

/** The tick and tock are unprintable characters; in Excel they look like a little square.
    We append the tick/tock at the end of every object name to trigger Excel recalculation
*/
enum XlTickTock
{
  XL_TICK = 164,
  XL_TOCK = 176
};

/** Adds the tick at the end of the string name
*/
std::string xlAddTick(std::string const & name);

/** Strips the tick from the string name;
returns the portion of the string up to the tick.
*/
std::string xlStripTick(std::string const & name);

/** Converts a row or column XlfOper range to an orf::Vector.
    It ignores any leading or trailing cells are empty.
    The passed-in range must be 1xn or nx1.
*/
orf::Vector xlOperToVector(xlw::XlfOper xlVec);

/** Converts an orf::Matrix to an XlfOper
*/
xlw::XlfOper xlVectorToOper(orf::Vector const & vec, bool colMajor = true);

/** Converts a rectangular XlfOper range to an orf::Matrix.
    It ignores any leading or trailing rows or columns that are empty.
*/
orf::Matrix xlOperToMatrix(xlw::XlfOper xlMat);

/** Converts an orf::Matrix to an XlfOper
*/
xlw::XlfOper xlMatrixToOper(orf::Matrix const & mat);
