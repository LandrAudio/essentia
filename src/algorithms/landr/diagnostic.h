#ifndef ESSENTIA_LANDR_DIAGNOSTIC_H
#define ESSENTIA_LANDR_DIAGNOSTIC_H

#include <memory>
#include "streamingalgorithm.h"

namespace essentia {
namespace streaming {

    class Diagnostic : public Algorithm
    {
    public:
        
        Diagnostic();
        virtual ~Diagnostic();
        
        void declareParameters() override;
        void configure() override;
        AlgorithmStatus process() override;
        
        static const char* name;
        static const char* category;
        static const char* description;
        
        static void Register();
        
    protected:
        
        Sink<std::vector<essentia::Real> > _frameInput;

    private:
        int _startSample;
        int _frameSizeInSamples;
    };

} // namespace streaming
} // namespace essentia

#endif ESSENTIA_LANDR_DIAGNOSTIC_H
