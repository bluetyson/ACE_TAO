/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    IOStream_T.h
//
// = AUTHOR
//    James CE Johnson <jcej@lads.com>
//
// = COAUTHOR
//    Jim Crossley <jim@lads.com>
//
// = NOTE
//    This file should not be #included directly by application code.
//    Instead, it should #include "ace/IOStream.h".  That's because
//    we only put some conditional compilations in that file.
//
// ============================================================================

#if !defined (ACE_IOSTREAM_T_H)
#define ACE_IOSTREAM_T_H

#include "ace/IOStream.h"
#include <iomanip.h>

///////////////////////////////////////////////////////////////////////////

template <class STREAM>
class ACE_Streambuf_T : public ACE_Streambuf
{
public:
  ACE_Streambuf_T (STREAM * peer, u_int streambuf_size = ACE_STREAMBUF_SIZE, int io_mode = ios::in | ios::out);
  // We will be given a STREAM by the iostream object which creates
  // us.  See the ACE_IOStream_T template for how that works.  Like
  // other streambuf objects, we can be input-only, output-only or
  // both.

  virtual ssize_t send  ( char * buf, ssize_t len )
  {
	return peer_->send_n(buf,len);
  }
  virtual ssize_t recv  ( char * buf, ssize_t len, ACE_Time_Value * tv = NULL )
  {
	return this->recv( buf, len, 0, tv );
  }
  virtual ssize_t recv  ( char * buf, ssize_t len, int flags, ACE_Time_Value * tv = NULL )
  {
	ssize_t rval = peer_->recv(buf,len,flags,tv);
	return rval;
  }
  virtual ssize_t recv_n( char * buf, ssize_t len, int flags = 0, ACE_Time_Value * tv = NULL )
  {
	ssize_t rval =  peer_->recv_n(buf,len,flags,tv);
	return rval;
  }

protected:
  STREAM * peer_;
  // This will be our ACE_SOCK_Stream or similar object.

  virtual ACE_HANDLE get_handle(void)
  {
  	return peer_ ? peer_->get_handle() : 0 ;
  }
};

///////////////////////////////////////////////////////////////////////////

template <class STREAM>
class ACE_IOStream_T : public iostream, public STREAM
  // = TITLE
  //     A template adapter for creating an iostream-like object using
  //     an ACE IPC Stream for the actual I/O.  Iostreams use an
  //     underlying streambuf object for the IO interface.  The
  //     iostream class and derivatives provide you with a host of
  //     convenient operators that access the streambuf.
  //
  // = DESCRIPTION
  //     We inherit all characteristics of iostream and your <STREAM>
  //     class.  When you create a new class from this template, you
  //     can use it anywhere you would have used your original
  //     <STREAM> class.
  //
  //     To create an iostream for your favorite ACE IPC class (e.g.,
  //     <ACE_SOCK_Stream>), feed that class to this template's
  //     <STREAM> parameter, e.g.,
  //
  //     typedef ACE_Svc_Handler<ACE_SOCK_iostream,
  //                             ACE_INET_Addr, ACE_NULL_SYNCH>
  //             Service_Handler;
  //
  //     Because the operators in the iostream class are not virtual,
  //     you cannot easily provide overloads in your custom
  //     ACE_IOStream_T classes.  To make these things work correctly,
  //     you need to overload ALL operators of the ACE_IOStream_T you
  //     create. I've attempted to do that here to make things easier
  //     for you but there are no guarantees.
  //
  //     In the iostream.cpp file is an example of why it is necessary
  //     to overload all of the get/put operators when you want to
  //     customize only one or two.
{
public:
  ACE_IOStream_T (STREAM & stream, u_int streambuf_size = ACE_STREAMBUF_SIZE);
  ACE_IOStream_T (u_int streambuf_size = ACE_STREAMBUF_SIZE);
  // The default constructor.  This will initiailze your STREAM and
  // then setup the iostream baseclass to use a custom streambuf based
  // on STREAM.

  virtual ~ACE_IOStream_T (void);
  // We have to get rid of the streambuf_ ourselves since we gave it
  // to iostream();

  virtual int close (void);
  // The only ambituity in the multiple inheritance is the close()
  // function.

#if defined (ACE_HAS_STRING_CLASS)
  virtual ACE_IOStream_T<STREAM> & operator>>(ACE_IOStream_String & v);
  // A simple string operator.  The base iostream has 'em for char*
  // but that isn't always the best thing for a String.  If we don't
  // provide our own here, we may not get what we want.

  virtual ACE_IOStream_T<STREAM> & operator<<(ACE_IOStream_String & v);
  // The converse of the String put operator.

  virtual ACE_IOStream_T<STREAM> & operator>>(QuotedString &v);
  // A more clever operator that handles quoted strings so that we
  // can get strings containing whitespace!

  virtual ACE_IOStream_T<STREAM> & operator<<(QuotedString &v);
  // The converse of the QuotedString put operator.
#endif /* ACE_HAS_STRING_CLASS */

  // = Using the macros to provide get/set operators.
  GETPUT_FUNC_SET (ACE_IOStream_T<STREAM>)

#if defined (ACE_LACKS_IOSTREAM_FX)
  virtual int ipfx (int need = 0) {  return good(); }
  virtual int ipfx0(void)         {  return good(); }  // Optimized ipfx(0)
  virtual int ipfx1(void)         {  return good(); }  // Optimized ipfx(1)
  virtual void isfx (void)        {  return; }
  virtual int opfx (void)         {  return good(); }
  virtual void osfx (void)        {  put(' '); return; }
#else
#if defined (__GNUC__)
  virtual int ipfx0(void)         { return(iostream::ipfx0()); }  // Optimized ipfx(0)
  virtual int ipfx1(void)         { return(iostream::ipfx1()); }  // Optimized ipfx(1)
#else
  virtual int ipfx0(void)         {  return(iostream::ipfx(0)); }
  virtual int ipfx1(void)         {  return(iostream::ipfx(1)); }
#endif
  virtual int ipfx (int need = 0) {  return(iostream::ipfx(need)); }
  virtual void isfx (void)        {  iostream::isfx(); return; }
  virtual int opfx (void)         {  return(iostream::opfx()); }
  virtual void osfx (void)        {  iostream::osfx(); return; }
#endif /* ACE_LACKS_IOSTREAM_FX */

  ACE_IOStream_T<STREAM> & operator>>(ACE_Time_Value *&tv);
  // Allow the programmer to provide a timeout for read operations.
  // Give it a pointer to NULL to block forever.

protected:

  ACE_Streambuf_T<STREAM> *streambuf_;
  // This is where all of the action takes place.  The
  // streambuf_ is the interface to the underlying STREAM.

private:
  // We move these into the private section so that they cannot
  // be used by the application programmer.  This is necessary
  // because streambuf_ will be buffering IO on the STREAM
  // object.  If these functions were used in your program,
  // there is a danger of getting the datastream out of sync.
  ssize_t send (...);
  ssize_t recv (...);
  ssize_t send_n (...);
  ssize_t recv_n (...);
};

