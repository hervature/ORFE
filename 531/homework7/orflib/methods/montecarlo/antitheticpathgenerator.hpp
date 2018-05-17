/**
@file  antitheticpathgenerator.hpp
@brief class for antithetic Monte Carlo path generators
*/

#ifndef ORF_ANTITHETICPATHGENERATOR_HPP
#define ORF_ANTITHETICPATHGENERATOR_HPP

#include <orflib/methods/montecarlo/pathgenerator.hpp>

BEGIN_NAMESPACE(orf)

using SPtrPathGenerator = std::shared_ptr<PathGenerator>;

class AntitheticPathGenerator : public PathGenerator {
public:
	// Keep track of the generated path
	Matrix& generated_path = Matrix();

	/** Returns the next price path */
	virtual void next(Matrix& pricePath) override;

	bool new_path = true;
	AntitheticPathGenerator(SPtrPathGenerator path_generator);
	SPtrPathGenerator path_generator;
};

///////////////////////////////////////////////////////////////////////////////
// Inline definitions
inline
AntitheticPathGenerator::AntitheticPathGenerator(SPtrPathGenerator path_generator)
{
	this->path_generator = path_generator;
}

inline void AntitheticPathGenerator::next(Matrix& pricePath) {
	if (this->new_path) {
		this->path_generator->next(pricePath);
		this->generated_path = pricePath;
		this->new_path = false;
	}
	else {
		pricePath = this->generated_path*-1;
		this->new_path = true;
	}
}

END_NAMESPACE(orf)

#endif // ORF_ANTITHETICPATHGENERATOR_HPP
