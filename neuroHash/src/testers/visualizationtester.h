#ifndef VISUALIZATIONTESTER_H
#define VISUALIZATIONTESTER_H

#include "../global/global.h"
#include <vector>
#include <vtkSmartPointer.h>
#include <vtkGraphLayoutView.h>
#include "../neuralnetwork/MultilayerPerceptron.h"

class VisualizationTester
{
public:
    VisualizationTester();

    static void doMinimalTest();
    static void multipleWindows();

    static void visVector(std::vector< std::vector<uint32_t> > values, vtkSmartPointer<vtkGraphLayoutView> &view);

    static void visNetwork(const MultilayerPerceptron& mlp, vtkSmartPointer<vtkGraphLayoutView> &view);
};

#endif // VISUALIZATIONTESTER_H
