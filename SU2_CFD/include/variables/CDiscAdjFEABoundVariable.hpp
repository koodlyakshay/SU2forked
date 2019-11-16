/*!
 * \file CDiscAdjFEABoundVariable.hpp
 * \brief Main class for defining the variables of the adjoint FEA solver at the boundary.
 * \author Ruben Sanchez
 * \version 6.2.0 "Falcon"
 *
 * The current SU2 release has been coordinated by the
 * SU2 International Developers Society <www.su2devsociety.org>
 * with selected contributions from the open-source community.
 *
 * The main research teams contributing to the current release are:
 *  - Prof. Juan J. Alonso's group at Stanford University.
 *  - Prof. Piero Colonna's group at Delft University of Technology.
 *  - Prof. Nicolas R. Gauger's group at Kaiserslautern University of Technology.
 *  - Prof. Alberto Guardone's group at Polytechnic University of Milan.
 *  - Prof. Rafael Palacios' group at Imperial College London.
 *  - Prof. Vincent Terrapon's group at the University of Liege.
 *  - Prof. Edwin van der Weide's group at the University of Twente.
 *  - Lab. of New Concepts in Aeronautics at Tech. Institute of Aeronautics.
 *
 * Copyright 2012-2019, Francisco D. Palacios, Thomas D. Economon,
 *                      Tim Albring, and the SU2 contributors.
 *
 * SU2 is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * SU2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with SU2. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "CDiscAdjFEAVariable.hpp"
#include "../../../Common/include/toolboxes/CVertexMap.hpp"

/*!
 * \class CDiscAdjFEABoundVariable
 * \brief Main class for defining the variables on the FEA boundaries for adjoint applications.
 * \ingroup Discrete_Adjoint
 * \author R. Sanchez.
 * \version 6.2.0 "Falcon"
 */
class CDiscAdjFEABoundVariable final : public CDiscAdjFEAVariable {
private:

  MatrixType FlowTraction_Sens;        /*!< \brief Adjoint of the flow tractions. */
  MatrixType SourceTerm_DispAdjoint;   /*!< \brief Source term applied into the displacement
                                                   adjoint coming from external solvers. */

  CVertexMap<unsigned> VertexMap;  /*!< \brief Object that controls accesses to the variables of this class. */

public:
  /*!
   * \overload
   * \param[in] disp - Pointer to the adjoint value (initialization value).
   * \param[in] vel - Pointer to the adjoint value (initialization value).
   * \param[in] accel - Pointer to the adjoint value (initialization value).
   * \param[in] npoint - Number of points/nodes/vertices in the domain.
   * \param[in] ndim - Number of dimensions of the problem.
   * \param[in] nvar - Number of variables of the problem.
   * \param[in] unsteady - Allocate velocity and acceleration.
   * \param[in] config - Definition of the particular problem.
   */
  CDiscAdjFEABoundVariable(const su2double *disp, const su2double *vel, const su2double *accel,
                           unsigned long npoint, unsigned long ndim, unsigned long nvar, bool unsteady, CConfig *config);

  /*!
   * \brief Destructor of the class.
   */
  ~CDiscAdjFEABoundVariable() = default;

  /*!
   * \brief Allocate member variables for points marked as vertex (via "Set_isVertex").
   * \param[in] config - Definition of the particular problem.
   */
  void AllocateBoundaryVariables(CConfig *config);

  /*!
   * \brief Set the FSI force sensitivity at the node
   * \param[in] iDim - spacial component
   * \param[in] val - value of the Sensitivity
   */
  inline void SetFlowTractionSensitivity(unsigned long iPoint, unsigned long iDim, su2double val) override {
    if (!VertexMap.GetVertexIndex(iPoint)) return;
    FlowTraction_Sens(iPoint,iDim) = val;
  }

  /*!
   * \brief Get the FSI force sensitivity at the node
   * \param[in] iDim - spacial component
   * \return value of the Sensitivity
   */
  inline su2double GetFlowTractionSensitivity(unsigned long iPoint, unsigned long iDim) const override {
    if (!VertexMap.GetVertexIndex(iPoint)) return 0.0;
    return FlowTraction_Sens(iPoint,iDim);
  }

  /*!
   * \brief Set the source term applied into the displacement adjoint coming from external solvers
   * \param[in] iDim - spacial component
   * \param[in] val - value of the source term
   */
  inline void SetSourceTerm_DispAdjoint(unsigned long iPoint, unsigned long iDim, su2double val) override {
    if (!VertexMap.GetVertexIndex(iPoint)) return;
    SourceTerm_DispAdjoint(iPoint,iDim) = val;
  }

  /*!
   * \brief Get the source term applied into the displacement adjoint coming from external solvers
   * \param[in] iDim - spacial component
   * \return value of the source term
   */
  inline su2double GetSourceTerm_DispAdjoint(unsigned long iPoint, unsigned long iDim) const override {
    if (!VertexMap.GetVertexIndex(iPoint)) return 0.0;
    return SourceTerm_DispAdjoint(iPoint,iDim);
  }

  /*!
   * \brief Get whether a node is on the boundary
   */
  inline bool Get_isVertex(unsigned long iPoint) const override {
    return VertexMap.GetIsVertex(iPoint);
  }

  /*!
   * \brief Set whether a node is on the boundary
   */
  inline void Set_isVertex(unsigned long iPoint, bool isVertex) override {
    VertexMap.SetIsVertex(iPoint,isVertex);
  }

};