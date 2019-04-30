#ifndef ESSENTIA_LANDR_MEAN_FRAMES_H
#define ESSENTIA_LANDR_MEAN_FRAMES_H

#include <memory>
#include <essentia/essentiamath.h>
#include "streamingalgorithm.h"

namespace essentia {
namespace streaming {

    class MeanFrames : public Algorithm
    {
    public:
        
        MeanFrames();
        virtual ~MeanFrames();       
       
        void declareParameters() override;
        void configure() override;
        AlgorithmStatus process() override;
        
        static const char* name;
        static const char* category;
        static const char* description;
    
        static void Register();
        
    protected:
        
        Sink<std::vector<std::vector<Real> > > _input;
        Source<std::vector<Real> > _output;
        
    private:

    };
}

} // namespace streaming
} // namespace essentia

#endif ESSENTIA_LANDR_MEAN_FRAMES_H
