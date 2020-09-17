/*
 * translate.h
 *
 *  Created on: 06.06.2016
 *      Author: stefan
 */

#ifndef TRANSLATE_H_
#define TRANSLATE_H_

#include <boost/locale.hpp>

#define _(STRING) boost::locale::translate(STRING)

#endif /* TRANSLATE_H_ */
