#include "logistictester.h"

#include "src/compgraph/logistic.h"

#include <math.h>

int LogisticTester::doNumericTest()
{
    int errors = 0;

    std::cout << __FUNCTION__ << std::endl;

    Logistic g;
    size_t ntests = 10000;
    std::vector<double> in(ntests);
    std::vector<double> out(ntests);
    std::vector<double> right(ntests);

    for(size_t i = 0; i < ntests; i++) {
        double num = hRnd() * 100.0;
        in[i] = log(num);
        right[i] = 1.0 / (1.0 + 1.0 / num);
    }

    in[0] = 0.0;
    right[0] = 0.5;

    in[1] = -1000.0;
    right[1] = 0.0;

    in[2] = 1000.0;
    right[2] = 1.0;

    g.forward(in, out);

    for(size_t i = 0; i < ntests; i++) {
        if(fabs(out[i] - right[i]) > 1e-9) {
            std::cerr << __FUNCTION__ << " Error on number " << in[i] << std::endl;
            errors++;
        }
    }

    std::cout << "Total errors: " << errors << std::endl;

    return errors;
}
