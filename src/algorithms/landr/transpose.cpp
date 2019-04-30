#include "transpose.h"

namespace essentia {
namespace streaming {

const char* Transpose::name = "Transpose";
const char* Transpose::category = "None";
const char* Transpose::description = "";

Transpose::Transpose()
    : Algorithm()
{
    declareInput(_input, 1, "input", "");
    declareOutput(_output, "output", "");
    declareParameters();
}

Transpose::~Transpose()
{}

void
Transpose::Register()
{
    AlgorithmFactory::Registrar<Transpose> Transpose;
}

void
Transpose::configure()
{}

void
Transpose::declareParameters()
{}

AlgorithmStatus
Transpose::process()
{
    auto status = acquireData();
    if (status != OK)
    {
        if (!shouldStop()) return status;

        return NO_INPUT;
    }

    // Grab frames from stream, as requested in constructor
    const std::vector<std::vector<Real > >& inputFrames = _input.tokens();

    // Loop frames in stream
    for (int i = 0; i < inputFrames.size(); ++i)
    {
        // Allocate memory at the output
        std::vector<Real> outputFrame(inputFrames[i].size());

        // Find max index
        int idxMax = argmax(inputFrames[i]);
        
        // Duplicate vector before transpose
        std::vector<Real> hpcp_bak = inputFrames[i];
        
        // Transpose [AB] -> [BA]
        for (int j=idxMax; j<(int)hpcp_bak.size(); j++)
        {
            outputFrame[j-idxMax] = hpcp_bak[j];
        }
        std::vector<Real>::size_type offset = hpcp_bak.size() - idxMax;
        for (int j=0; j<idxMax; j++)
        {
            outputFrame[j+offset] = hpcp_bak[j];
        }

        _output.push(outputFrame);
    }

    releaseData();
    return OK;
}

} // namespace streaming
} // namespace essentia
