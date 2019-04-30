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
        
        Sink<std::vector<essentia::Real> > _vectorInput;
        Source<essentia::Real> _gradientOutput;
        Source<essentia::Real> _intersectionOutput;

    private:
        void linearRegression(const std::vector<essentia::Real> &y,
                              essentia::Real &a,
                              essentia::Real &b,
                              essentia::Real &sigma_a,
                              essentia::Real &sigma_b,
                              essentia::Real &chi2);
    };

} // namespace streaming
} // namespace essentia

#endif ESSENTIA_LANDR_LINEAR_REGRESSION_H
