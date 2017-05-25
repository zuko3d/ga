#include "vtkAutoInit.h"
VTK_MODULE_INIT(vtkRenderingOpenGL2); // VTK was built with vtkRenderingOpenGL2
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);

#include "visualizationtester.h"

#include <sstream>

#include <vtkSmartPointer.h>

#include <vtkCircularLayoutStrategy.h>
#include <vtkTreeLayoutStrategy.h>
#include <vtkSpanTreeLayoutStrategy.h>
#include <vtkFast2DLayoutStrategy.h>
#include <vtkForceDirectedLayoutStrategy.h>
#include <vtkConstrained2DLayoutStrategy.h>
#include <vtkAssignCoordinatesLayoutStrategy.h>

#include <vtkDataSetAttributes.h>
#include <vtkDoubleArray.h>
#include <vtkGraphLayoutView.h>
#include <vtkIntArray.h>
#include <vtkMutableUndirectedGraph.h>
#include <vtkMutableDirectedGraph.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkAssignCoordinates.h>
#include <vtkPoints.h>

#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>

VisualizationTester::VisualizationTester()
{

}

void VisualizationTester::doMinimalTest()
{
    vtkSmartPointer<vtkMutableDirectedGraph> g =
        vtkSmartPointer<vtkMutableDirectedGraph>::New();

      // Create 3 vertices
      vtkIdType v1 = g->AddVertex();
      vtkIdType v2 = g->AddVertex();
      vtkIdType v3 = g->AddVertex();

      // Create a fully connected graph
      g->AddEdge(v1, v2);
      g->AddEdge(v2, v3);
      //g->AddEdge(v1, v3);

      // Create the edge weight array
      vtkSmartPointer<vtkDoubleArray> weights =
        vtkSmartPointer<vtkDoubleArray>::New();
      weights->SetNumberOfComponents(1);
      weights->SetName("Weights");

      // Set the edge weights
      weights->InsertNextValue(1.0);
      weights->InsertNextValue(1.0);
      weights->InsertNextValue(2.0);

      // Create an array for the vertex labels
      vtkSmartPointer<vtkIntArray> vertexIDs =
        vtkSmartPointer<vtkIntArray>::New();
      vertexIDs->SetNumberOfComponents(1);
      vertexIDs->SetName("VertexIDs");

      // Set the vertex labels
      vertexIDs->InsertNextValue(0);
      vertexIDs->InsertNextValue(1);
      vertexIDs->InsertNextValue(2);

      // Add the edge weight array to the graph
      g->GetEdgeData()->AddArray(weights);
      g->GetVertexData()->AddArray(vertexIDs);

      vtkSmartPointer<vtkCircularLayoutStrategy> layoutStrategy =
        vtkSmartPointer<vtkCircularLayoutStrategy>::New();


      vtkSmartPointer<vtkGraphLayoutView> view =
        vtkSmartPointer<vtkGraphLayoutView>::New();
      view->AddRepresentationFromInput(g);

      view->SetLayoutStrategy(layoutStrategy);
      view->SetVertexLabelVisibility(true);
      view->SetEdgeLabelVisibility(true);
      view->SetEdgeLabelArrayName("Weights"); //default is "labels"
      view->SetVertexLabelArrayName("VertexIDs"); //default is "labels"
      view->ResetCamera();
      view->Render();
      view->GetInteractor()->Start();
}

void VisualizationTester::multipleWindows()
{
    std::vector<vtkSmartPointer<vtkRenderWindowInteractor> > interactors;

      for(unsigned int i = 0; i < 4; i++)
        {
        vtkSmartPointer<vtkRenderWindow> renderWindow =
          vtkSmartPointer<vtkRenderWindow>::New();

        vtkSmartPointer<vtkRenderer> renderer =
          vtkSmartPointer<vtkRenderer>::New();

        renderWindow->AddRenderer(renderer);

        vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
          vtkSmartPointer<vtkRenderWindowInteractor>::New();

        interactors.push_back(renderWindowInteractor);

        renderWindowInteractor->SetRenderWindow(renderWindow);
        renderWindow->Render();
        std::stringstream ss;
        ss << "Window " << i;
        renderWindow->SetWindowName(ss.str().c_str());
        renderWindow->SetPosition(i*300,0);

        //create a sphere
        vtkSmartPointer<vtkSphereSource> sphereSource =
          vtkSmartPointer<vtkSphereSource>::New();
        sphereSource->SetCenter(0.0, 0.0, 0.0);
        sphereSource->SetRadius(5.0);
        sphereSource->Update();

        //Create a mapper and actor
        vtkSmartPointer<vtkPolyDataMapper> mapper =
          vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputConnection(sphereSource->GetOutputPort());

        vtkSmartPointer<vtkActor> actor =
          vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);
        renderer->AddActor(actor);
        renderer->ResetCamera();
        }

      interactors[3]->Start();
}



