/*
 * translate.h
 *
 *  Created on: 06.06.2016
 *      Author: stefan
 */

#ifndef TRANSLATE_H_
#define TRANSLATE_H_

//#include <boost/locale.hpp>
//#include <libintl.h>

//#define _(STRING) gettext(STRING)
#include <string>

#define _(STRING) std::string(STRING)

#endif /* TRANSLATE_H_ */
