//# MSTimeGram.cc: Grammar for time expressions
//# Copyright (C) 1998,1999,2001,2003
//# Associated Universities, Inc. Washington DC, USA.
//#
//# This library is free software; you can redistribute it and/or modify it
//# under the terms of the GNU Library General Public License as published by
//# the Free Software Foundation; either version 2 of the License, or (at your
//# option) any later version.
//#
//# This library is distributed in the hope that it will be useful, but WITHOUT
//# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
//# License for more details.
//#
//# You should have received a copy of the GNU Library General Public License
//# along with this library; if not, write to the Free Software Foundation,
//# Inc., 675 Massachusetts Ave, Cambridge, MA 02139, USA.
//#
//# Correspondence concerning AIPS++ should be addressed as follows:
//#        Internet email: aips2-request@nrao.edu.
//#        Postal address: AIPS++ Project Office
//#                        National Radio Astronomy Observatory
//#                        520 Edgemont Road
//#                        Charlottesville, VA 22903-2475 USA
//#
//# $Id$

// MSTimeGram; grammar for time command lines

// This file includes the output files of bison and flex for
// parsing command lines operating on lattices.

#include <casacore/tables/TaQL/ExprNode.h>
#include <casacore/tables/TaQL/ExprNodeSet.h>
#include <casacore/ms/MeasurementSets/MeasurementSet.h>
#include <casacore/ms/MSSel/MSTimeGram.h>
#include <casacore/ms/MSSel/MSTimeParse.h> // routines used by bison actions
#include <casacore/tables/TaQL/TableParse.h>       // routines used by bison actions
#include <casacore/casa/Arrays/Matrix.h>
#include <casacore/ms/MSSel/MSSelectableMainColumn.h>
#include <casacore/ms/MSSel/MSSelectionError.h>

#include <casa/Quanta/MVTime.h>
#include <casa/Quanta/MVFrequency.h>
#include <casa/Quanta/Quantum.h>
//# stdlib.h is needed for bison 1.28 and needs to be included here
//# (before the flex/bison files).
#include <casacore/casa/stdlib.h>
//# Define register as empty string to avoid warnings in C++11 compilers
//# because keyword register is not supported anymore.
#define register
#include "MSTimeGram.ycc"                  // bison output
#define yy_scan_chars yy_scan_chars_MSTimeGram
#include "MSTimeGram.lcc"                  // flex output

// Define the yywrap function for flex.
int MSTimeGramwrap()
{
  return 1;
}

namespace casacore { //# NAMESPACE CASACORE - BEGIN
  
  //# Declare a file global pointer to a char* for the input string.
  static const char*           strpMSTimeGram = 0;
  static Int                   posMSTimeGram = 0;
  extern MSTimeParse *thisMSTParser;
  
  //# Parse the command.
  //# Do a yyrestart(yyin) first to make the flex scanner reentrant.
  //----------------------------------------------------------------------------

  int baseMSTimeGramParseCommand (MSTimeParse* parser, const String& command, 
				  Matrix<Double>& selectedTimeList)
  {
    Int ret;
    try
      {
	MSTimeGramrestart (MSTimeGramin);
	yy_start = 1;
	strpMSTimeGram = command.chars();     // get pointer to command string
	posMSTimeGram  = 0;                   // initialize string position
	parser->reset();  		      // global pointer to it
	MSTimeParse::thisMSTParser = parser;
	ret=MSTimeGramparse();                // parse command string
	selectedTimeList = parser->selectedTimes();
      } 
    catch (MSSelectionTimeError &x)
      {
	String newMesgs;
	newMesgs = constructMessage(msTimeGramPosition()+1, command);
	x.addMessage(newMesgs);
	throw;
      }
    return ret;
    
  }

  int msTimeGramParseCommand (const MeasurementSet *ms, const String& command, 
			      const TableExprNode& colAsTEN,
			      MSSelectableMainColumn& msMainColInterface,
			      const TableExprNode& otherTens,
			      Matrix<Double>& selectedTimeList)
  {
    MSTimeParse *thisParser = new MSTimeParse(ms,colAsTEN,msMainColInterface, otherTens);
    int ret;
    try
      {
       ret = baseMSTimeGramParseCommand(thisParser, command, selectedTimeList);
      }
    catch (MSSelectionTimeError &x)
      {
	delete thisParser;
	throw;
      }
    delete thisParser;
    return ret;
  }