void VisualizationTester::visVector(std::vector<std::vector<uint32_t> > values, vtkSmartPointer<vtkGraphLayoutView>& view)
{
    vtkSmartPointer<vtkMutableDirectedGraph> g =
        vtkSmartPointer<vtkMutableDirectedGraph>::New();

    vtkSmartPointer<vtkIntArray> vertexIDs =
      vtkSmartPointer<vtkIntArray>::New();
    vertexIDs->SetNumberOfComponents(1);
    vertexIDs->SetName("VertexIDs");

    // Create 3 vertices
    std::vector<std::vector<vtkIdType> > vertices;

    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    size_t max = 0.0;
    for(const auto& v: values) {
        if(v.size() > max) max = v.size();
    }

    double center = static_cast<double>(max) / 2.0;

    vertices.resize(values.size());
    for(size_t i = 0; i < values.size(); i++){
        vertices[i].reserve(values[i].size());
        double coord = 1.0 + center - static_cast<double>(values[i].size()) / 2.0;
        for(const auto& value: values[i]) {
            vertices[i].push_back(g->AddVertex());
            vertexIDs->InsertNextValue(value);
            points->InsertNextPoint(static_cast<double>(i), coord, 0.0);
            coord++;
        }
        if(i > 0) {
            for(const auto& prevVertex: vertices[i-1]) {
                for(const auto& vertex: vertices[i]) {
                    g->AddEdge(prevVertex, vertex);
                }
            }
        }
    }

    g->SetPoints(points);
      // Create the edge weight array
      vtkSmartPointer<vtkDoubleArray> weights =
        vtkSmartPointer<vtkDoubleArray>::New();
      weights->SetNumberOfComponents(1);
      weights->SetName("Weights");

      // Set the edge weights
      weights->InsertNextValue(1.0);
      weights->InsertNextValue(1.0);
      weights->InsertNextValue(2.0);

      // Add the edge weight array to the graph
      //g->GetEdgeData()->AddArray(weights);
      g->GetVertexData()->AddArray(vertexIDs);

      vtkSmartPointer<vtkAssignCoordinatesLayoutStrategy> layoutStrategy =
        vtkSmartPointer<vtkAssignCoordinatesLayoutStrategy>::New();

      layoutStrategy->SetYCoordArrayName("Ycoords");
      //layoutStrategy->SetXCoordArrayName("Xcoords");

      view->AddRepresentationFromInput(g);

      view->SetLayoutStrategy("Pass Through");
      view->SetVertexLabelVisibility(true);
      view->SetEdgeLabelVisibility(true);
      view->SetEdgeLabelArrayName("Weights"); //default is "labels"
      view->SetVertexLabelArrayName("VertexIDs"); //default is "labels"
      view->ResetCamera();
      view->Render();
      //view->GetInteractor()->Start();
}

void VisualizationTester::visNetwork(const MultilayerPerceptron &mlp, vtkSmartPointer<vtkGraphLayoutView>& view)
{
    vtkSmartPointer<vtkMutableDirectedGraph> g =
        vtkSmartPointer<vtkMutableDirectedGraph>::New();

    vtkSmartPointer<vtkIntArray> vertexIDs =
      vtkSmartPointer<vtkIntArray>::New();
    vertexIDs->SetNumberOfComponents(1);
    vertexIDs->SetName("VertexIDs");

    // Create 3 vertices
    std::vector<std::vector<vtkIdType> > vertices;

    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    size_t max = 0.0;
    auto& values = mlp.barriers_;
    for(const auto& v: values) {
        if(v.size() > max) max = v.size();
    }

    double center = static_cast<double>(max) / 2.0;

    vtkSmartPointer<vtkDoubleArray> weights =
      vtkSmartPointer<vtkDoubleArray>::New();
    weights->SetNumberOfComponents(1);
    weights->SetName("Weights");

    vertices.resize(values.size());
    for(size_t i = 0; i < values.size(); i++){
        vertices[i].reserve(values[i].size());
        double coord = 1.0 + center - static_cast<double>(values[i].size()) / 2.0;
        for(const auto& value: values[i]) {
            vertices[i].push_back(g->AddVertex());
            vertexIDs->InsertNextValue(value);
            points->InsertNextPoint(static_cast<double>(i), coord, 0.0);
            coord++;
        }
        if(i > 0) {
            for(size_t j = 0; j < vertices[i-1].size(); j++) {
                for(size_t k = 0; k < vertices[i].size(); k++) {
                    if(mlp.weights_[i-1][j][k] != 0) {
                        g->AddEdge(vertices[i-1][j], vertices[i][k]);
                        weights->InsertNextValue(mlp.weights_[i-1][j][k]);
                    }
                }
            }
        }
    }

    g->SetPoints(points);

      // Add the edge weight array to the graph
      g->GetEdgeData()->AddArray(weights);
      g->GetVertexData()->AddArray(vertexIDs);

      view->AddRepresentationFromInput(g);

      view->SetLayoutStrategy("Pass Through");
      view->SetVertexLabelVisibility(true);
      view->SetEdgeLabelVisibility(true);
      view->SetEdgeLabelArrayName("Weights"); //default is "labels"
      view->SetVertexLabelArrayName("VertexIDs"); //default is "labels"
      view->ResetCamera();
      view->Render();
}
