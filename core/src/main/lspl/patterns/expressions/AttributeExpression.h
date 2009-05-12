/*
 * AttributeExpression.h
 *
 *  Created on: Sep 28, 2008
 *      Author: alno
 */

#ifndef _LSPL_PATTERNS_EXPRESSIONS_ATTRIBUTEEXPRESSION_H_
#define _LSPL_PATTERNS_EXPRESSIONS_ATTRIBUTEEXPRESSION_H_

#include "../../base/Base.h"
#include "Expression.h"
#include "../../text/attributes/AttributeKey.h"

#include <boost/scoped_ptr.hpp>

namespace lspl { namespace patterns { namespace expressions {

class LSPL_EXPORT AttributeExpression : public Expression {
public:
	AttributeExpression( const Expression * base, const text::attributes::AttributeKey & attribute );
	virtual ~AttributeExpression();

	text::attributes::AttributeValue evaluate( const text::Transition & annotation, const matchers::Context & ctx ) const;

	void dump( std::ostream & out, const std::string & tabs = "" ) const;
public:
	boost::scoped_ptr<const Expression> base;
	text::attributes::AttributeKey attribute;
};

} } }

#endif//_LSPL_PATTERNS_EXPRESSIONS_ATTRIBUTEEXPRESSION_H_