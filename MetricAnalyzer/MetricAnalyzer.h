
#ifndef MetricAnalzyer_H
#define MetricAnalyzer_H

#include <iostream>

namespace Executive {

	class MetricAnalyzer {
	public:	
		MetricAnalyzer() {};
		~MetricAnalyzer() {};

		void Analyze(std::string file);
	};
}

#endif
