/* ***** BEGIN LICENSE BLOCK *****
 * Roadsend PHP Compiler Runtime Libraries
 *
 * Copyright (c) 2009 Shannon Weyrick <weyrick@roadsend.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 * ***** END LICENSE BLOCK ***** */

#ifndef RPHP_PCONFIG_H_
#define RPHP_PCONFIG_H_

#include <boost/unordered_map.hpp>
#include <string>

namespace rphp {

typedef std::string configStringType;
typedef boost::unordered_map<configStringType, configStringType> configRegistryType;

/**
 * a simple configuration object which maintains key/value pairs
 */
class pConfig {
private:
    configRegistryType registry_;

public:
    void set(configStringType key, configStringType value) {
        registry_[key] = value;
    }

    configStringType get(configStringType key) {
        return registry_[key];
    }

};

} /* namespace rphp */


#endif /* RPHP_PCONFIG_H_ */
