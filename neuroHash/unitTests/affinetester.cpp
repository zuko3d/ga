#include "affinetester.h"

#include "src/compgraph/affine.h"

#include <vector>

int AffineTester::doNumericTest()
{
    int errors = 0;

    std::cout << __FUNCTION__ << std::endl;

    Affine g(2, 3, {2, -5, 6, -2, 3, 8});
    std::vector<double> in = {1, 2, 3};
    std::vector<double> out;

    out.resize(2);

    g.forward(in, out);

    if(out != std::vector<double>({10, 28})) {
        std::cerr << __FUNCTION__ << " Error!" << std::endl;
        errors++;
    }

    std::cout << "Total errors: " << errors << std::endl;

    return errors;
}
