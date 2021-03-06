#include "../../base/BaseInternal.h"

#include "RegexpMatcher.h"

#include "../../text/Node.h"
#include "../../text/markup/Token.h"

#include "../../morphology/Morphology.h"

using lspl::text::Transition;
using lspl::text::TransitionRef;
using lspl::text::TransitionList;
using lspl::text::markup::Token;

using lspl::morphology::Morphology;

namespace lspl { namespace patterns { namespace matchers {

RegexpMatcher::RegexpMatcher( const std::string & token ) : AnnotationMatcher( REGEXP ), exp( Morphology::instance().upcase( token ) ) {}

TransitionList RegexpMatcher::buildTransitions( const text::Node & node, const Context & context ) const {
	TransitionList transitions;

	for ( uint i = 0, sz = node.getTokenCount(); i < sz; ++ i )
		if ( matchTransition( *node.getTransition( i ), context ) )
			transitions.push_back( node.getTransition( i ) );

	return transitions;
}

bool RegexpMatcher::matchTransition( const Transition & transition, const Context & context ) const {
	switch ( transition.type ) {
	case Transition::TOKEN: // Слово
		return exp.FullMatch( static_cast<const Token&>( transition ).getToken() );
	default:
		return false;
	}
}

void RegexpMatcher::dump( std::ostream & out, const std::string & tabs ) const {
	out << "RegexpMatcher{ exp = \"" << exp.pattern() << "\" }";
}

} } } //namespace lspl::patterns::matchers
