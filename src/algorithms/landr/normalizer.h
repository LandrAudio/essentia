#ifndef ESSENTIA_LANDR_NORMALIZER_H
#define ESSENTIA_LANDR_NORMALIZER_H

#include <memory>
#include <essentia/essentiamath.h>
#include "streamingalgorithm.h"

namespace essentia {
namespace streaming {

    class Normalizer : public Algorithm
    {
    public:
        
        Normalizer();
        virtual ~Normalizer();       
       
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

    };

} // namespace streaming
} // namespace essentia

#endif ESSENTIA_LANDR_NORMALIZER_H
