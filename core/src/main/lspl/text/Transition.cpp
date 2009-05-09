#include "../base/BaseInternal.h"

#include "Transition.h"
#include "Node.h"
#include "Text.h"

#include <iostream>

using lspl::text::attributes::AttributeValue;

LSPL_REFCOUNT_CLASS( lspl::text::Transition );

namespace lspl { namespace text {

Transition::~Transition() {
}

uint Transition::getStartOffset() const {
	return start.endOffset;
}

uint Transition::getEndOffset() const {
	return end.startOffset;
}

std::string Transition::getText() const {
	return start.text.getContent().substr( start.endOffset, end.startOffset - start.endOffset );
}

} } // namespace lspl
