/*
 * glm_ostream.hpp
 *
 *  Created on: 13.08.2010
 *      Author: sam
 */

#ifndef GLM_OSTREAM_HPP_
#define GLM_OSTREAM_HPP_

#include <glm/core/type.hpp>
#include <ostream>

template<typename U>
std::ostream &operator<<(std::ostream &ostr, const glm::detail::tvec1<U> &v) {
	return ostr << "(" << v.x << ")";
}

template<typename U>
std::ostream &operator<<(std::ostream &ostr, const glm::detail::tvec2<U> &v) {
	return ostr << "(" << v.x << "," << v.y << ")";
}

template<typename U>
std::ostream &operator<<(std::ostream &ostr, const glm::detail::tvec3<U> &v) {
	return ostr << "(" << v.x << "," << v.y << "," << v.z << ")";
}

template<typename U>
std::ostream &operator<<(std::ostream &ostr, const glm::detail::tvec4<U> &v) {
	return ostr << "(" << v.x << "," << v.y << "," << v.z << "," << v.w <<")";
}


#endif /* GLM_OSTREAM_HPP_ */
