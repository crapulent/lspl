/*
 * Range.h
 *
 *  Created on: Feb 14, 2009
 *      Author: alno
 */

#ifndef _LSPL_BASE_RANGE_H_
#define _LSPL_BASE_RANGE_H_

#include "Base.h"

namespace lspl { namespace base {

class LSPL_EXPORT Range {
public:
	Range() {}
	Range( uint start, uint end ) : start( start ), end( end ) {}
	~Range() {}

	uint length() const { return end - start; }

public:
	uint start;
	uint end;
};

} } // namespace lspl::base

#endif /* _LSPL_BASE_RANGE_H_ */