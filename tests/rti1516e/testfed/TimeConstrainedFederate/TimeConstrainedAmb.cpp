/**
 * @file
 *
 * Copyright (C) 2013 Max Oberberger <max@oberbergers.de>
 *
 * Last modified: 2013 February 23, 20:05:02 by max
 *
 * This file is part of ba-hla.
 *
 * ba-hla is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ba-hla is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ba-hla.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include "TimeConstrainedAmb.h"
#include <string.h>

#include "RTI/time/HLAfloat64Time.h"

//using namespace std;
using std::wcout;
using std::endl;
using namespace rti1516e;

/**
 * NOTE: The main part of the functions in this file is to print something on stdout
 * to see whether the function is called or not.
 */
TimeConstrainedFedAmb::TimeConstrainedFedAmb(rti1516e::RTIambassador* rtiamb) : SimpleTestAmbassador(rtiamb)
{
  // initialize all the variable values
  this->rtiamb = rtiamb;
  this->federateTime      = 0.0;
  this->federateLookahead = 1.0;
  this->isRegulating  = false;
  this->isConstrained = false;
  this->isAdvancing   = false;
  this->isAnnouncedReadyToRun   = false;
  this->isAnnouncedAllDone   = false;
  this->isReadyToRun  = false;
  this->allDone  = false;
}

TimeConstrainedFedAmb::~TimeConstrainedFedAmb() throw()
{
}

