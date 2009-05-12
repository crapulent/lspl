#include "../../base/BaseInternal.h"

#include "Matcher.h"

#include "../../text/Transition.h"
#include "../../text/Node.h"

#include <iostream>

using lspl::text::Transition;
using lspl::text::TransitionList;
using lspl::text::TransitionRef;

using lspl::text::attributes::AttributeKey;
using lspl::text::attributes::AttributeType;
using lspl::text::attributes::AttributeValue;

namespace lspl { namespace patterns { namespace matchers {

uint Matcher::aliveObjectsCount = 0;

Matcher::Matcher( Type type ) :
	type( type ) {
	++ aliveObjectsCount;
}

Matcher::~Matcher() {
	-- aliveObjectsCount;
}

bool Matcher::matchRestrictions( const Transition & annotation, const Context & ctx ) const {
	for ( uint i = 0; i < restrictions.size(); ++ i ) // Проверяем соответствие ограничениям
		if ( !restrictions[ i ].matches( annotation, ctx ) )
			return false;

	return true;
}

void Matcher::dumpRestrictions( std::ostream & out, const std::string & tabs ) const {
	out << "< ";

	for ( uint i = 0; i < restrictions.size(); ++ i ) {
		if ( i != 0 )
			out << ", ";

		restrictions[i].dump( out, tabs + "\t" );
	}

	out << " >";
}

TransitionList AnnotationMatcher::buildTransitions( const text::Node & node, const Context & context ) const {
	TransitionList transitions;

	for ( uint i = 0; i < node.transitions.size(); ++ i )
		if ( matchTransition( *node.transitions[i], context ) )
			transitions.push_back( node.transitions[i] );

	return transitions;
}

} } } // namespace lspl::patterns::matchers