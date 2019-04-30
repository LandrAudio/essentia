#ifndef ESSENTIA_LANDR_LINEAR_REGRESSION_H
#define ESSENTIA_LANDR_LINEAR_REGRESSION_H

#include <math.h>       /* sqrt */
#include "streamingalgorithm.h"

namespace essentia {
namespace streaming {

    class LinearRegression : public Algorithm {
        protected:
            Sink<std::vector<Real> > _vectorInput;
            Source<Real> _gradientOutput;
            Source<Real> _intersectionOutput;

        public:
            LinearRegression() : Algorithm() {
                declareInput(_vectorInput, 1, "input", "Input vector");
                // declareInput(_vectorInput, 1, "weighting", "Input vector weighting");
                declareOutput(_gradientOutput, 1, "gradient", "Gradient of linear regression");
                declareOutput(_intersectionOutput, 1, "intersection", "Intersection of linear regression");
                declareParameters();
            }
            
            ~LinearRegression() {}

            AlgorithmStatus process();
            
            void declareParameters() {}
            void configure() {}
            
            static const char* name;
            static const char* category;
            static const char* description;

        private:
            void linearRegression(const std::vector<Real> &y,
                                Real &a,
                                Real &b,
                                Real &sigma_a,
                                Real &sigma_b,
                                Real &chi2);
    };

} // namespace streaming
} // namespace essentia

#endif ESSENTIA_LANDR_LINEAR_REGRESSION_H
