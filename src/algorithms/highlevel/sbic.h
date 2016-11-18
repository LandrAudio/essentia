/*
 * Copyright (C) 2006-2016  Music Technology Group - Universitat Pompeu Fabra
 *
 * This file is part of Essentia
 *
 * Essentia is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Affero General Public License as published by the Free
 * Software Foundation (FSF), either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the Affero GNU General Public License
 * version 3 along with this program.  If not, see http://www.gnu.org/licenses/
 */

#ifndef ESSENTIA_SBIC_H
#define ESSENTIA_SBIC_H

#include "algorithm.h"
#include "tnt/tnt.h"

namespace essentia {
namespace standard {

class SBic : public Algorithm {

 private:
  Input<TNT::Array2D<Real> > _features;
  Output<std::vector<Real> > _segmentation;
  Output<std::vector<Real> > _segValues;
  Output<std::vector<Real> > _bicValues;

  int _size1;
  int _size2;
  int _inc1;
  int _inc2;
  Real _cpw;
  Real _cp; // complexity penalty

 public:
  SBic() {
    declareInput(_features, "features", "extracted features matrix (rows represent features, and columns represent frames of audio)");
    declareOutput(_segmentation, "segmentation", "a list of frame indices that indicate where a segment of audio begins/ends (the indices of the first and last frame are also added to the list at the beginning and end, respectively)");
    declareOutput(_segValues, "segmentationValues", "a list of values, one per segment, of a log measure of the change in the variance of the feature values between adjacent segments. The further below 0, the greater the change.");
    declareOutput(_bicValues, "bicValues", "all the values of the BIC variation, from one frame to the next. The further below 0, the greater the likelihood of a change.");
  }

  ~SBic() {}

  void declareParameters() {
    declareParameter("size1", "first pass window size [frames]", "[1,inf)", 300);
    declareParameter("inc1", "first pass increment [frames]", "[1,inf)", 60);
    declareParameter("size2", "second pass window size [frames]", "[1,inf)", 200);
    declareParameter("inc2", "second pass increment [frames]", "[1,inf)", 20);
    declareParameter("cpw", "complexity penalty weight", "[0,inf)", 1.5);
  }

  void compute();
  void configure();

  static const char* name;
  static const char* category;
  static const char* description;

 private:
  Real logDet(const TNT::Array2D<Real>& matrix) const;
  std::pair <int, std::vector<Real> > bicChangeSearch(const TNT::Array2D<Real>& matrix, int inc, int current, Real& dmin) const;
  Real delta_bic(const TNT::Array2D<Real>& matrix, Real segPoint) const;
};

} // namespace standard
} // namespace essentia

#include "streamingalgorithmwrapper.h"

namespace essentia {
namespace streaming {

class SBic : public StreamingAlgorithmWrapper {

 protected:
  Sink<TNT::Array2D<Real> > _features;
  Source<std::vector<Real> > _segmentation;
  Source<std::vector<Real> > _segValues;
  Source<std::vector<Real> > _bicValues;

 public:
  SBic() {
    declareAlgorithm("SBic");
    declareInput(_features, TOKEN, "features");
    declareOutput(_segmentation, TOKEN, "segmentation");
    declareOutput(_segValues, TOKEN, "segmentationValues");
    declareOutput(_bicValues, TOKEN, "bicValues");
  }
};

} // namespace streaming
} // namespace essentia

#endif // ESSENTIA_SBIC_H
