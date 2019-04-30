#include "linearregression.h"

namespace essentia {
namespace streaming {

const char* LANDR::LinearRegression::name = "LinearRegression";
const char* LANDR::LinearRegression::category = "Statistics";
const char* LANDR::LinearRegression::description = "Returns the gradient and intersection of a linear regression fit to the input vector";

LANDR::LinearRegression::LinearRegression()
: Algorithm()
{
    declareInput(_vectorInput, 1, "input", "Input vector");
    // declareInput(_vectorInput, 1, "weighting", "Input vector weighting");
    declareOutput(_gradientOutput, 1, "gradient", "Gradient of linear regression");
    declareOutput(_intersectionOutput, 1, "intersection", "Intersection of linear regression");
    declareParameters();
}

LANDR::LinearRegression::~LinearRegression()
{
}

void
LANDR::LinearRegression::Register()
{
    AlgorithmFactory::Registrar<LinearRegression> LinearRegression;
}

void
LANDR::LinearRegression::declareParameters()
{
}

void
LANDR::LinearRegression::configure()
{
}

// A simplified interface to standard linear (least squares) regression function, assuming X values are the indices of y and
// hence implicitly monotonic and integer increments. Also assumed is a constant standard deviation on each y value.
// A line y = a + bx is fit by minimizing chi-squared. Returns a,b, probabilities siga, sigb, chi-square chi2.
// Chi-squared is normalized to unit standard deviation on all points.
// Derived from fit() from Numerical Recipes in C++ 2nd Edition, p670.
void
LANDR::LinearRegression::linearRegression(const std::vector<essentia::Real> &y,
                                          essentia::Real &a,
                                          essentia::Real &b,
                                          essentia::Real &sigma_a,
                                          essentia::Real &sigma_b,
                                          essentia::Real &chi2)
{
    unsigned int vectorIndex;
    essentia::Real t, sxoss, sx = 0.0, sy = 0.0, st2 = 0.0, sigma_data;
    unsigned vectorSize = y.size();
    
    b = 0.0;
    // TODO convert to vector processing versions.
    // Accumulate sums without weights.
    for (vectorIndex = 0; vectorIndex < vectorSize; vectorIndex++) {
        sx += vectorIndex; // assumes x values are 0..vectorSize - 1.
        sy += y[vectorIndex];
    }
    sxoss = sx / vectorSize;
    for (vectorIndex = 0; vectorIndex < vectorSize; vectorIndex++) {
        t = vectorIndex - sxoss;
        st2 += t * t;
        b += t * y[vectorIndex];
    }
    // Solve for a, b, sigma_a, sigma_b
    b /= st2;
    a = (sy - sx * b) / vectorSize;
    sigma_a = sqrt((1.0 + sx * sx / (vectorSize * st2)) / vectorSize);
    sigma_b = sqrt(1.0 / st2);
    // Calculate chi-squared from the solved a & b values against the original data.
    chi2 = 0.0;
    for (vectorIndex = 0; vectorIndex < vectorSize; vectorIndex++) {
        essentia::Real residual = y[vectorIndex] - a - b * vectorIndex;
        chi2 += residual * residual;
    }
    // For unweighted data, evaluate typical sigma using chi2, and adjust the standard deviations.
    sigma_data = sqrt(chi2 / (vectorSize - 2));
    sigma_a *= sigma_data;
    sigma_b *= sigma_data;
}

AlgorithmStatus
LANDR::LinearRegression::process()
{
    auto status = acquireData();
    if (status != OK)
    {
        if (!shouldStop()) return status;
        
        return NO_INPUT;
    }
    
    // Grab frames from stream, as requested in constructor
    const std::vector<std::vector<essentia::Real>>& vecIn = _vectorInput.tokens();
    std::vector<essentia::Real>& gradOut = _gradientOutput.tokens();
    std::vector<essentia::Real>& interOut = _intersectionOutput.tokens();

    // Loop frames in stream
    for (int i = 0; i < vecIn.size() && i < gradOut.size(); ++i)
    {
        essentia::Real gradient = 0.0; // default is that of zero gradient, i.e. no changing values.
        essentia::Real intersection;
        essentia::Real sigma_intersection;
        essentia::Real sigma_gradient;
        essentia::Real chi2;

        if (vecIn[i].size() >= 2) { // ensure the number of data points exceed the number of variables in a line equation.
            // To determine the gradient, we do a linear least squares regression fitting a line to the data.
            // Since our x axis values are equally spaced we can simplify a bit and not have to pass in the x values.
            linearRegression(vecIn[i], intersection, gradient, sigma_intersection, sigma_gradient, chi2);
            COUT << "gradient y = " << gradient << " intersection x + " <<  intersection << std::endl;
            COUT << "gradient stddev = " << sigma_gradient << " intersection stddev = " << sigma_intersection << std::endl;
            COUT << "chi2 = " << chi2 << std::endl;
            gradOut[i] = gradient;
            interOut[i] = intersection;
        }
    }
    releaseData();
    return OK;
}

} // namespace streaming
} // namespace essentia
