#include <iostream>
#include <string>

#include "lspl/patterns/expressions/VariableExpression.h"
#include "lspl/patterns/PatternBuilder.h"
#include "DictionaryRecognizer.h"
#include "SimilarityRecognizer.h"
#include "Util.h"

void ExtractTerms(const std::vector<lspl::PatternMatchRef> &matches,
		lspl::TTerms &terms) {
	terms.clear();
	for(int i = 0; i < matches.size(); ++i) {
		lspl::TTerm term(matches[i]->matches[0]->getRangeString(),
				matches[i]->match_count);
		terms.push_back(term);
		std::cout << lspl::Util::out.convert(matches[i]->matches[0]->getRangeString())
				<< " " << matches[i]->match_count << std::endl;
	}
	std::cout << "----" << std::endl;
}

int main(int argc, char** argv) {
	if (argc < 4) {
		std::cout << "Usage: " << argv[0] <<
				" patterns1 patterns2 text [similar patterns] " << std::endl;
		std::cout << "\tpatterns1 - dictionary terms" << std::endl;
		std::cout << "\tpatterns2 - all terms candidates patterns" << std::endl;
		std::cout << "\tsimilar_patterns - similar patterns templates" <<
				std::endl;
		return 1;
	}
	std::cout << "Patterns from file " << argv[1] << std::endl << std::endl;
	lspl::DictionaryRecognizer *recognizer =
			new lspl::DictionaryRecognizer(argv[1], argv[3]);
	std::vector<lspl::PatternMatchRef> matches1 =
			recognizer->RecognizeAndSearch();
	std::cout << std::endl << "Patterns from file " << argv[2] << std::endl <<
			std::endl;
	recognizer = new lspl::DictionaryRecognizer(argv[2], argv[3]);
	std::vector<lspl::PatternMatchRef> matches2 =
			recognizer->RecognizeAndSearch();
	lspl::TTerms terms1;
	//terms1.push_back(lspl::Util::in.convert("правильность программ"));
	ExtractTerms(matches1, terms1);
	lspl::TTerms terms2;
	//terms2.push_back(lspl::Util::in.convert("правильность"));
	ExtractTerms(matches2, terms2);
	delete recognizer;
	const char *similar_patterns_file = "similarity_patterns.txt";
	if (argc == 5) {
		similar_patterns_file = argv[4];
	}
	std::cout << std::endl << "Similarity patterns from file " <<
			similar_patterns_file << std::endl << std::endl;
	lspl::SimilarityRecognizer *similarity_recognizer =
			new lspl::SimilarityRecognizer(similar_patterns_file);
	similarity_recognizer->FindSimilars(terms1, terms2);
	/*recognizer = new lspl::DictionaryRecognizer(similar_patterns_file, argv[2]);
	std::vector<lspl::PatternsMatch> similar_terms =
			recognizer->RecognizeAndSearch();
	delete recognizer;*/
	return 0;
}
