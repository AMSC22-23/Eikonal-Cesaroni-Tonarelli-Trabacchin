/*
 * Eikonal_traits.hpp
 *
 *  Created on: May 31, 2022
 *      Author: forma
 */

#ifndef EIKONAL_EIKONAL_TRAITS_HPP_
#define EIKONAL_EIKONAL_TRAITS_HPP_
#include <vector>
#include<array>
#include "Eigen/Core"
#include "LineSearch_traits.hpp"
//PHDIM phisical dimension of the domain
namespace Eikonal
{

  template <std::size_t PHDIM>
  struct Eikonal_traits
  {
	  using Point=Eigen::Matrix<double,PHDIM,1>;
	  using Edge=Eigen::Matrix<double,PHDIM,1>;
	  using AnisotropyM=Eigen::Matrix<double,PHDIM,PHDIM>;
	  using MMatrix=Eigen::Matrix<double,PHDIM,PHDIM>;
	  using Vector=Eigen::Matrix<double,PHDIM-1,1>;
	  using VectorExt=Eigen::Matrix<double,PHDIM,1>;
  };
}



#endif /* EIKONAL_EIKONAL_TRAITS_HPP_ */
