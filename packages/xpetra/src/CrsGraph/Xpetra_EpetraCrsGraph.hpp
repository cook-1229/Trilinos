// @HEADER
//
// ***********************************************************************
//
//             Xpetra: A linear algebra interface package
//                  Copyright 2012 Sandia Corporation
//
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact
//                    Jonathan Hu       (jhu@sandia.gov)
//                    Andrey Prokopenko (aprokop@sandia.gov)
//                    Ray Tuminaro      (rstumin@sandia.gov)
//
// ***********************************************************************
//
// @HEADER
#ifndef XPETRA_EPETRACRSGRAPH_HPP
#define XPETRA_EPETRACRSGRAPH_HPP

/* this file is automatically generated - do not edit (see script/epetra.py) */

#include "Xpetra_EpetraConfigDefs.hpp"

#include "Xpetra_CrsGraph.hpp"

#include "Xpetra_EpetraExport.hpp"
#include "Xpetra_EpetraImport.hpp"
#include "Xpetra_EpetraMap.hpp"
#include "Xpetra_EpetraUtils.hpp"

#include <Epetra_CrsGraph.h>

namespace Xpetra {

  // TODO: move that elsewhere
  template<class GlobalOrdinal>
  RCP< const CrsGraph<int, GlobalOrdinal> >
  toXpetra (const Epetra_CrsGraph& graph);

  template<class GlobalOrdinal>
  const Epetra_CrsGraph&
  toEpetra (const RCP<const CrsGraph<int, GlobalOrdinal> > &graph);

