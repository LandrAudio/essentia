#include "count.h"

namespace essentia {
namespace streaming {

const char* Count::name = "Count";
const char* Count::category = "None";
const char* Count::description = "";

Count::Count()
    : AlgorithmComposite()
{
    declareInput(_input, "input", "");
    declareOutput(_output, 0, "output", "");

    _vectorOutput = std::make_shared<VectorOutput<std::vector<Real>>>(&_accu);
    _input >> _vectorOutput->input("data");
}

Count::~Count()
{}

void
Count::Register()
{
    AlgorithmFactory::Registrar<Count> count;
}

void
Count::configure()
{}

void
Count::declareParameters()
{}

AlgorithmStatus
Count::process()
{
    if (!shouldStop()) return PASS;

    float count = 0;

    for (auto data : _accu)
    {
        count += data.size();
    }

    _output.push(count);

    return FINISHED;

}

} // namespace streaming
} // namespace essentia
