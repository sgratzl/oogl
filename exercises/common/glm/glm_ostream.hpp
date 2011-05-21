/*
 * glm_ostream.hpp
 *
 *  Created on: 13.08.2010
 *      Author: sam
 */

#ifndef GLM_OSTREAM_HPP_
#define GLM_OSTREAM_HPP_

#include <glm/core/type.hpp>
#include <glm/gtx/matrix_selection.hpp>
#include <ostream>
#include <iomanip>

#define GLM_FORMATTING std::setw(5) << std::setprecision(3) << std::fixed << std::setfill('0')

template<typename U>
std::ostream &operator<<(std::ostream &ostr, const glm::detail::tvec1<U> &v) {
	return ostr << GLM_FORMATTING << "(" << v.x << ")";
}

template<typename U>
std::ostream &operator<<(std::ostream &ostr, const glm::detail::tvec2<U> &v) {
	return ostr << GLM_FORMATTING << "(" << v.x << "," << v.y << ")";
}

template<typename U>
std::ostream &operator<<(std::ostream &ostr, const glm::detail::tvec3<U> &v) {
	return ostr << GLM_FORMATTING << "(" << v.x << "," << v.y << "," << v.z << ")";
}

template<typename U>
std::ostream &operator<<(std::ostream &ostr, const glm::detail::tvec4<U> &v) {
	return ostr << GLM_FORMATTING << "(" << v.x << "," << v.y << "," << v.z << "," << v.w << ")";
}

template<typename T>
std::ostream &operator<<(std::ostream &ostr, const glm::detail::tmat2x2<T> &v) {
	namespace glmm = glm::gtx::matrix_selection;

	ostr << std::endl;

	glm::detail::tvec2<T> row = glmm::row(v, 0);
	ostr << "[" << GLM_FORMATTING << row.x << "," << row.y << " " << std::endl;
	row = glmm::row(v, 1);
	ostr << " " << GLM_FORMATTING << row.x << "," << row.y << "]" << std::endl;
	return ostr;
}

template<typename T>
std::ostream &operator<<(std::ostream &ostr, const glm::detail::tmat3x3<T> &v) {
	namespace glmm = glm::gtx::matrix_selection;

	ostr << std::endl;

	glm::detail::tvec3<T> row = glmm::row(v, 0);
	ostr << "[" << GLM_FORMATTING << row.x << "," << row.y << "," << row.z << " " << std::endl;
	row = glmm::row(v, 1);
	ostr << " " << GLM_FORMATTING << row.x << "," << row.y << "," << row.z << " " << std::endl;
	row = glmm::row(v, 2);
	ostr << " " << GLM_FORMATTING << row.x << "," << row.y << "," << row.z << "]" << std::endl;
	return ostr;
}

template<typename T>
std::ostream &operator<<(std::ostream &ostr, const glm::detail::tmat4x4<T> &v) {
	namespace glmm = glm::gtx::matrix_selection;

	ostr << std::endl;

	glm::detail::tvec4<T> row = glmm::row(v, 0);
	ostr << "[" << GLM_FORMATTING << row.x << "," << row.y << "," << row.z << "," << row.w << " " << std::endl;
	row = glmm::row(v, 1);
	ostr << " " << GLM_FORMATTING << row.x << "," << row.y << "," << row.z << "," << row.w << " " << std::endl;
	row = glmm::row(v, 2);
	ostr << " " << GLM_FORMATTING << row.x << "," << row.y << "," << row.z << "," << row.w << "]" << std::endl;
	row = glmm::row(v, 3);
	ostr << " " << GLM_FORMATTING << row.x << "," << row.y << "," << row.z << "," << row.w << "]" << std::endl;
	return ostr;
}

#undef GLM_FORMATTING

#endif /* GLM_OSTREAM_HPP_ */
