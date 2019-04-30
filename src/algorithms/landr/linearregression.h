#ifndef ESSENTIA_LANDR_LINEAR_REGRESSION_H
#define ESSENTIA_LANDR_LINEAR_REGRESSION_H

#include <memory>
#include <math.h>       /* sqrt */
#include "streamingalgorithm.h"

namespace essentia {
namespace streaming {

    class LinearRegression : public Algorithm
    {
    public:
        
        LinearRegression();
        virtual ~LinearRegression();
        
        void declareParameters() override;
        void configure() override;
        AlgorithmStatus process() override;
        
        static const char* name;
        static const char* category;
        static const char* description;
        
        static void Register();
        
    protected:
        
        Sink<std::vector<Real> > _vectorInput;
        Source<Real> _gradientOutput;
        Source<Real> _intersectionOutput;

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
