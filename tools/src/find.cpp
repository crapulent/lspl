/*
 * find.cpp
 *
 *  Created on: Dec 22, 2008
 *      Author: alno
 */

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <lspl/Namespace.h>
#include <lspl/patterns/Pattern.h>
#include <lspl/patterns/PatternBuilder.h>

#include <lspl/text/Text.h>
#include <lspl/text/readers/PlainTextReader.h>

#include <lspl/transforms/TextTransformBuilder.h>
#include <lspl/transforms/PatternTransformBuilder.h>

using lspl::uint;

void printHelp() {
	std::cout << "Usage: lspl-gen [options] patterns" << std::endl
		<< "Options:" << std::endl
		<< "  -h - show this help message" << std::endl
		<< "  -p <file> - specify patterns file (in CP1251 encoding)" << std::endl
		<< "  -i <file> - specify input file (in CP1251 encoding)" << std::endl
		<< "  -o <file> - specify output file for all patterns (in CP1251 encoding)" << std::endl
		<< "  -t <file> - specify output file for patterns with text transformation (in CP1251 encoding)" << std::endl
		<< "  -r <file> - specify output file for patterns with pattern transformation (in CP1251 encoding)" << std::endl
		<< "  -e <file> - specify error output file (in CP1251 encoding)" << std::endl;
}

void buildPatterns( const lspl::patterns::PatternBuilderRef builders[], std::istream & in, std::ostream & err ) {
	char buffer[1000];
	while (!in.eof()) {
		in.getline( buffer, sizeof(buffer), '\n' );

		buffer[in.gcount()] = 0;

		try {
			std::string pattern(buffer);
			lspl::patterns::PatternBuilder::BuildInfo info;
			if (pattern.find("=pattern>") != std::string::npos) {
				info = builders[2]->build( pattern );
			} else if (pattern.find("=text>") != std::string::npos) {
				info = builders[1]->build( pattern );
			} else {
				info = builders[0]->build( pattern );
			}

			if ( info.parseTail.length() > 0 ) {
				err << "Error during parsing '" << buffer << "': '" << info.parseTail << "' not parsed" << std::endl;
			}
		} catch ( std::exception & ex ) {
			err << "Error during parsing '" << buffer << "': " << ex.what() << std::endl;
		} catch ( ... ) {
			err << "Unknown error during parsing '" << buffer << "'" << std::endl;
		}
	}
}

lspl::patterns::PatternList buildGoals( const lspl::patterns::PatternBuilderRef builders[], char ** argv, int start, int end, std::ostream & err ) {
	lspl::patterns::PatternList goals;

	for ( int i = start; i < end; ++i ) {
		std::string source( argv[i] );

		if ( source == "*" ) { // Цель * - найти ВСЕ шаблоны
			for ( uint i = 0, sz = builders[0]->space->getPatternCount(); i < sz; ++i )
				goals.push_back( builders[0]->space->getPatternByIndex( i ) );

			continue;
		}

		try {
			lspl::patterns::PatternRef goal = builders[0]->space->getPatternByName( source );

			if ( !goal ) {
				lspl::patterns::PatternBuilder::BuildInfo info;
				if (source.find("=pattern>") != std::string::npos) {
					info = builders[2]->build( source );
				} else if (source.find("=text>") != std::string::npos) {
					info = builders[1]->build( source );
				} else {
					info = builders[0]->build( source );
				}

				if ( info.parseTail.length() == 0 ) {
					goal = builders[0]->space->getPatternByName( source );
				} else {
					err << "Error during parsing '" << source << "': '" << info.parseTail << "' not parsed" << std::endl;
				}
			}

			if ( goal )
				goals.push_back( goal );
		} catch ( std::exception & ex ) {
			err << "Error during parsing '" << source << "': " << ex.what() << std::endl;
		} catch ( ... ) {
			err << "Unknown error during parsing '" << source << "'" << std::endl;
		}
	}

	return goals;
}

void processGoal( const lspl::patterns::PatternRef & goal, const lspl::text::TextRef & text, std::ostream *outs[], std::ostream & err ) {
	int patternType;
	if (goal->getSource().find("=pattern>") != std::string::npos) {
		patternType = 2;
	} else if (goal->getSource().find("=text>") != std::string::npos) {
		patternType = 1;
	} else {
		patternType = 0;
	}
	std::ostream& out = *outs[patternType];
	

	lspl::text::MatchList matches = text->getMatches( goal );

	out << "\t\t<goal name=\"" << goal->name << "\">\n";

	for ( uint matchIndex = 0; matchIndex < matches.size(); ++ matchIndex ) {
		lspl::text::MatchRef match = matches[ matchIndex ];

		out << "\t\t\t<match startPos=\"" << match->getRangeStart() << "\" endPos=\"" << match->getRangeEnd() << "\">\n";
		out << "\t\t\t\t<fragment>" << match->getRangeString() << "</fragment>\n";

		if (patternType == 1) {
			for ( uint variantIndex = 0; variantIndex < match->getVariantCount(); ++ variantIndex )
				out << "\t\t\t\t<result>" << match->getVariant(variantIndex)->getTransformResult<std::string>() << "</result>\n";
		} else if (patternType == 2) {
			for ( uint variantIndex = 0; variantIndex < match->getVariantCount(); ++ variantIndex ) {
				lspl::patterns::PatternRef pt = match->getVariant(variantIndex)->getTransformResult<lspl::patterns::PatternRef>();
				out << "\t\t\t\t<result><![CDATA[" << pt->getName() << " = " << pt->getSource() << "]]></result>\n";
			}
		}

		out << "\t\t\t</match>\n";
	}

	out << "\t\t</goal>\n";
	out.flush();
}

