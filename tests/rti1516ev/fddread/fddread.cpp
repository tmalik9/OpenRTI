/* -*-c++-*- OpenRTI - Copyright (C) 2009-2012 Mathias Froehlich
 *
 * This file is part of OpenRTI.
 *
 * OpenRTI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * OpenRTI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with OpenRTI.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <cstdlib>
#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <fstream>
#include "RTI/RTIambassadorFactory.h"
#include "RTI/RTIambassador.h"

#include "Options.h"
#include "StringUtils.h"
#include "FDD1516EFileReader.h"


int
main(int argc, char* argv[])
{
  OpenRTI::FDD1516EFileReader reader;
  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "could not open file " << argv[1] << std::endl;
    return EXIT_FAILURE;
  }
  reader.read(file, std::string());
  return EXIT_SUCCESS;
}
