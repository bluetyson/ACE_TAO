// -*- C++ -*-

//=============================================================================
/**
 *  @file    FTRMFF_Worstfit.h
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef FTRMFF_WORSTFIT_ALGORITHM_H_
#define FTRMFF_WORSTFIT_ALGORITHM_H_

#include "Schedule.h"

class FTRMFF_Worstfit : public FTRMFF_Algorithm
{
public:
  virtual ~FTRMFF_Worstfit ();

  virtual FTRMFF_Output operator () (const FTRMFF_Input & input);
};

class FTRMFF_Worstfit_Algorithm : 
  public FTRMFF_Algorithm_Impl
{
public:
  FTRMFF_Worstfit_Algorithm (const PROCESSOR_LIST & processors,
                            unsigned int consistency_level);

  virtual ~FTRMFF_Worstfit_Algorithm ();

  virtual SCHEDULING_MAP operator () (const TASK_LIST & tasks);

  virtual const SCHEDULE & schedule () const;

private:
  SCHEDULE schedule_;
  SCHEDULE_PROGRESS_LIST unschedulable_;
};

#endif /* FTRMFF_WORSTFIT_ALGORITHM_H_ */
