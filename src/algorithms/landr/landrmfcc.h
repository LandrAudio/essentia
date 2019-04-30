#ifndef ESSENTIA_LANDR_MFCC_H
#define ESSENTIA_LANDR_MFCC_H

#include <memory>
#include "streamingalgorithm.h"

namespace essentia {
namespace streaming {

    class LandrMFCC : public Algorithm
    {
    public:
        
        LandrMFCC();
        virtual ~LandrMFCC();       
       
        void declareParameters() override;
        void configure() override;
        AlgorithmStatus process() override;
        
        static const char* name;
        static const char* category;
        static const char* description;
    
        static void Register();
        
    protected:
        
        
        Sink<std::vector<Real> > _magnitudeSpectrumInput;
        Source<std::vector<Real> > _mfccOutput;
        
    private:
        void GenerateMFCCFilters(int magnitudeSpectrumSize, double sampleRate);
        void GenerateDCTMatrix(int nBands, int nCoeffs);

        /// Constants and Statics
        static const int NUM_COEFFICIENTS;
        int _numCoeffs;
        int _magnitudeSizeInSamples;
        double _sampleRate;
        
        std::vector<std::vector<double>> _H;
        std::vector<std::vector<double>> _T;
    };

} // namespace streaming
} // namespace essentia

#endif ESSENTIA_LANDR_MFCC_H
