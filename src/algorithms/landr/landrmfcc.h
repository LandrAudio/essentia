#ifndef ESSENTIA_LANDR_MFCC_H
#define ESSENTIA_LANDR_MFCC_H

#include "streamingalgorithm.h"

namespace essentia {
namespace streaming {

    class LandrMFCC : public Algorithm {            
        protected:
            Sink<std::vector<Real> > _magnitudeSpectrumInput;
            Source<std::vector<Real> > _mfccOutput;

        public:
            LandrMFCC() : Algorithm(), _numCoeffs(NUM_COEFFICIENTS), _magnitudeSizeInSamples(0)
            {
                declareInput(_magnitudeSpectrumInput, 1, "magnitude", "Magnitude spectrum.");
                declareOutput(_mfccOutput, 1, "mfcc", "MFCC coefficients ");
                declareParameters();
            }

            ~LandrMFCC() {}   
        
            AlgorithmStatus process() override;

            void declareParameters() {
                declareParameter("sampleRate", "the sample rate", "(0,inf)", 44100.0);
                declareParameter("magnitudeSpectrumSize", "the number of bins in the input magnitude spectrum", "(0,inf)", 513);
            }
            void configure() {
                _sampleRate = parameter("sampleRate").toDouble();
                _magnitudeSizeInSamples = parameter("magnitudeSpectrumSize").toInt();
                
                GenerateMFCCFilters(_magnitudeSizeInSamples, _sampleRate);
                GenerateDCTMatrix(static_cast<int>(_H.size()), _numCoeffs);
            }
            
            static const char* name;
            static const char* category;
            static const char* description;
            
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
