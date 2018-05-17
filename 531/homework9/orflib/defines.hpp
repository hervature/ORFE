/** 
  @file  defines.hpp
  @brief Library-wide version numbers, macro and constant definitions
*/

#ifndef ORF_DEFINES_HPP
#define ORF_DEFINES_HPP

/** version string */
#ifdef _DEBUG
	#define ORF_VERSION "0.9.0-debug"
#else
    #define ORF_VERSION "0.9.0"
#endif

/** version numbers */
#define ORF_MAJOR_VERSION 0
#define ORF_MINOR_VERSION 9
#define ORF_REVISION_VERSION 0

/** Macro for namespaces */
#define BEGIN_NAMESPACE(x)	namespace x {
#define END_NAMESPACE(x)	}

/** Macro for Extern C */
#define BEGIN_EXTERN_C  extern "C" {
#define END_EXTERN_C    }

/** number of days in a year */
#define DAYS_PER_YEAR 365.25

/** number of seconds in a day */
#define SECS_PER_DAY 86400. //(24.*60*60)
#define SECS_PER_DAY_LONG 86400L

/** number of seconds in a year */
#define SECS_PER_YEAR (SECS_PER_DAY*DAYS_PER_YEAR)

/** number of seconds in an hour */
#define SECS_PER_HOUR 3600.

/** sqrt(pi) */
#define M_SQRTPI  1.77245385090551602792981

/** 1/sqrt(pi) */
#define M_1_SQRTPI  0.564189583547756286948

/** 1/sqrt(2*pi) */
#define M_SQRT_2PI  0.398942280401432678

/** 1/sqrt(2) */
#define M_SQRT_2  0.707106781186547524

/** sqrt(2) */
#define M_SQRT2 1.41421356237309505

#endif // ORF_DEFINES_HPP
