#ifndef PHX_GRID_GENERATOR_H
#define PHX_GRID_GENERATOR_H

#include "phx_grid_Loadable.h"
#include "phx_grid_Quad.h"

#include "Teuchos_RefCountPtr.hpp"

namespace phx {
namespace grid {

class Generator 
{
public:
  static void
  getSquareWithTriangles(Epetra_Comm& comm, 
                         const int numGlobalElementsX, const int numGlobalElementsY,
                         const int numDomainsX, const int numDomainsY,
                         phx::grid::Loadable& domain, phx::grid::Loadable& boundary)
  {
    getSquare(comm, numGlobalElementsX, numGlobalElementsY,
              numDomainsX, numDomainsY, domain, boundary, "Triangle");
  }

  static void
  getSquareWithQuads(Epetra_Comm& comm, 
                    const int numGlobalElementsX, const int numGlobalElementsY,
                    const int numDomainsX, const int numDomainsY,
                    phx::grid::Loadable& domain, phx::grid::Loadable& boundary)
  {
    getSquare(comm, numGlobalElementsX, numGlobalElementsY,
              numDomainsX, numDomainsY, domain, boundary, "Quad");
  }

  static void
  getSquare(Epetra_Comm& comm, 
            const int numGlobalElementsX, const int numGlobalElementsY,
            const int numDomainsX, const int numDomainsY,
            phx::grid::Loadable& domain, phx::grid::Loadable& boundary,
            const string what)
  {
    TEST_FOR_EXCEPTION(numDomainsX * numDomainsY != comm.NumProc(), std::logic_error,
                       "the number of processor should equal numDomainsX * numDomainsY"
                       << ", now numProcs = " << comm.NumProc()
                       << " and numDomainsX * numDomainsY = " << numDomainsX * numDomainsY);

    TEST_FOR_EXCEPTION(numGlobalElementsX % numDomainsX != 0, std::logic_error,
                       "numGlobalElementsX must be a multiple of numDomainsX");

    TEST_FOR_EXCEPTION(numGlobalElementsY % numDomainsY != 0, std::logic_error,
                       "numGlobalElementsY must be a multiple of numDomainsY");

    double lx = 1.0;
    double ly = 1.0;

    // these are the global number of elements and vertices
    int numGlobalElements = numGlobalElementsX * numGlobalElementsY;
    if (what == "Triangle") numGlobalElements *= 2;
    int numGlobalVertices = (numGlobalElementsX + 1) * (numGlobalElementsY + 1);

    int numGlobalVerticesX = numGlobalElementsX + 1;
    int numGlobalVerticesY = numGlobalElementsY + 1;

    // these are the mesh sizes, hx and hy
    double deltax = lx / numGlobalElementsX;
    double deltay = ly / numGlobalElementsY;

    // (px, py) are the coordinates of this processor.
    int px = comm.MyPID() % numDomainsX;
    int py = comm.MyPID() / numDomainsX;

    // (numMyElementsX, numMyElementsY) are the number of elements
    // in the square assigned to this processor, and
    // (numMyVerticesX, numMyVerticesY) the number of vertices.
    int numMyElementsX = numGlobalElementsX / numDomainsX;
    int numMyElementsY = numGlobalElementsY / numDomainsY;

    int numMyVerticesX = numMyElementsX + 1;
    int numMyVerticesY = numMyElementsY + 1;

    // (sx, sy) are the coordinates of the first element of this processor.
    int sx = px * numMyElementsX;
    int sy = py * numMyElementsY;

    // and these are the number of vertices and elements assigned
    // to this processor.
    int numMyElements = numMyElementsX * numMyElementsY;
    if (what == "Triangle") numMyElements *= 2;
    int numMyVertices = (numMyElementsX + 1) * (numMyElementsY + 1);

    domain.initialize(comm, numGlobalElements, numMyElements, "Triangle");

    int elementOffset = numMyElements * comm.MyPID();
    int vertexOffset  = px * numMyElementsX + py * numMyElementsY * numGlobalVerticesX;

    int count = 0;
    if (what == "Triangle")
    {
      for (int iy = 0; iy < numMyElementsY; ++iy)
      {
        for (int ix = 0; ix < numMyElementsX; ++ix)
        {
          int GEID = elementOffset + count++;
          int GVID = vertexOffset + ix + iy * numGlobalVerticesX;

          domain.setGlobalConnectivity(GEID, 0, GVID);
          domain.setGlobalConnectivity(GEID, 1, GVID + 1);
          domain.setGlobalConnectivity(GEID, 2, GVID + 2 + numGlobalElementsX);

          GEID = elementOffset + count++;
          domain.setGlobalConnectivity(GEID, 0, GVID + 2 + numGlobalElementsX);
          domain.setGlobalConnectivity(GEID, 1, GVID + 1 + numGlobalElementsX);
          domain.setGlobalConnectivity(GEID, 2, GVID);
        }
      }
    }
    else
    {
      for (int iy = 0; iy < numMyElementsY; ++iy)
      {
        for (int ix = 0; ix < numMyElementsX; ++ix)
        {
          int GEID = elementOffset + count++;
          int GVID = vertexOffset + ix + iy * numGlobalVerticesX;

          domain.setGlobalConnectivity(GEID, 0, GVID);
          domain.setGlobalConnectivity(GEID, 1, GVID + 1);
          domain.setGlobalConnectivity(GEID, 2, GVID + 2 + numGlobalElementsX);
          domain.setGlobalConnectivity(GEID, 3, GVID + 1 + numGlobalElementsX);
        }
      }
    }

    domain.freezeConnectivity();

    for (int iy = 0; iy < numMyVerticesY; ++iy)
    {
      for (int ix = 0; ix < numMyVerticesX; ++ix)
      {
        int GVID = vertexOffset + ix + iy * numGlobalVerticesX;

        domain.setGlobalCoordinates(GVID, 0, (sx + ix) * deltax);
        domain.setGlobalCoordinates(GVID, 1, (sy + iy) * deltay);
      }
    }

    domain.freezeCoordinates();

    // now build boundary faces

    int numMyBoundaries = 0;
    
    if (py == 0)               numMyBoundaries += numMyElementsX;
    if (py == numDomainsY - 1) numMyBoundaries += numMyElementsX;
    
    if (px == 0)               numMyBoundaries += numMyElementsY;
    if (px == numDomainsX - 1) numMyBoundaries += numMyElementsY;

    int pos = 0;
    vector<int> list(numMyBoundaries);

    if (py == 0)
    {
      int offset = px * numMyElementsX;

      for (int i = 0; i < numMyElementsX; ++i)
        list[pos++] = offset + i;
    }

    if (px == numDomainsX - 1)
    {
      int offset = numGlobalElementsX + py * numMyElementsY;

      for (int i = 0; i < numMyElementsY; ++i)
        list[pos++] = offset + i;
    }

    if (py == numDomainsY - 1)
    {
      int offset = numGlobalElementsX + numGlobalElementsY + px * numMyElementsX;

      for (int i = 0; i < numMyElementsX; ++i)
        list[pos++] = offset + i;
    }

    if (px == 0)
    {
      int offset = 2 * numGlobalElementsX + numGlobalElementsY + py * numMyElementsY;

      for (int i = 0; i < numMyElementsY; ++i)
        list[pos++] = offset + i;
    }

    TEST_FOR_EXCEPTION(pos != numMyBoundaries, std::logic_error,
                       "internal error in boundary list definition, " 
                       << pos << " vs. " << numMyBoundaries);

    boundary.initialize(comm, -1, numMyBoundaries, "Segment", &list[0]);

    // now insert the actual vertices in the grid

    if (py == 0)
    {
      int offset = px * numMyElementsX;

      for (int i = 0; i < numMyElementsX; ++i)
      {
        boundary.setGlobalConnectivity(offset + i, 0, offset + i);
        boundary.setGlobalConnectivity(offset + i, 1, offset + i + 1);
      }
    }

    if (px == numDomainsX - 1)
    {
      int offset = numGlobalVerticesX * py * numMyElementsY + numGlobalElementsX;
      int offset2 = numGlobalElementsX + py * numMyElementsY;

      for (int i = 0; i < numMyElementsY; ++i)
      {
        boundary.setGlobalConnectivity(offset2 + i, 0, offset + i * numGlobalVerticesX);
        boundary.setGlobalConnectivity(offset2 + i, 1, offset + (i + 1) * numGlobalVerticesX);
      }
    }

    if (py == numDomainsY - 1)
    {
      int offset = numGlobalVerticesX * numGlobalElementsY + px * numMyElementsX;
      int offset2 = numGlobalElementsX + numGlobalElementsY + px * numMyElementsX;

      for (int i = 0; i < numMyElementsX; ++i)
      {
        boundary.setGlobalConnectivity(offset2 + i, 0, offset + i);
        boundary.setGlobalConnectivity(offset2 + i, 1, offset + i + 1);
      }
    }

    if (px == 0)
    {
      int offset = numGlobalVerticesX * py * numMyElementsY;
      int offset2 = 2 * numGlobalElementsX + numGlobalElementsY + py * numMyElementsY;

      for (int i = 0; i < numMyElementsY; ++i)
      {
        boundary.setGlobalConnectivity(offset2 + i, 0, offset + i * numGlobalVerticesX);
        boundary.setGlobalConnectivity(offset2 + i, 1, offset + (i + 1) * numGlobalVerticesX);
      }
    }

    boundary.freezeConnectivity();

    if (py == 0)
    {
      int offset = px * numMyElementsX + 1;

      for (int i = 0; i < numMyElementsX + 1; ++i)
      {
        boundary.setGlobalCoordinates(offset + i, 0, deltax * (offset + i));
        boundary.setGlobalCoordinates(offset + i, 1, 0.0);
      }
    }

    if (px == numDomainsX - 1)
    {
      int offset = numGlobalVerticesX + py * numMyElementsY - 1;
      int offset2 = px * numMyElementsX;

      for (int i = 0; i < numMyElementsY + 1; ++i)
      {
        boundary.setGlobalCoordinates(offset + i * numGlobalVerticesX, 0, lx);
        boundary.setGlobalCoordinates(offset + i * numGlobalVerticesX, 1, deltay * (offset2 + i));
      }
    }

    if (py == numDomainsY - 1)
    {
      int offset = px * numMyElementsX;
      int offset2 = numGlobalVerticesX * numGlobalElementsY + px * numMyElementsX;

      for (int i = 0; i < numMyElementsX + 1; ++i)
      {
        boundary.setGlobalCoordinates(offset2 + i, 0, deltax * (offset + i));
        boundary.setGlobalCoordinates(offset2 + i, 1, ly);
      }
    }

    if (px == 0)
    {
      int offset = numGlobalVerticesX * py * numMyElementsY;
      int offset2 = py * numMyElementsX;

      for (int i = 0; i < numMyElementsY + 1; ++i)
      {
        boundary.setGlobalCoordinates(offset + i * numGlobalVerticesX, 0, 0.0);
        boundary.setGlobalCoordinates(offset + i * numGlobalVerticesX, 1, deltay * (offset2 + i));
      }
    }

    boundary.freezeCoordinates();
  }

#if 0
  static void
  getSquareWithQuads(Epetra_Comm& comm, 
                    const int numGlobalElementsX, const int numGlobalElementsY,
                    const int numDomainsX, const int numDomainsY,
                    phx::grid::Loadable& domain, phx::grid::Loadable& boundary)
  {
    TEST_FOR_EXCEPTION(numDomainsX * numDomainsY != comm.NumProc(), std::logic_error,
                       "the number of processor should equal numDomainsX * numDomainsY"
                       << ", now numProcs = " << comm.NumProc()
                       << " and numDomainsX * numDomainsY = " << numDomainsX * numDomainsY);

    TEST_FOR_EXCEPTION(numGlobalElementsX % numDomainsX != 0, std::logic_error,
                       "numGlobalElementsX must be a multiple of numDomainsX");

    TEST_FOR_EXCEPTION(numGlobalElementsY % numDomainsY != 0, std::logic_error,
                       "numGlobalElementsY must be a multiple of numDomainsY");

    double lx = 1.0;
    double ly = 1.0;

    // these are the global number of elements and vertices
    int numGlobalElements = numGlobalElementsX * numGlobalElementsY;
    int numGlobalVertices = (numGlobalElementsX + 1) * (numGlobalElementsY + 1);

    int numGlobalVerticesX = numGlobalElementsX + 1;
    int numGlobalVerticesY = numGlobalElementsY + 1;

    // these are the mesh sizes, hx and hy
    double deltax = lx / numGlobalElementsX;
    double deltay = ly / numGlobalElementsY;

    // (px, py) are the coordinates of this processor.
    int px = comm.MyPID() % numDomainsX;
    int py = comm.MyPID() / numDomainsX;

    // (numMyElementsX, numMyElementsY) are the number of elements
    // in the square assigned to this processor, and
    // (numMyVerticesX, numMyVerticesY) the number of vertices.
    int numMyElementsX = numGlobalElementsX / numDomainsX;
    int numMyElementsY = numGlobalElementsY / numDomainsY;

    int numMyVerticesX = numMyElementsX + 1;
    int numMyVerticesY = numMyElementsY + 1;

    // (sx, sy) are the coordinates of the first element of this processor.
    int sx = px * numMyElementsX;
    int sy = py * numMyElementsY;

    // and these are the number of vertices and elements assigned
    // to this processor.
    int numMyElements = numMyElementsX * numMyElementsY;
    int numMyVertices = (numMyElementsX + 1) * (numMyElementsY + 1);

    domain.initialize(comm, numGlobalElements, numMyElements, "Quad");

    int elementOffset = numMyElements * comm.MyPID();
    int vertexOffset  = px * numMyElementsX + py * numMyElementsY * numGlobalVerticesX;

    int count = 0;
    for (int iy = 0; iy < numMyElementsY; ++iy)
    {
      for (int ix = 0; ix < numMyElementsX; ++ix)
      {
        int GEID = elementOffset + count++;
        int GVID = vertexOffset + ix + iy * numGlobalVerticesX;

        domain.setGlobalConnectivity(GEID, 0, GVID);
        domain.setGlobalConnectivity(GEID, 1, GVID + 1);
        domain.setGlobalConnectivity(GEID, 2, GVID + 2 + numGlobalElementsX);
        domain.setGlobalConnectivity(GEID, 3, GVID + 1 + numGlobalElementsX);
      }
    }

    domain.freezeConnectivity();

    for (int iy = 0; iy < numMyVerticesY; ++iy)
    {
      for (int ix = 0; ix < numMyVerticesX; ++ix)
      {
        int GVID = vertexOffset + ix + iy * numGlobalVerticesX;

        domain.setGlobalCoordinates(GVID, 0, (sx + ix) * deltax);
        domain.setGlobalCoordinates(GVID, 1, (sy + iy) * deltay);
      }
    }

    domain.freezeCoordinates();

    // now build boundary faces

    int numMyBoundaries = 0;
    
    if (py == 0)               numMyBoundaries += numMyElementsX;
    if (py == numDomainsY - 1) numMyBoundaries += numMyElementsX;
    
    if (px == 0)               numMyBoundaries += numMyElementsY;
    if (px == numDomainsX - 1) numMyBoundaries += numMyElementsY;

    int pos = 0;
    vector<int> list(numMyBoundaries);

    if (py == 0)
    {
      int offset = px * numMyElementsX;

      for (int i = 0; i < numMyElementsX; ++i)
        list[pos++] = offset + i;
    }

    if (px == numDomainsX - 1)
    {
      int offset2 = numGlobalElementsX + py * numMyElementsY;

      for (int i = 0; i < numMyElementsY; ++i)
        list[pos++] = offset2 + i;
    }

    if (py == numDomainsY - 1)
    {
      int offset2 = numGlobalElementsX + numGlobalElementsY + px * numMyElementsX;

      for (int i = 0; i < numMyElementsX; ++i)
        list[pos++] = offset2 + i;
    }

    if (px == 0)
    {
      int offset2 = 2 * numGlobalElementsX + numGlobalElementsY + py * numMyElementsX;

      for (int i = 0; i < numMyElementsY; ++i)
        list[pos++] = offset2 + i;
    }

    TEST_FOR_EXCEPTION(pos != numMyBoundaries, std::logic_error,
                       "internal error in boundary list definition, " 
                       << pos << " vs. " << numMyBoundaries);

    boundary.initialize(comm, -1, numMyBoundaries, "Segment", &list[0]);

    // now insert the actual vertices in the grid

    if (py == 0)
    {
      int offset = px * numMyElementsX;

      for (int i = 0; i < numMyElementsX; ++i)
      {
        boundary.setGlobalConnectivity(offset + i, 0, offset + i);
        boundary.setGlobalConnectivity(offset + i, 1, offset + i + 1);
      }
    }

    if (px == numDomainsX - 1)
    {
      int offset = numGlobalVerticesX + py * numMyElementsY - 1;
      int offset2 = numGlobalElementsX + py * numMyElementsY;

      for (int i = 0; i < numMyElementsX; ++i)
      {
        boundary.setGlobalConnectivity(offset2 + i, 0, offset + i * numGlobalVerticesX);
        boundary.setGlobalConnectivity(offset2 + i, 1, offset + (i + 1) * numGlobalVerticesX);
      }
    }

    if (py == numDomainsY - 1)
    {
      int offset = numGlobalVerticesX * numGlobalElementsY + px * numMyElementsX;
      int offset2 = numGlobalElementsX + numGlobalElementsY + px * numMyElementsX;

      for (int i = 0; i < numMyElementsX; ++i)
      {
        boundary.setGlobalConnectivity(offset2 + i, 0, offset + i);
        boundary.setGlobalConnectivity(offset2 + i, 1, offset + i + 1);
      }
    }

    if (px == 0)
    {
      int offset = numGlobalVerticesX * py * numMyElementsY;
      int offset2 = 2 * numGlobalElementsX + numGlobalElementsY + py * numMyElementsX;

      for (int i = 0; i < numMyElementsY; ++i)
      {
        boundary.setGlobalConnectivity(offset2 + i, 0, offset + i * numGlobalVerticesX);
        boundary.setGlobalConnectivity(offset2 + i, 1, offset + (i + 1) * numGlobalVerticesX);
      }
    }

    boundary.freezeConnectivity();

    if (py == 0)
    {
      int offset = px * numMyElementsX + 1;

      for (int i = 0; i < numMyElementsX + 1; ++i)
      {
        boundary.setGlobalCoordinates(offset + i, 0, deltax * (offset + i));
        boundary.setGlobalCoordinates(offset + i, 1, 0.0);
      }
    }

    if (px == numDomainsX - 1)
    {
      int offset = numGlobalVerticesX + py * numMyElementsY - 1;
      int offset2 = px * numMyElementsX;

      for (int i = 0; i < numMyElementsY + 1; ++i)
      {
        boundary.setGlobalCoordinates(offset + i * numGlobalVerticesX, 0, lx);
        boundary.setGlobalCoordinates(offset + i * numGlobalVerticesX, 1, deltay * (offset2 + i));
      }
    }

    if (py == numDomainsY - 1)
    {
      int offset = px * numMyElementsX;
      int offset2 = numGlobalVerticesX * numGlobalElementsY + px * numMyElementsX;

      for (int i = 0; i < numMyElementsX + 1; ++i)
      {
        boundary.setGlobalCoordinates(offset2 + i, 0, deltax * (offset + i));
        boundary.setGlobalCoordinates(offset2 + i, 1, ly);
      }
    }

    if (px == 0)
    {
      int offset = numGlobalVerticesX * py * numMyElementsY;
      int offset2 = py * numMyElementsX;

      for (int i = 0; i < numMyElementsY + 1; ++i)
      {
        boundary.setGlobalCoordinates(offset + i * numGlobalVerticesX, 0, 0.0);
        boundary.setGlobalCoordinates(offset + i * numGlobalVerticesX, 1, deltay * (offset2 + i));
      }
    }

    boundary.freezeCoordinates();
  }
#endif

}; // class Generator

} // namespace grid
} // namespace phx

#endif
