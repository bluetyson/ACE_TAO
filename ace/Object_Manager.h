/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Object_Manager.h
//
// = AUTHOR
//    David L. Levine
//
// ============================================================================

#if !defined (ACE_OBJECT_MANAGER_H)
#define ACE_OBJECT_MANAGER_H

#include "ace/Containers.h"

class ACE_Object_Manager
  // = TITLE
  //     Manager for static/singleton objects in the ACE library.
  //
  // = DESCRIPTION
  // This class deletes singleton objects in the ACE library at program
  // termination.  It does that by creating a static instance, whose
  // destructor gets called along with those of all other static objects.
  // Hooks are provided for application code to register objects and arrays
  // for destruction.
  // Please note that the order of such destructor calls is not specified.
  // Therefore, these destructors should not depend on any of the static
  // instances.  Also note that ACE_Log_Msg currently takes care of its
  // own cleanup.
{
public:
  static ACE_Object_Manager *instance ();
  // Accessor to singleton instance.

  void delete_at_exit (void *);
  // Register an object for deletion at program termination.

  void delete_array_at_exit (void *);
  // Register an array for deletion at program termination.

private:
  static ACE_Object_Manager *instance_;

  ACE_Unbounded_Queue<void *> registered_objects_;
  ACE_Unbounded_Queue<void *> registered_arrays_;

  ACE_Object_Manager ();
  ~ACE_Object_Manager ();

  friend class ACE_Object_Manager_Destroyer;
};

#endif /* ACE_OBJECT_MANAGER_H */
