#include "landrmfcc.h"

namespace essentia {
namespace streaming {

const int LandrMFCC::NUM_COEFFICIENTS = 13;
const char* LandrMFCC::name = "LandrMFCC";
const char* LandrMFCC::category = "None";
const char* LandrMFCC::description = "";
//"Computes MFCC coefficients. First, the mel spectrum is computed using 40 triangular bands (13 linearly spaced and 27 logarithmically spaced). The mel spectrum is then analyzed using the Discrete Cosine Transform ( https://en.wikipedia.org/wiki/Discrete_cosine_transform#DCT-II ) to extract 13 MFCC coefficients"

LandrMFCC::LandrMFCC()
: Algorithm(), _numCoeffs(NUM_COEFFICIENTS), _magnitudeSizeInSamples(0)
{
    declareInput(_magnitudeSpectrumInput, 1, "magnitude", "Magnitude spectrum.");
    declareOutput(_mfccOutput, 1, "mfcc", "MFCC coefficients ");
    declareParameters();
}

LandrMFCC::~LandrMFCC()
{
}

void
LandrMFCC::Register()
{
    AlgorithmFactory::Registrar<LandrMFCC> LandrMFCC;
}

void
LandrMFCC::declareParameters()
{
    declareParameter("sampleRate", "the sample rate", "(0,inf)", 44100.0);
    declareParameter("magnitudeSpectrumSize", "the number of bins in the input magnitude spectrum", "(0,inf)", 513);
}

void
LandrMFCC::configure()
{
    _sampleRate = parameter("sampleRate").toDouble();
    _magnitudeSizeInSamples = parameter("magnitudeSpectrumSize").toInt();
    
    GenerateMFCCFilters(_magnitudeSizeInSamples, _sampleRate);
    GenerateDCTMatrix(static_cast<int>(_H.size()), _numCoeffs);
}

AlgorithmStatus
LandrMFCC::process()
{
    auto status = acquireData();
    if (status != OK)
    {
        if (!shouldStop()) return status;

        return NO_INPUT;
    }

    // Grab frames from stream, as requested in constructor
    const std::vector<std::vector<essentia::Real>>& spec = _magnitudeSpectrumInput.tokens();
    std::vector<std::vector<essentia::Real>>& mfcc = _mfccOutput.tokens();
    
    // Loop frames in stream
    for (int i = 0; i < spec.size() && i < mfcc.size(); ++i)
    {
        std::vector<essentia::Real> XMel;
        XMel.resize(_H.size());
        
        mfcc[i].resize(_numCoeffs);
        
        //compute the mel spectrum
        
        for (int n = 0; n < _H.size(); ++n) {
            for (int k = 0; k < _magnitudeSizeInSamples; ++k) {
                XMel[n] += _H[n][k] * spec[i][k];
            }
            XMel[n] = log10(XMel[n] + 1.0e-20);
        }
        
        // allocate memory
        essentia::Real mfcctemp;
        
        // calculate the mfccs
        for (int n = 0; n < _numCoeffs; ++n)
        {
            mfcctemp = 0.0;
            for (int k = 0; k < XMel.size(); ++k) {
                mfcctemp += _T[n][k] * XMel[k];
            }
            mfcc[i][n] = mfcctemp;
        }
    }
    releaseData();
    return OK;
}

void
LandrMFCC::GenerateMFCCFilters(int fftSize, double sampleRate)
{
    const double fStart = 133.3333;
    const int numLinFilters = 13;
    const int numLogFilters = 27;
    
    size_t numFilters = numLinFilters + numLogFilters;
    
    const double linSpacing = 66.66666666; //exact same numbers as in Matlab because training is done with Matlab
    const double logSpacing = 1.0711703;
    std::vector<double> f;
    // compute band frequencies
    for (int k = 0; k < numLinFilters; ++k) {
        f.push_back(fStart + k*linSpacing);
    }
    for (int k = 1; k <= numLogFilters + 2; ++k) {
        f.push_back(f[numLinFilters - 1] * pow(logSpacing, k));
    }
    // sanity check
    int i = 0;
    while (i < f.size() && f[i] < sampleRate / 2) {
        ++i;
    }
    if (i < f.size() - 1) {
        f.erase(f.begin() + i, f.end());
    }
    
    numFilters = f.size() - 2;
    std::vector<double> fL, fC, fU;
    
    std::copy(f.begin(), f.begin() + numFilters, std::back_inserter(fL));
    std::copy(f.begin() + 1, f.begin() + numFilters + 1, std::back_inserter(fC));
    std::copy(f.begin() + 2, f.begin() + numFilters + 2, std::back_inserter(fU));
    
    //set max amplitude
    std::vector<double> afFilterMax;
    for (int k = 0; k < fU.size(); ++k) {
        afFilterMax.push_back(2.0 / (fU[k] - fL[k]));
    }
    
    std::vector<double> fK;
    for (int k = 0; k < fftSize; ++k){
        fK.push_back(((double)k) / ((double)fftSize)*sampleRate / 2.0);
    }
    
    _H.resize(numFilters);
    for (int i = 0; i < _H.size(); ++i) {
        _H[i].resize(fftSize);
    }
    // compute the transfer functions
    for (int c = 0; c<numFilters; ++c) {
        for (int k = 0; k<fftSize; ++k) {
            _H[c][k] =
            ((double)(fK[k] > fL[c] && fK[k] <= fC[c])) *
            afFilterMax[c] * ((double)(fK[k] - fL[c]) / (fC[c] - fL[c])) +
            ((double)(fK[k] > fC[c] && fK[k] < fU[c])) *
            afFilterMax[c] * (fU[c] - fK[k]) / (fU[c] - fC[c]);
        }
    }
}

void
LandrMFCC::GenerateDCTMatrix(int numBands, int numCoeffs)
{
    std::vector<double> c, n;
    
    const double PI(4.0*atan(1.0));
    
    for (int k = 0; k < numCoeffs; ++k) {
        c.push_back((double)k);
    }
    for (int k = 0; k < numBands; ++k) {
        n.push_back((2 * k + 1)*PI / 2.0 / (double)numBands);
    }
    
    _T.resize(numCoeffs);
    for (int i = 0; i < numCoeffs; ++i) {
        _T[i].resize(numBands);
    }
    
    for (int l = 0; l < numCoeffs; ++l) {
        for (int k = 0; k < numBands; ++k) {
            _T[l][k] = cos(c[l] * n[k]) / sqrt((double)numBands / 2.0);
        }
    }
    for (int c = 0; c < numBands; ++c) {
        _T[0][c] *= sqrt(2.0) / 2.0;
    }
}

} // namespace streaming
} // namespace essentia
