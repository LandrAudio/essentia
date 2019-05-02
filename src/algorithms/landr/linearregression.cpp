#include "linearregression.h"

namespace essentia {
namespace streaming {

    const char* LinearRegression::name = "LinearRegression";
    const char* LinearRegression::category = "Statistics";
    const char* LinearRegression::description = "Returns the gradient and intersection of a linear regression fit to the input vector";

    // A simplified interface to standard linear (least squares) regression function, assuming X values are the indices of y and
    // hence implicitly monotonic and integer increments. Also assumed is a constant standard deviation on each y value.
    // A line y = a + bx is fit by minimizing chi-squared. Returns a,b, probabilities siga, sigb, chi-square chi2.
    // Chi-squared is normalized to unit standard deviation on all points.
    // Derived from fit() from Numerical Recipes in C++ 2nd Edition, p670.
    void LinearRegression::linearRegression(const std::vector<Real> &y,
                                            Real &a,
                                            Real &b,
                                            Real &sigma_a,
                                            Real &sigma_b,
                                            Real &chi2) {
        unsigned int vectorIndex;
        Real t, sxoss, sx = 0.0, sy = 0.0, st2 = 0.0, sigma_data;
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
            Real residual = y[vectorIndex] - a - b * vectorIndex;
            chi2 += residual * residual;
        }
        // For unweighted data, evaluate typical sigma using chi2, and adjust the standard deviations.
        sigma_data = sqrt(chi2 / (vectorSize - 2));
        sigma_a *= sigma_data;
        sigma_b *= sigma_data;
    }

    AlgorithmStatus LinearRegression::process() {
        auto status = acquireData();
        if (status != OK)
        {
            if (!shouldStop()) return status;
            
            return NO_INPUT;
        }
        
        // Grab frames from stream, as requested in constructor
        const std::vector<std::vector<Real>>& vecIn = _vectorInput.tokens();
        std::vector<Real>& gradOut = _gradientOutput.tokens();
        std::vector<Real>& interOut = _intersectionOutput.tokens();

        // Loop frames in stream
        for (int i = 0; i < vecIn.size() && i < gradOut.size(); ++i)
        {
            Real gradient = 0.0; // default is that of zero gradient, i.e. no changing values.
            Real intersection;
            Real sigma_intersection;
            Real sigma_gradient;
            Real chi2;

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