  int msTimeGramParseCommand (const MeasurementSet* ms, const String& command, 
			      const TableExprNode& otherTens,
			      Matrix<Double>& selectedTimeList)
  {
    MSTimeParse *thisParser = new MSTimeParse(ms,otherTens);
    int ret;
    try
      {
       ret = baseMSTimeGramParseCommand(thisParser, command, selectedTimeList);
      }
    catch (MSSelectionTimeError &x)
      {
	delete thisParser;
	throw;
      }
    delete thisParser;
    return ret;
    // Int ret;
    // try
    //   {
    // 	MSTimeGramrestart (MSTimeGramin);
    // 	yy_start = 1;
    // 	strpMSTimeGram = command.chars();     // get pointer to command string
    // 	posMSTimeGram  = 0;                   // initialize string position
    // 	MSTimeParse parser(ms,otherTens);     // setup the parser and
    // 	parser.reset();  		      // global pointer to it
    // 	MSTimeParse::thisMSTParser = &parser;
    // 	ret=MSTimeGramparse();                // parse command string
    // 	selectedTimeList = parser.selectedTimes();
    //   } 
    // catch (MSSelectionTimeError &x)
    //   {
    // 	String newMesgs;
    // 	newMesgs = constructMessage(msTimeGramPosition()+1, command);
    // 	x.addMessage(newMesgs);
    // 	throw;
    //   }
    // return ret;
  }
  int msTimeGramParseCommand (const MeasurementSet* ms, const String& command, const TableExprNode& otherTens) 
  {
    Matrix<Double> timeList;
    Int ret;
    try
      {
	MSTimeGramrestart (MSTimeGramin);
	yy_start = 1;
	strpMSTimeGram = command.chars();     // get pointer to command string
	posMSTimeGram  = 0;                   // initialize string position
	MSTimeParse parser(ms,otherTens);     // setup the parser and
					      // global pointer to it
	MSTimeParse::thisMSTParser = &parser;
	ret=MSTimeGramparse();                // parse command string
      } 
    catch (MSSelectionTimeError &x)
      {
	String newMesgs;
	newMesgs = constructMessage(msTimeGramPosition()+1, command);
	x.addMessage(newMesgs);
	throw;
      }
    return ret;
  }
  
  //# Give the table expression node
  //----------------------------------------------------------------------------
  const TableExprNode* msTimeGramParseNode()
  {
    return MSTimeParse::node();
  }
  void msTimeGramParseDeleteNode()
  {
    return MSTimeParse::cleanup();
  }
  
  //# Give the string position.
  //----------------------------------------------------------------------------
  Int& msTimeGramPosition()
  {
    return posMSTimeGram;
  }
  
  //# Get the next input characters for flex.
  //----------------------------------------------------------------------------
  int msTimeGramInput (char* buf, int max_size)
  {
    int nr=0;
    while (*strpMSTimeGram != 0) {
      if (nr >= max_size) {
	break;                         // get max. max_size char.
      }
      buf[nr++] = *strpMSTimeGram++;
    }
    return nr;
  }
  
  //----------------------------------------------------------------------------
  void MSTimeGramerror (const char*)
  {
    throw(MSSelectionTimeParseError("MSSelection time error: Parse error at or near token '" +
			       String(MSTimeGramtext) + "'"));
  }
  
  String msTimeGramRemoveEscapes (const String& in)
  {
    String out;
    int leng = in.length();
    for (int i=0; i<leng; i++) {
      if (in[i] == '\\') {
	i++;
      }
      out += in[i];
    }
    return out;
  }
  
  //----------------------------------------------------------------------------
  void msTimeGramSetTimeFields (struct TimeFields& tf, 
				Int year, Int month, Int day,
				Int hour, Int minute, Int sec, Int fsec)
  {
    tf.year = year;
    tf.month = month;
    tf.day = day;
    tf.hour = hour;
    tf.minute = minute;
    tf.sec = sec;
    tf.fsec = fsec;
  }

  //----------------------------------------------------------------------------
  void msTimeGramParseTimeFormats(struct TimeFields& tf,
				  const char* timeStr)
  {
    Quantum<Double> qt;
    if (MVTime::read(qt, timeStr))
      {
	MVTime tt(qt);
	cerr << qt << " " << tt.getTime().year() << " " << tt.monthday() << " " << tt.month() << " " << tt.getTime().hours() << " " << tt.getTime().minutes() << " " << " " << tt.getTime().dseconds() << endl;
      }
    else
      throw(MSSelectionTimeParseError("Time format not allowed: '" + String(timeStr) + "'"));
    // std::tm t = {};
    // std::istringstream ss("6-Mar-2007/17:00:5");

    // sscanf(timeStr,"%d-%s-%d %d:%d:%f",&t.tm_year, &t.tm_mon, &t.tm_mday, &t.tm_hour, &t.tm_min, &ss);
    // std::cout << t.tm_sec << " " << t.tm_min << " " << t.tm_hour << '\n';

    //    ss.imbue(std::locale("de_DE.utf-8"));
    // ss >> std::get_time(&t, "%Y-%b-%d %H:%M:%S");
    // if (ss.fail()) {
    //     std::cout << "Parse failed\n";
    // } else {
    //   std::cout << t.tm_sec << " " << t.tm_min << " " << t.tm_hour << '\n';
    // }
  }
  
} //# NAMESPACE CASACORE - END