///////////////////////////////////////////////////////////////////////////

template <class STREAM>
class ACE_SOCK_Dgram_SC : public STREAM
// Datagrams don't have the notion of a "peer".  Each send and receive
// on a datagram can go to a different peer if you want.  If you're
// using datagrams for stream activity, you probably want 'em all to
// go to (and come from) the same place.  That's what this class is
// for.  BTW: 'Dgram_SC' is short for 'Datagram-Self-Contained'.
// Here, we keep an address object so that we can remember who last
// sent us data.  When we write back, we're then able to write back to
// that same address.
{
protected:
  ACE_INET_Addr peer_;

public:
  ACE_SOCK_Dgram_SC (void)
    {
    }

  ACE_SOCK_Dgram_SC (STREAM &source, ACE_INET_Addr &dest)
    : STREAM (source), peer_ (dest)
    {
    }

  inline ssize_t send_n (char *buf, ssize_t len)
    {
      return STREAM::send (buf, len, peer_);
    }

  inline ssize_t recv  (char * buf, ssize_t len, ACE_Time_Value * tv = NULL)
    {
      return recv (buf, len, 0, tv);
    }

  inline ssize_t recv (char * buf, ssize_t len, int flags, ACE_Time_Value * tv = NULL)
    {
      if (tv != 0)
	{
	  ACE_HANDLE handle = this->get_handle ();
	  ACE_Handle_Set handle_set;
		
	  handle_set.set_bit (handle);
		
	  switch (ACE_OS::select (int (handle) + 1,
				  (fd_set *) handle_set, // read_fds.
				  (fd_set *) 0,          // write_fds.
				  (fd_set *) 0,          // exception_fds.
				  tv))
	    {
	    case 0:
	      errno = ETIME;
	    case -1:
	      return -1;
	    default:
	      ;	// Do the 'recv' below
	    }
	}

      int rval = STREAM::recv (buf, len, peer_, flags);
#ifdef WIN32
      if (rval == SOCKET_ERROR)
	if (WSAGetLastError() == WSAEMSGSIZE)
	  if (flags & MSG_PEEK)
	    rval = len;
#endif
	
      return rval < len ? rval : len;
    }
  inline ssize_t recv_n (char * buf,
			 ssize_t len, int flags = 0,
			 ACE_Time_Value * tv = NULL)
    {
      int rval = this->recv (buf, len, flags, tv);
      return rval;
    }

  inline int get_remote_addr (ACE_INET_Addr &addr) const
    {
      addr = peer_;
      return 0;
    }

};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/IOStream_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("IOStream_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_IOSTREAM_T_H */
