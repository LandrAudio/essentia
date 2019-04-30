#ifndef ESSENTIA_LANDR_DERIVATIVE_VECTOR_H
#define ESSENTIA_LANDR_DERIVATIVE_VECTOR_H

#include <memory>
#include <essentia/essentiamath.h>
#include "streamingalgorithm.h"

namespace essentia {
namespace streaming {

    class DerivativeVector : public Algorithm
    {
    public:
        
        DerivativeVector();
        virtual ~DerivativeVector();       
       
        void declareParameters() override;
        void configure() override;
        AlgorithmStatus process() override;
        
        static const char* name;
        static const char* category;
        static const char* description;
    
        static void Register();
        
    protected:
        
        Sink<std::vector<Real> > _input;
        Source<std::vector<Real> > _output;
        
    private:

        std::vector<Real> _prevFrame;

    };

} // namespace streaming
} // namespace essentia

#endif ESSENTIA_LANDR_DERIVATIVE_VECTOR_H
