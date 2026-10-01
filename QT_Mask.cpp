#include <QCoreApplication>
#include <QDebug>
#include "GrammarEngine.h"
#include "TopologyEngine.h"
#include "GeometryEngine.h"
#include "ProjectionEngine.h"
#include "MaskWriter.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    GrammarEngine grammar;
    grammar.addRule("A", "AB");
    grammar.addRule("B", "A");
    QString output = grammar.expand("A", 6);

    qDebug() << "Expanded L-system:" << output;

    TopologyEngine topo;
    QVector<Edge> edges = topo.buildGraph(output);
    topo.applyReidemeisterII(edges);

    GeometryEngine geom;
    QPolygonF poly = geom.toPolygon(edges);

    ProjectionEngine proj;
    QPolygonF projected = proj.applyAxonometric(poly, -0.5, 0.5);

    MaskWriter writer;
    if (writer.writeSVG(projected, "output.svg"))
        qDebug() << "SVG mask written to output.svg";
    else
        qDebug() << "Failed to write SVG.";

    return 0;
}
