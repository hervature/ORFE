/**
@file  ptpricers.hpp
@brief Declaration of portfolio pricing functions
*/

#include <orflib/defines.hpp>
#include <orflib/exception.hpp>
#include <orflib/math/matrix.hpp>
#include <tuple>

BEGIN_NAMESPACE(orf)

/** Mean return and standard deviation of return of a portfolio */
std::tuple<double, double> ptRisk(Vector const& weights,
                                  Vector const& assetRets,
                                  Vector const& assetVols,
                                  Matrix const& correlMat);

/** Weights of the minimum variance portfolio */
Vector mvpWeights(Vector const& assetRets, Vector const& assetVols, Matrix const& correlMat);

/** Weights of the CAPM market portfolio */
Vector mktWeights(Vector const& assetRets, Vector const& assetVols, Matrix const& correlMat, double rfreeRate);

/** Mean return, standard deviation of return, and lambda of the CAPM market portfolio */
std::tuple<double, double, double> 
mktRisk(Vector const& assetRets, Vector const& assetVols, Matrix const& correlMat, double rfreeRate);

/** Weights of the mean variance efficient portfolio */
Vector meanVarWeights(Vector const& assetRets, Vector const& assetVols, Matrix const& correlMat, double riskAversion);

/** Mean return, standard deviation of return, and lambda of the mean variance efficient portfolio */
std::tuple<double, double, double>
meanVarFront(Vector const& assetRets, Vector const& assetVols, Matrix const& correlMat, double riskAversion);


END_NAMESPACE(orf)
