#pragma once

#include "libmesh/elem.h"
#include "libmesh/enum_io_package.h"
#include "libmesh/exodusII_io.h"
#include "libmesh/nemesis_io.h"
#include "libmesh/node.h"
#include "libmesh/parallel_mesh.h"
#include "CubitElementInfo.h"
#include "NodeBiMap.h"
#include "mfem.hpp"

/**
 * MFEMMesh
 *
 * MFEMMesh wraps an mfem::Mesh object.
 */
class MFEMMesh : public mfem::Mesh
{
public:
  /**
   * Initializer for 1st order elements.
   */
  MFEMMesh(const int num_elements_in_mesh,
           const CubitElementInfo & element_info,
           const std::vector<int> & unique_block_ids,
           const std::vector<int> & unique_side_boundary_ids,
           const std::vector<int> & unique_libmesh_corner_node_ids,
           std::map<int, int> & num_elements_for_boundary_id,
           std::map<int, std::vector<int>> & libmesh_element_ids_for_block_id,
           std::map<int, std::vector<int>> & libmesh_node_ids_for_element_id,
           std::map<int, std::vector<int>> & libmesh_node_ids_for_boundary_id,
           std::map<int, std::array<double, 3>> & coordinates_for_libmesh_node_id);

  /**
   * Initializer for 2nd order elements.
   */
  MFEMMesh(const int num_elements_in_mesh,
           const CubitElementInfo & element_info,
           const std::vector<int> & unique_block_ids,
           const std::vector<int> & unique_side_boundary_ids,
           const std::vector<int> & unique_libmesh_corner_node_ids,
           std::map<int, int> & num_elements_for_boundary_id,
           std::map<int, std::vector<int>> & libmesh_element_ids_for_block_id,
           std::map<int, std::vector<int>> & libmesh_node_ids_for_element_id,
           std::map<int, std::vector<int>> & libmesh_node_ids_for_boundary_id,
           std::map<int, std::array<double, 3>> & coordinates_for_libmesh_node_id,
           NodeBiMap & second_order_node_bimap);

  MFEMMesh(std::string mesh_fname,
           int generate_edges = 0,
           int refine = 1,
           bool fix_orientation = true);

protected:
  /**
   * Calls buildMFEMVertices, buildMFEMElements, buildMFEMBoundaryElements methods
   * to construct the mesh. NB: - additional methods should be called after this
   * to handle second-order elements. The Finalize() method must be called at the
   * end.
   */
  void buildMFEMVerticesAndElements(
      const int num_elements_in_mesh,
      const CubitElementInfo & element_info,
      const std::vector<int> & unique_block_ids,
      const std::vector<int> & unique_side_boundary_ids,
      const std::vector<int> & unique_libmesh_corner_node_ids,
      std::map<int, int> & num_elements_for_boundary_id,
      std::map<int, std::vector<int>> & libmesh_element_ids_for_block_id,
      std::map<int, std::vector<int>> & libmesh_node_ids_for_element_id,
      std::map<int, std::vector<int>> & libmesh_node_ids_for_boundary_id,
      std::map<int, std::array<double, 3>> & coordinates_for_libmesh_node_id);

  /**
   * Sets the protected variable array using the provided vector of corner node
   * IDs from MOOSE. Note that the vertices (named "nodes" in MOOSE) are ONLY
   * at the corners of elements. These are referred to as "corner nodes" in MOOSE.
   */
  void buildMFEMVertices(const std::vector<int> & unique_libmesh_corner_node_ids,
                         std::map<int, std::array<double, 3>> & coordinates_for_libmesh_node_id);

  /**
   * Construct the MFEM elements array.
   */
  void buildMFEMElements(const int num_elements_in_mesh,
                         const CubitElementInfo & element_info,
                         const std::vector<int> & unique_block_ids,
                         std::map<int, std::vector<int>> & libmesh_element_ids_for_block_id,
                         std::map<int, std::vector<int>> & libmesh_node_ids_for_element_id);

  /**
   * Construct the boundary array of elements.
   */
  void
  buildMFEMBoundaryElements(const CubitElementInfo & element_info,
                            const std::vector<int> & unique_side_boundary_ids,
                            std::map<int, int> & num_elements_for_boundary_id,
                            std::map<int, std::vector<int>> & libmesh_node_ids_for_boundary_id);

  /**
   * Returns a pointer to an mfem::Element.
   */
  mfem::Element *
  buildMFEMElement(const int element_type, const int * vertex_ids, const int block_id);

  /**
   * Returns an pointer to an mfem::Element (for faces only).
   */
  mfem::Element *
  buildMFEMFaceElement(const int face_type, const int * vertex_ids, const int boundary_id);

  /**
   * Called internally in constructor if the element is second-order.
   */
  void
  handleQuadraticFESpace(const CubitElementInfo & element_info,
                         const std::vector<int> & unique_block_ids,
                         std::map<int, std::vector<int>> & libmesh_element_ids_for_block_id,
                         std::map<int, std::vector<int>> & libmesh_node_ids_for_element_id,
                         std::map<int, std::array<double, 3>> & coordinates_for_libmesh_node_id,
                         NodeBiMap & second_order_node_bimap);

  /**
   * Verifies whether the libmesh and mfem node ids have a unique mapping. All
   * coordinates should match and every mfem node id should have a corresponding
   * libmesh node id. Any left-over node ids will be detected.
   */
  void verifyUniqueMappingBetweenLibmeshAndMFEMNodes(
      mfem::FiniteElementSpace & finite_element_space,
      const std::vector<int> & unique_block_ids,
      std::map<int, std::vector<int>> & libmesh_element_ids_for_block_id,
      std::map<int, std::vector<int>> & libmesh_node_ids_for_element_id,
      std::map<int, std::array<double, 3>> & coordinates_for_libmesh_node_id,
      NodeBiMap & second_order_node_bimap);

  /**
   * Map from the MFEM element id to the libmesh element id internally.
   */
  std::map<int, int> _libmesh_element_id_for_mfem_element_id;

  /**
   * Maps from the libmesh corner node id to the mfem vertex id internally.
   */
  std::map<int, int> _mfem_vertex_index_for_libmesh_corner_node_id;
};
