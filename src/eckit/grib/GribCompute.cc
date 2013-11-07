// File GribCompute.cc
// Baudouin Raoult - (c) ECMWF Nov 13

#include "GribCompute.h"
#include "GribField.h"

namespace eckit {
namespace compute {


void mean(const GribFieldSet& in, GribFieldSet& out) {
    size_t nfields = in.count();
    ASSERT(out.count() == 0);

    if(!nfields) {
        return;
    }


    size_t npoints = 0;
    GribField* first = in.willAdopt(0);
    const double* values = first->getValues(npoints);
    double* result = new double[npoints];
    ASSERT(result);

    std::copy(values, values + npoints, result);

    for(size_t j = 1; j < nfields; ++j) {
         size_t n = 0;
        const GribField* first = in.get(0);
        const double* values = first->getValues(n);
        ASSERT(n == npoints);
         for(size_t i = 0; i < npoints; ++i) {
             result[i] += values[i];
         }
    }

    for(size_t i = 0; i < npoints; ++i) {
        result[i] /= npoints;
    }

    out.add(new GribField(first, result, npoints));
}

} // namespace compute
} // namespace eckit
