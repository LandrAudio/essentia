#ifndef ESSENTIA_LANDR_TRANSPOSE_H
#define ESSENTIA_LANDR_TRANSPOSE_H

#include <memory>
#include <essentia/essentiamath.h>
#include "streamingalgorithm.h"

namespace essentia {
namespace streaming {

    class Transpose : public Algorithm
    {
    public:
        
        Transpose();
        virtual ~Transpose();       
       
        void declareParameters() override;
        void configure() override;
        AlgorithmStatus process() override;
        
        static const char* name;
        static const char* category;
        static const char* description;
    
        static void Register();
        
    protected:
        
        Sink<std::vector<essentia::Real> > _input;
        Source<std::vector<essentia::Real> > _output;
        
    private:

    };

} // namespace streaming
} // namespace essentia

#endif ESSENTIA_LANDR_TRANSPOSE_H
