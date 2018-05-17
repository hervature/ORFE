/**
@file  mcparams.hpp
@brief Monte Carlo parameters
*/

#ifndef ORF_MCPARAMS_HPP
#define ORF_MCPARAMS_HPP

#include <orflib/defines.hpp>
#include <orflib/exception.hpp>

BEGIN_NAMESPACE(orf)


/** Monte Carlo parameters
*/
struct McParams
{
  /** The known URNG types */
  enum UrngType
  {
    MINSTDRAND,
    MT19937,
    RANLUX3,
    RANLUX4,
    SOBOL
  };

  /** The known path generator types */
  enum PathGenType
  {
    EULER,
    BROWNIANBRIDGE
  };

  /** The known path generator types */
  enum ControlVarType
  {
    NONE,
    ANTITHETIC
  };


  /** Default ctor */
  McParams(UrngType u = MT19937, PathGenType p = EULER, ControlVarType c = NONE);

  // state
  UrngType urngType;
  PathGenType pathGenType;
  ControlVarType controlVarType;
};

///////////////////////////////////////////////////////////////////////////////
// Inline definitions

inline
McParams::McParams(UrngType u, PathGenType p, ControlVarType c)
: urngType(u), pathGenType(p), controlVarType(c)
{}

END_NAMESPACE(orf)

#endif // ORF_MCPARAMS_HPP