int main(int argc, char ** argv) {
	opterr = 0;

	std::istream * in = &std::cin;
	std::istream * pin = 0;
	std::ostream * out = &std::cout;
	std::ostream * outt = &std::cout;
	std::ostream * outp = &std::cout;
	std::ostream * err = &std::cerr;

	if ( argc <= 1 ) {
		printHelp();
		return 0;
	}

	char c;
	while ((c = getopt(argc, argv, "hi:o:e:p:t:r:")) != -1) {
		switch (c) {
		case 'i':
			in = new std::ifstream( optarg );
			if ( in->fail() ) {
				std::cerr << "Error opening '" << optarg << "' as input file" << std::endl;
				return 1;
			}
			break;
		case 'o':
			out = new std::ofstream( optarg );
			if ( out->fail() ) {
				std::cerr << "Error opening '" << optarg << "' as output file" << std::endl;
				return 1;
			}
			break;
		case 't':
			outt = new std::ofstream( optarg );
			if ( outt->fail() ) {
				std::cerr << "Error opening '" << optarg << "' as output file" << std::endl;
				return 1;
			}
			break;
		case 'r':
			outp = new std::ofstream( optarg );
			if ( outp->fail() ) {
				std::cerr << "Error opening '" << optarg << "' as output file" << std::endl;
				return 1;
			}
			break;
		case 'e':
			err = new std::ofstream( optarg );
			if ( err->fail() ) {
				std::cerr << "Error opening '" << optarg << "' as error file" << std::endl;
				return 1;
			}
			break;
		case 'p': {
			pin = new std::ifstream( optarg );
			if ( pin->fail() ) {
				std::cerr << "Error opening '" << optarg << "' as pattern file" << std::endl;
				return 1;
			}

			break;
		}
		case 'h':
		default:
			printHelp();
			return 0;
		}

		if ( opterr ) {
			printHelp();
			return 0;
		}
	}

	if (outt != out && outt == &std::cout) outt = out;
	if (outp != out && outp == &std::cout) outp = out;

	lspl::NamespaceRef ns = new lspl::Namespace();
	lspl::patterns::PatternBuilderRef builder = new lspl::patterns::PatternBuilder( ns );
	lspl::patterns::PatternBuilderRef builderText = new lspl::patterns::PatternBuilder( ns, new lspl::transforms::TextTransformBuilder( ns ) );
	lspl::patterns::PatternBuilderRef builderPattern = new lspl::patterns::PatternBuilder( ns, new lspl::transforms::PatternTransformBuilder( ns ) );
	const lspl::patterns::PatternBuilderRef builders[] = {builder, builderText, builderPattern};
	lspl::text::readers::PlainTextReader r;

	if ( pin ) {
		buildPatterns( builders, *pin, *err );
		delete pin;
		pin = 0;
	}

	lspl::patterns::PatternList goals = buildGoals( builders, argv, optind, argc, *err );
	lspl::text::TextRef text = r.readFromStream( *in );

	*out << "<?xml version=\"1.0\" encoding=\"windows-1251\"?>\n<texts>\n\t<text>\n";
	if (outt != out)
		*outt << "<?xml version=\"1.0\" encoding=\"windows-1251\"?>\n<texts>\n\t<text>\n";
	if (outp != out)
		*outp << "<?xml version=\"1.0\" encoding=\"windows-1251\"?>\n<texts>\n\t<text>\n";

	std::ostream *outs[] = {out, outt, outp};

	for ( uint goalIndex = 0; goalIndex < goals.size(); ++ goalIndex )
		processGoal( goals[ goalIndex ], text, outs, *err );

	*out << "\t</text>\n</texts>\n";
	if (outt != out)
		*outt << "\t</text>\n</texts>\n";
	if (outp != out)
		*outp << "\t</text>\n</texts>\n";

	if ( in != &std::cin )
		delete in;
	if ( outp != &std::cout && outp != out )
		delete outp;
	if ( outt != &std::cout && outt != out )
		delete outt;
	if ( out != &std::cout )
		delete out;
	if ( err != &std::cerr )
		delete err;

	return 0;
}