  template<class EpetraGlobalOrdinal>
  class EpetraCrsGraphT
    : public CrsGraph<int, EpetraGlobalOrdinal>
  {
    typedef int LocalOrdinal;
    typedef EpetraGlobalOrdinal GlobalOrdinal;
    typedef typename CrsGraph<LocalOrdinal, GlobalOrdinal>::node_type Node;
    //! The specialization of Map used by this class.
    typedef Map<LocalOrdinal,GlobalOrdinal,Node> map_type;

  public:

    //! @name Constructor/Destructor Methods
    //@{
    //! Constructor specifying fixed number of entries for each row.
    EpetraCrsGraphT(const RCP< const map_type > &rowMap, size_t maxNumEntriesPerRow, ProfileType pftype=DynamicProfile, const RCP< ParameterList > &params=null);


    ////! Constructor specifying (possibly different) number of entries in each row.
	// Definition not in cpp, so comment out
    //EpetraCrsGraphT(const RCP< const Map< LocalOrdinal, GlobalOrdinal, Node > > &rowMap, const ArrayRCP< const size_t > &NumEntriesPerRowToAlloc, ProfileType pftype=DynamicProfile, const RCP< ParameterList > &params=null);

    //! Constructor specifying column Map and fixed number of entries for each row.
    EpetraCrsGraphT(const RCP< const Map< LocalOrdinal, GlobalOrdinal, Node > > &rowMap, const RCP< const Map< LocalOrdinal, GlobalOrdinal, Node > > &colMap, size_t maxNumEntriesPerRow, ProfileType pftype=DynamicProfile, const RCP< ParameterList > &params=null);

    ////! Constructor specifying column Map and number of entries in each row.
	// Definition not in cpp, so comment out
    //EpetraCrsGraphT(const RCP< const Map< LocalOrdinal, GlobalOrdinal, Node > > &rowMap, const RCP< const Map< LocalOrdinal, GlobalOrdinal, Node > > &colMap, const ArrayRCP< const size_t > &NumEntriesPerRowToAlloc, ProfileType pftype=DynamicProfile, const RCP< ParameterList > &params=null);

    //! Destructor.
    virtual ~EpetraCrsGraphT() { }

    //@}

    //! @name Insertion/Removal Methods
    //@{

    //! Insert global indices into the graph.
    void insertGlobalIndices(GlobalOrdinal globalRow, const ArrayView< const GlobalOrdinal > &indices);

    //! Insert local indices into the graph.
    void insertLocalIndices(const LocalOrdinal localRow, const ArrayView< const LocalOrdinal > &indices);

    //! Remove all graph indices from the specified local row.
    void removeLocalIndices(LocalOrdinal localRow) { XPETRA_MONITOR("EpetraCrsGraphT::removeLocalIndices"); graph_->RemoveMyIndices(localRow); }

    //@}

    //! @name Transformational Methods
    //@{

    //! Signal that data entry is complete, specifying domain and range maps.
    void fillComplete(const RCP< const Map< LocalOrdinal, GlobalOrdinal, Node > > &domainMap, const RCP< const Map< LocalOrdinal, GlobalOrdinal, Node > > &rangeMap, const RCP< ParameterList > &params=null);

    //! Signal that data entry is complete.
    void fillComplete(const RCP< ParameterList > &params=null);

    //@}

    //! @name Methods implementing RowGraph.
    //@{

    //! Returns the communicator.
    RCP< const Comm< int > >  getComm() const {
      XPETRA_MONITOR("EpetraCrsGraphT::getComm");
      return toXpetra (graph_->Comm ());
    }

    //! Returns the Map that describes the row distribution in this graph.
    RCP< const Map< LocalOrdinal, GlobalOrdinal, Node > >  getRowMap() const { XPETRA_MONITOR("EpetraCrsGraphT::getRowMap"); return toXpetra<GlobalOrdinal>(graph_->RowMap()); }

    //! Returns the Map that describes the column distribution in this graph.
    RCP< const Map< LocalOrdinal, GlobalOrdinal, Node > >  getColMap() const { XPETRA_MONITOR("EpetraCrsGraphT::getColMap"); return toXpetra<GlobalOrdinal>(graph_->ColMap()); }

    //! Returns the Map associated with the domain of this graph.
    RCP< const Map< LocalOrdinal, GlobalOrdinal, Node > >  getDomainMap() const { XPETRA_MONITOR("EpetraCrsGraphT::getDomainMap"); return toXpetra<GlobalOrdinal>(graph_->DomainMap()); }

    //! Returns the Map associated with the domain of this graph.
    RCP< const Map< LocalOrdinal, GlobalOrdinal, Node > >  getRangeMap() const { XPETRA_MONITOR("EpetraCrsGraphT::getRangeMap"); return toXpetra<GlobalOrdinal>(graph_->RangeMap()); }

    //! Returns the importer associated with this graph.
    RCP< const Import< LocalOrdinal, GlobalOrdinal, Node > > getImporter() const { XPETRA_MONITOR("EpetraCrsGraphT::getImporter"); return toXpetra<GlobalOrdinal>(graph_->Importer()); }

    //! Returns the exporter associated with this graph.
    RCP< const Export< LocalOrdinal, GlobalOrdinal, Node > > getExporter() const { XPETRA_MONITOR("EpetraCrsGraphT::getExporter"); return toXpetra<GlobalOrdinal>(graph_->Exporter()); }

    //! Returns the number of global rows in the graph.
    global_size_t getGlobalNumRows() const { XPETRA_MONITOR("EpetraCrsGraphT::getGlobalNumRows"); return graph_->NumGlobalRows64(); }

    //! Returns the number of global columns in the graph.
    global_size_t getGlobalNumCols() const { XPETRA_MONITOR("EpetraCrsGraphT::getGlobalNumCols"); return graph_->NumGlobalCols64(); }

    //! Returns the number of graph rows owned on the calling node.
    size_t getNodeNumRows() const { XPETRA_MONITOR("EpetraCrsGraphT::getNodeNumRows"); return graph_->NumMyRows(); }

    //! Returns the number of columns connected to the locally owned rows of this graph.
    size_t getNodeNumCols() const { XPETRA_MONITOR("EpetraCrsGraphT::getNodeNumCols"); return graph_->NumMyCols(); }

    //! Returns the index base for global indices for this graph.
    GlobalOrdinal getIndexBase() const { XPETRA_MONITOR("EpetraCrsGraphT::getIndexBase"); return (GlobalOrdinal) graph_->IndexBase64(); }

    //! Returns the global number of entries in the graph.
    global_size_t getGlobalNumEntries() const { XPETRA_MONITOR("EpetraCrsGraphT::getGlobalNumEntries"); return graph_->NumGlobalEntries64(); }

    //! Returns the local number of entries in the graph.
    size_t getNodeNumEntries() const { XPETRA_MONITOR("EpetraCrsGraphT::getNodeNumEntries"); return graph_->NumMyEntries(); }

    //! Returns the current number of entries on this node in the specified global row.
    size_t getNumEntriesInGlobalRow(GlobalOrdinal globalRow) const { XPETRA_MONITOR("EpetraCrsGraphT::getNumEntriesInGlobalRow"); return graph_->NumGlobalIndices(globalRow); }

    //! Returns the current number of entries on this node in the specified local row.
    size_t getNumEntriesInLocalRow(LocalOrdinal localRow) const { XPETRA_MONITOR("EpetraCrsGraphT::getNumEntriesInLocalRow"); return graph_->NumMyIndices(localRow); }

    //! Returns the current number of allocated entries for this node in the specified global row .
    size_t getNumAllocatedEntriesInGlobalRow(GlobalOrdinal globalRow) const { XPETRA_MONITOR("EpetraCrsGraphT::getNumAllocatedEntriesInGlobalRow"); return graph_->NumAllocatedGlobalIndices(globalRow); }

    //! Returns the current number of allocated entries on this node in the specified local row.
    size_t getNumAllocatedEntriesInLocalRow(LocalOrdinal localRow) const { XPETRA_MONITOR("EpetraCrsGraphT::getNumAllocatedEntriesInLocalRow"); return graph_->NumAllocatedMyIndices(localRow); }

    //! Returns the number of global diagonal entries, based on global row/column index comparisons.
    global_size_t getGlobalNumDiags() const { XPETRA_MONITOR("EpetraCrsGraphT::getGlobalNumDiags"); return graph_->NumGlobalDiagonals64(); }

    //! Returns the number of local diagonal entries, based on global row/column index comparisons.
    size_t getNodeNumDiags() const { XPETRA_MONITOR("EpetraCrsGraphT::getNodeNumDiags"); return graph_->NumMyDiagonals(); }

    //! Maximum number of entries in all rows over all processes.
    size_t getGlobalMaxNumRowEntries() const { XPETRA_MONITOR("EpetraCrsGraphT::getGlobalMaxNumRowEntries"); return graph_->GlobalMaxNumIndices(); }

    //! Maximum number of entries in all rows owned by the calling process.
    size_t getNodeMaxNumRowEntries() const { XPETRA_MONITOR("EpetraCrsGraphT::getNodeMaxNumRowEntries"); return graph_->MaxNumIndices(); }

    //! Whether the graph has a column Map.
    bool hasColMap() const { XPETRA_MONITOR("EpetraCrsGraphT::hasColMap"); return graph_->HaveColMap(); }

    //! Whether the graph is locally lower triangular.
    bool isLowerTriangular() const { XPETRA_MONITOR("EpetraCrsGraphT::isLowerTriangular"); return graph_->LowerTriangular(); }

    //! Whether the graph is locally upper triangular.
    bool isUpperTriangular() const { XPETRA_MONITOR("EpetraCrsGraphT::isUpperTriangular"); return graph_->UpperTriangular(); }

    //! Whether column indices are stored using local indices on the calling process.
    bool isLocallyIndexed() const { XPETRA_MONITOR("EpetraCrsGraphT::isLocallyIndexed"); return graph_->IndicesAreLocal(); }

    //! Whether column indices are stored using global indices on the calling process.
    bool isGloballyIndexed() const { XPETRA_MONITOR("EpetraCrsGraphT::isGloballyIndexed"); return graph_->IndicesAreGlobal(); }

    //! Whether fillComplete() has been called and the graph is in compute mode.
    bool isFillComplete() const { XPETRA_MONITOR("EpetraCrsGraphT::isFillComplete"); return graph_->Filled(); }

    //! Returns true if storage has been optimized.
    bool isStorageOptimized() const { XPETRA_MONITOR("EpetraCrsGraphT::isStorageOptimized"); return graph_->StorageOptimized(); }

    //! Return a const, nonpersisting view of global indices in the given row.
    void getGlobalRowView(GlobalOrdinal GlobalRow, ArrayView< const GlobalOrdinal > &Indices) const;

    //! Return a const, nonpersisting view of local indices in the given row.
    void getLocalRowView(LocalOrdinal LocalRow, ArrayView< const LocalOrdinal > &indices) const;

    //@}

    //! @name Overridden from Teuchos::Describable
    //@{

    //! Return a simple one-line description of this object.
    std::string description() const;

    //! Print the object with some verbosity level to an FancyOStream object.
    void describe(Teuchos::FancyOStream &out, const Teuchos::EVerbosityLevel verbLevel=Teuchos::Describable::verbLevel_default) const;

    //@}

    //! Implements DistObject interface
    //{@

    //! Access function for the Tpetra::Map this DistObject was constructed with.
    Teuchos::RCP< const Map< LocalOrdinal, GlobalOrdinal, Node > > getMap() const { XPETRA_MONITOR("EpetraCrsGraphT::getMap"); return toXpetra<GlobalOrdinal>(graph_->Map()); }

    //! Import.
    void doImport(const DistObject<GlobalOrdinal, LocalOrdinal, GlobalOrdinal, Node> &source, const Import< LocalOrdinal, GlobalOrdinal, Node > &importer, CombineMode CM);

    //! Export.
    void doExport(const DistObject<GlobalOrdinal, LocalOrdinal, GlobalOrdinal, Node> &dest, const Import< LocalOrdinal, GlobalOrdinal, Node >& importer, CombineMode CM);

    //! Import (using an Exporter).
    void doImport(const DistObject<GlobalOrdinal, LocalOrdinal, GlobalOrdinal, Node> &source, const Export< LocalOrdinal, GlobalOrdinal, Node >& exporter, CombineMode CM);

    //! Export (using an Importer).
    void doExport(const DistObject<GlobalOrdinal, LocalOrdinal, GlobalOrdinal, Node> &dest, const Export< LocalOrdinal, GlobalOrdinal, Node >& exporter, CombineMode CM);

    //@}

    //! @name Xpetra specific
    //@{

    //! EpetraCrsGraphT constructor to wrap a Epetra_CrsGraph object
    EpetraCrsGraphT(const Teuchos::RCP<Epetra_CrsGraph> &graph) : graph_(graph) {
      TEUCHOS_TEST_FOR_EXCEPTION(!graph->RowMap().GlobalIndicesIsType<GlobalOrdinal>(), std::runtime_error, "Xpetra::EpetraCrsGraphT: GlobalOrdinal mismatch.");
    }

    //! Get the underlying Epetra graph
    RCP< const Epetra_CrsGraph> getEpetra_CrsGraph() const { return graph_; }

    //@}

  private:

    RCP<Epetra_CrsGraph> graph_;

  }; // EpetraCrsGraphT class

#ifndef XPETRA_EPETRA_NO_32BIT_GLOBAL_INDICES
  typedef EpetraCrsGraphT<int> EpetraCrsGraph;
#endif

#ifndef XPETRA_EPETRA_NO_64BIT_GLOBAL_INDICES
  typedef EpetraCrsGraphT<long long> EpetraCrsGraph64;
#endif

} // Xpetra namespace

#endif // XPETRA_EPETRACRSGRAPH_HPP
