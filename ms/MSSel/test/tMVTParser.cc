//# Copyright (C) 1995,1996,1997,1999,2001,2002,2005
//# Associated Universities, Inc. Washington DC, USA.
//#
//# This program is free software; you can redistribute it and/or modify it
//# under the terms of the GNU General Public License as published by the Free
//# Software Foundation; either version 2 of the License, or (at your option)
//# any later version.
//#
//# This program is distributed in the hope that it will be useful, but WITHOUT
//# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
//# more details.
//#
//# You should have received a copy of the GNU General Public License along
//# with this program; if not, write to the Free Software Foundation, Inc.,
//# 675 Massachusetts Ave, Cambridge, MA 02139, USA.
//#
//# Correspondence concerning AIPS++ should be addressed as follows:
//#        Internet email: aips2-request@nrao.edu.
//#        Postal address: AIPS++ Project Office
//#                        National Radio Astronomy Observatory
//#                        520 Edgemont Road
//#                        Charlottesville, VA 22903-2475 USA
//#
//# $Id$

#include <casa/aips.h>

#include <casa/Quanta/MVTime.h>
#include <casa/Quanta/MVFrequency.h>
#include <casa/Quanta/Quantum.h>

using namespace std;
using namespace casacore;

int main(int argc, char **argv)
{
  String timeStr;
  if (argc > 1) timeStr=String(argv[1]);
  Quantum<Double> qt;
  if (MVTime::read(qt, timeStr))
    {
      MVTime tt(qt);
      cerr << "YYYY=" << tt.getTime().year() << " " << "DD=" << tt.monthday() << " "
	   << "MM=" << tt.month() << " "
	   << "HH=" << tt.getTime().hours() << " "
	   << "MM=" << tt.getTime().minutes() << " " << " "
	   << "SS.ss=" << tt.getTime().dseconds() << " "
	   << "SS=" << tt.getTime().seconds()
	   << endl;
      cerr << "Quantum value = " << qt << endl;
    }
  else
    cerr << "Time format not allowed: '" << timeStr << "'"<<endl;
}
