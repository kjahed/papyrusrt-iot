// -*- C++ -*-
// $Id$

/**
 * Code generated by the The ACE ORB (TAO) IDL Compiler v2.2a_p14
 * TAO and the TAO IDL Compiler have been developed by:
 *       Center for Distributed Object Computing
 *       Washington University
 *       St. Louis, MO
 *       USA
 *       http://www.cs.wustl.edu/~schmidt/doc-center.html
 * and
 *       Distributed Object Computing Laboratory
 *       University of California at Irvine
 *       Irvine, CA
 *       USA
 * and
 *       Institute for Software Integrated Systems
 *       Vanderbilt University
 *       Nashville, TN
 *       USA
 *       http://www.isis.vanderbilt.edu/
 *
 * Information about TAO is available at:
 *     http://www.cs.wustl.edu/~schmidt/TAO.html
 **/

// TAO_IDL - Generated from
// be/be_codegen.cpp:376


#include "RawTypeSupportC.h"
#include "tao/AnyTypeCode/Null_RefCount_Policy.h"
#include "tao/AnyTypeCode/TypeCode_Constants.h"
#include "tao/AnyTypeCode/Alias_TypeCode_Static.h"
#include "tao/AnyTypeCode/Objref_TypeCode_Static.h"
#include "tao/AnyTypeCode/Sequence_TypeCode_Static.h"
#include "tao/CDR.h"
#include "tao/CDR.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/AnyTypeCode/Any_Impl_T.h"
#include "tao/AnyTypeCode/Any_Dual_Impl_T.h"
#include "ace/OS_NS_string.h"

#if !defined (__ACE_INLINE__)
#include "RawTypeSupportC.inl"
#endif /* !defined INLINE */

// TAO_IDL - Generated from
// be/be_visitor_typecode/alias_typecode.cpp:51



// TAO_IDL - Generated from
// be/be_visitor_typecode/typecode_defn.cpp:464


#ifndef _TAO_TYPECODE_PapyrusrtDDS_RawBufferSeq_GUARD
#define _TAO_TYPECODE_PapyrusrtDDS_RawBufferSeq_GUARD

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace TypeCode
  {
    namespace
    {
      TAO::TypeCode::Sequence< ::CORBA::TypeCode_ptr const *,
                              TAO::Null_RefCount_Policy>
        PapyrusrtDDS_RawBufferSeq_0 (
          ::CORBA::tk_sequence,
          &PapyrusrtDDS::_tc_RawBuffer,
          0U);
        
      ::CORBA::TypeCode_ptr const tc_PapyrusrtDDS_RawBufferSeq_0 =
        &PapyrusrtDDS_RawBufferSeq_0;
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* _TAO_TYPECODE_PapyrusrtDDS_RawBufferSeq_GUARD */

static TAO::TypeCode::Alias<char const *,
                            ::CORBA::TypeCode_ptr const *,
                            TAO::Null_RefCount_Policy>
  _tao_tc_PapyrusrtDDS_RawBufferSeq (
    ::CORBA::tk_alias,
    "IDL:PapyrusrtDDS/RawBufferSeq:1.0",
    "RawBufferSeq",
    &TAO::TypeCode::tc_PapyrusrtDDS_RawBufferSeq_0);
  

namespace PapyrusrtDDS
{
  ::CORBA::TypeCode_ptr const _tc_RawBufferSeq =
    &_tao_tc_PapyrusrtDDS_RawBufferSeq;
}

// TAO_IDL - Generated from
// be/be_visitor_interface/interface_cs.cpp:51

// Traits specializations for PapyrusrtDDS::RawBufferTypeSupport.

PapyrusrtDDS::RawBufferTypeSupport_ptr
TAO::Objref_Traits<PapyrusrtDDS::RawBufferTypeSupport>::duplicate (
    PapyrusrtDDS::RawBufferTypeSupport_ptr p)
{
  return PapyrusrtDDS::RawBufferTypeSupport::_duplicate (p);
}

void
TAO::Objref_Traits<PapyrusrtDDS::RawBufferTypeSupport>::release (
    PapyrusrtDDS::RawBufferTypeSupport_ptr p)
{
  ::CORBA::release (p);
}

PapyrusrtDDS::RawBufferTypeSupport_ptr
TAO::Objref_Traits<PapyrusrtDDS::RawBufferTypeSupport>::nil (void)
{
  return PapyrusrtDDS::RawBufferTypeSupport::_nil ();
}

::CORBA::Boolean
TAO::Objref_Traits<PapyrusrtDDS::RawBufferTypeSupport>::marshal (
    const PapyrusrtDDS::RawBufferTypeSupport_ptr p,
    TAO_OutputCDR & cdr)
{
  return ::CORBA::Object::marshal (p, cdr);
}

PapyrusrtDDS::RawBufferTypeSupport::RawBufferTypeSupport (void)
{}

PapyrusrtDDS::RawBufferTypeSupport::~RawBufferTypeSupport (void)
{
}

void
PapyrusrtDDS::RawBufferTypeSupport::_tao_any_destructor (void *_tao_void_pointer)
{
  RawBufferTypeSupport *_tao_tmp_pointer =
    static_cast<RawBufferTypeSupport *> (_tao_void_pointer);
  ::CORBA::release (_tao_tmp_pointer);
}

PapyrusrtDDS::RawBufferTypeSupport_ptr
PapyrusrtDDS::RawBufferTypeSupport::_narrow (
    ::CORBA::Object_ptr _tao_objref)
{
  return RawBufferTypeSupport::_duplicate (
      dynamic_cast<RawBufferTypeSupport_ptr> (_tao_objref)
    );
}

PapyrusrtDDS::RawBufferTypeSupport_ptr
PapyrusrtDDS::RawBufferTypeSupport::_unchecked_narrow (
    ::CORBA::Object_ptr _tao_objref)
{
  return RawBufferTypeSupport::_duplicate (
      dynamic_cast<RawBufferTypeSupport_ptr> (_tao_objref)
    );
}

PapyrusrtDDS::RawBufferTypeSupport_ptr
PapyrusrtDDS::RawBufferTypeSupport::_nil (void)
{
  return 0;
}

PapyrusrtDDS::RawBufferTypeSupport_ptr
PapyrusrtDDS::RawBufferTypeSupport::_duplicate (RawBufferTypeSupport_ptr obj)
{
  if (! ::CORBA::is_nil (obj))
    {
      obj->_add_ref ();
    }
  return obj;
}

void
PapyrusrtDDS::RawBufferTypeSupport::_tao_release (RawBufferTypeSupport_ptr obj)
{
  ::CORBA::release (obj);
}

::CORBA::Boolean
PapyrusrtDDS::RawBufferTypeSupport::_is_a (const char *value)
{
  if (
      ACE_OS::strcmp (
          value,
          "IDL:DDS/TypeSupport:1.0"
        ) == 0 ||
      ACE_OS::strcmp (
          value,
          "IDL:OpenDDS/DCPS/TypeSupport:1.0"
        ) == 0 ||
      ACE_OS::strcmp (
          value,
          "IDL:PapyrusrtDDS/RawBufferTypeSupport:1.0"
        ) == 0 ||
      ACE_OS::strcmp (
          value,
          "IDL:omg.org/CORBA/LocalObject:1.0"
        ) == 0 ||
      ACE_OS::strcmp (
          value,
          "IDL:omg.org/CORBA/Object:1.0"
        ) == 0
    )
    {
      return true; // success using local knowledge
    }
  else
    {
      return false;
    }
}

const char* PapyrusrtDDS::RawBufferTypeSupport::_interface_repository_id (void) const
{
  return "IDL:PapyrusrtDDS/RawBufferTypeSupport:1.0";
}

::CORBA::Boolean
PapyrusrtDDS::RawBufferTypeSupport::marshal (TAO_OutputCDR & /* cdr */)
{
  return false;
}

// TAO_IDL - Generated from
// be/be_visitor_typecode/objref_typecode.cpp:72

static TAO::TypeCode::Objref<char const *,
                             TAO::Null_RefCount_Policy>
  _tao_tc_PapyrusrtDDS_RawBufferTypeSupport (
    ::CORBA::tk_local_interface,
    "IDL:PapyrusrtDDS/RawBufferTypeSupport:1.0",
    "RawBufferTypeSupport");
  

namespace PapyrusrtDDS
{
  ::CORBA::TypeCode_ptr const _tc_RawBufferTypeSupport =
    &_tao_tc_PapyrusrtDDS_RawBufferTypeSupport;
}

// TAO_IDL - Generated from
// be/be_visitor_interface/interface_cs.cpp:51

// Traits specializations for PapyrusrtDDS::RawBufferDataWriter.

PapyrusrtDDS::RawBufferDataWriter_ptr
TAO::Objref_Traits<PapyrusrtDDS::RawBufferDataWriter>::duplicate (
    PapyrusrtDDS::RawBufferDataWriter_ptr p)
{
  return PapyrusrtDDS::RawBufferDataWriter::_duplicate (p);
}

void
TAO::Objref_Traits<PapyrusrtDDS::RawBufferDataWriter>::release (
    PapyrusrtDDS::RawBufferDataWriter_ptr p)
{
  ::CORBA::release (p);
}

PapyrusrtDDS::RawBufferDataWriter_ptr
TAO::Objref_Traits<PapyrusrtDDS::RawBufferDataWriter>::nil (void)
{
  return PapyrusrtDDS::RawBufferDataWriter::_nil ();
}

::CORBA::Boolean
TAO::Objref_Traits<PapyrusrtDDS::RawBufferDataWriter>::marshal (
    const PapyrusrtDDS::RawBufferDataWriter_ptr p,
    TAO_OutputCDR & cdr)
{
  return ::CORBA::Object::marshal (p, cdr);
}

PapyrusrtDDS::RawBufferDataWriter::RawBufferDataWriter (void)
{}

PapyrusrtDDS::RawBufferDataWriter::~RawBufferDataWriter (void)
{
}

void
PapyrusrtDDS::RawBufferDataWriter::_tao_any_destructor (void *_tao_void_pointer)
{
  RawBufferDataWriter *_tao_tmp_pointer =
    static_cast<RawBufferDataWriter *> (_tao_void_pointer);
  ::CORBA::release (_tao_tmp_pointer);
}

PapyrusrtDDS::RawBufferDataWriter_ptr
PapyrusrtDDS::RawBufferDataWriter::_narrow (
    ::CORBA::Object_ptr _tao_objref)
{
  return RawBufferDataWriter::_duplicate (
      dynamic_cast<RawBufferDataWriter_ptr> (_tao_objref)
    );
}

PapyrusrtDDS::RawBufferDataWriter_ptr
PapyrusrtDDS::RawBufferDataWriter::_unchecked_narrow (
    ::CORBA::Object_ptr _tao_objref)
{
  return RawBufferDataWriter::_duplicate (
      dynamic_cast<RawBufferDataWriter_ptr> (_tao_objref)
    );
}

PapyrusrtDDS::RawBufferDataWriter_ptr
PapyrusrtDDS::RawBufferDataWriter::_nil (void)
{
  return 0;
}

PapyrusrtDDS::RawBufferDataWriter_ptr
PapyrusrtDDS::RawBufferDataWriter::_duplicate (RawBufferDataWriter_ptr obj)
{
  if (! ::CORBA::is_nil (obj))
    {
      obj->_add_ref ();
    }
  return obj;
}

void
PapyrusrtDDS::RawBufferDataWriter::_tao_release (RawBufferDataWriter_ptr obj)
{
  ::CORBA::release (obj);
}

::CORBA::Boolean
PapyrusrtDDS::RawBufferDataWriter::_is_a (const char *value)
{
  if (
      ACE_OS::strcmp (
          value,
          "IDL:DDS/Entity:1.0"
        ) == 0 ||
      ACE_OS::strcmp (
          value,
          "IDL:DDS/DataWriter:1.0"
        ) == 0 ||
      ACE_OS::strcmp (
          value,
          "IDL:PapyrusrtDDS/RawBufferDataWriter:1.0"
        ) == 0 ||
      ACE_OS::strcmp (
          value,
          "IDL:omg.org/CORBA/LocalObject:1.0"
        ) == 0 ||
      ACE_OS::strcmp (
          value,
          "IDL:omg.org/CORBA/Object:1.0"
        ) == 0
    )
    {
      return true; // success using local knowledge
    }
  else
    {
      return false;
    }
}

const char* PapyrusrtDDS::RawBufferDataWriter::_interface_repository_id (void) const
{
  return "IDL:PapyrusrtDDS/RawBufferDataWriter:1.0";
}

::CORBA::Boolean
PapyrusrtDDS::RawBufferDataWriter::marshal (TAO_OutputCDR & /* cdr */)
{
  return false;
}

// TAO_IDL - Generated from
// be/be_visitor_typecode/objref_typecode.cpp:72

static TAO::TypeCode::Objref<char const *,
                             TAO::Null_RefCount_Policy>
  _tao_tc_PapyrusrtDDS_RawBufferDataWriter (
    ::CORBA::tk_local_interface,
    "IDL:PapyrusrtDDS/RawBufferDataWriter:1.0",
    "RawBufferDataWriter");
  

namespace PapyrusrtDDS
{
  ::CORBA::TypeCode_ptr const _tc_RawBufferDataWriter =
    &_tao_tc_PapyrusrtDDS_RawBufferDataWriter;
}

// TAO_IDL - Generated from
// be/be_visitor_interface/interface_cs.cpp:51

// Traits specializations for PapyrusrtDDS::RawBufferDataReader.

PapyrusrtDDS::RawBufferDataReader_ptr
TAO::Objref_Traits<PapyrusrtDDS::RawBufferDataReader>::duplicate (
    PapyrusrtDDS::RawBufferDataReader_ptr p)
{
  return PapyrusrtDDS::RawBufferDataReader::_duplicate (p);
}

void
TAO::Objref_Traits<PapyrusrtDDS::RawBufferDataReader>::release (
    PapyrusrtDDS::RawBufferDataReader_ptr p)
{
  ::CORBA::release (p);
}

PapyrusrtDDS::RawBufferDataReader_ptr
TAO::Objref_Traits<PapyrusrtDDS::RawBufferDataReader>::nil (void)
{
  return PapyrusrtDDS::RawBufferDataReader::_nil ();
}

::CORBA::Boolean
TAO::Objref_Traits<PapyrusrtDDS::RawBufferDataReader>::marshal (
    const PapyrusrtDDS::RawBufferDataReader_ptr p,
    TAO_OutputCDR & cdr)
{
  return ::CORBA::Object::marshal (p, cdr);
}

PapyrusrtDDS::RawBufferDataReader::RawBufferDataReader (void)
{}

PapyrusrtDDS::RawBufferDataReader::~RawBufferDataReader (void)
{
}

void
PapyrusrtDDS::RawBufferDataReader::_tao_any_destructor (void *_tao_void_pointer)
{
  RawBufferDataReader *_tao_tmp_pointer =
    static_cast<RawBufferDataReader *> (_tao_void_pointer);
  ::CORBA::release (_tao_tmp_pointer);
}

PapyrusrtDDS::RawBufferDataReader_ptr
PapyrusrtDDS::RawBufferDataReader::_narrow (
    ::CORBA::Object_ptr _tao_objref)
{
  return RawBufferDataReader::_duplicate (
      dynamic_cast<RawBufferDataReader_ptr> (_tao_objref)
    );
}

PapyrusrtDDS::RawBufferDataReader_ptr
PapyrusrtDDS::RawBufferDataReader::_unchecked_narrow (
    ::CORBA::Object_ptr _tao_objref)
{
  return RawBufferDataReader::_duplicate (
      dynamic_cast<RawBufferDataReader_ptr> (_tao_objref)
    );
}

PapyrusrtDDS::RawBufferDataReader_ptr
PapyrusrtDDS::RawBufferDataReader::_nil (void)
{
  return 0;
}

PapyrusrtDDS::RawBufferDataReader_ptr
PapyrusrtDDS::RawBufferDataReader::_duplicate (RawBufferDataReader_ptr obj)
{
  if (! ::CORBA::is_nil (obj))
    {
      obj->_add_ref ();
    }
  return obj;
}

void
PapyrusrtDDS::RawBufferDataReader::_tao_release (RawBufferDataReader_ptr obj)
{
  ::CORBA::release (obj);
}

::CORBA::Boolean
PapyrusrtDDS::RawBufferDataReader::_is_a (const char *value)
{
  if (
      ACE_OS::strcmp (
          value,
          "IDL:DDS/Entity:1.0"
        ) == 0 ||
      ACE_OS::strcmp (
          value,
          "IDL:DDS/DataReader:1.0"
        ) == 0 ||
      ACE_OS::strcmp (
          value,
          "IDL:OpenDDS/DCPS/DataReaderEx:1.0"
        ) == 0 ||
      ACE_OS::strcmp (
          value,
          "IDL:PapyrusrtDDS/RawBufferDataReader:1.0"
        ) == 0 ||
      ACE_OS::strcmp (
          value,
          "IDL:omg.org/CORBA/LocalObject:1.0"
        ) == 0 ||
      ACE_OS::strcmp (
          value,
          "IDL:omg.org/CORBA/Object:1.0"
        ) == 0
    )
    {
      return true; // success using local knowledge
    }
  else
    {
      return false;
    }
}

const char* PapyrusrtDDS::RawBufferDataReader::_interface_repository_id (void) const
{
  return "IDL:PapyrusrtDDS/RawBufferDataReader:1.0";
}

::CORBA::Boolean
PapyrusrtDDS::RawBufferDataReader::marshal (TAO_OutputCDR & /* cdr */)
{
  return false;
}

// TAO_IDL - Generated from
// be/be_visitor_typecode/objref_typecode.cpp:72

static TAO::TypeCode::Objref<char const *,
                             TAO::Null_RefCount_Policy>
  _tao_tc_PapyrusrtDDS_RawBufferDataReader (
    ::CORBA::tk_local_interface,
    "IDL:PapyrusrtDDS/RawBufferDataReader:1.0",
    "RawBufferDataReader");
  

namespace PapyrusrtDDS
{
  ::CORBA::TypeCode_ptr const _tc_RawBufferDataReader =
    &_tao_tc_PapyrusrtDDS_RawBufferDataReader;
}

// TAO_IDL - Generated from
// be/be_visitor_interface/any_op_cs.cpp:41
TAO_BEGIN_VERSIONED_NAMESPACE_DECL



namespace TAO
{
  template<>
  ::CORBA::Boolean
  Any_Impl_T<PapyrusrtDDS::RawBufferTypeSupport>::to_object (
      ::CORBA::Object_ptr &_tao_elem) const
  {
    _tao_elem = ::CORBA::Object::_duplicate (this->value_);
    return true;
  }
}

namespace TAO
{
  template<>
  ::CORBA::Boolean
  Any_Impl_T<PapyrusrtDDS::RawBufferTypeSupport>::marshal_value (TAO_OutputCDR &)
  {
    return false;
  }

  template<>
  ::CORBA::Boolean
  Any_Impl_T<PapyrusrtDDS::RawBufferTypeSupport>::demarshal_value (TAO_InputCDR &)
  {
    return false;
  }
}
TAO_END_VERSIONED_NAMESPACE_DECL



#if defined (ACE_ANY_OPS_USE_NAMESPACE)

namespace PapyrusrtDDS
{
  

  /// Copying insertion.
  void
  operator<<= (
      ::CORBA::Any &_tao_any,
      RawBufferTypeSupport_ptr _tao_elem)
  {
    RawBufferTypeSupport_ptr _tao_objptr =
      RawBufferTypeSupport::_duplicate (_tao_elem);
    _tao_any <<= &_tao_objptr;
  }

  /// Non-copying insertion.
  void
  operator<<= (
      ::CORBA::Any &_tao_any,
      RawBufferTypeSupport_ptr *_tao_elem)
  {
    TAO::Any_Impl_T<RawBufferTypeSupport>::insert (
        _tao_any,
        RawBufferTypeSupport::_tao_any_destructor,
        _tc_RawBufferTypeSupport,
        *_tao_elem);
  }

  ::CORBA::Boolean
  operator>>= (
      const ::CORBA::Any &_tao_any,
      RawBufferTypeSupport_ptr &_tao_elem)
  {
    return
      TAO::Any_Impl_T<RawBufferTypeSupport>::extract (
          _tao_any,
          RawBufferTypeSupport::_tao_any_destructor,
          _tc_RawBufferTypeSupport,
          _tao_elem);
  }
}

#else

TAO_BEGIN_VERSIONED_NAMESPACE_DECL



/// Copying insertion.
void
operator<<= (
    ::CORBA::Any &_tao_any,
    PapyrusrtDDS::RawBufferTypeSupport_ptr _tao_elem)
{
  PapyrusrtDDS::RawBufferTypeSupport_ptr _tao_objptr =
    PapyrusrtDDS::RawBufferTypeSupport::_duplicate (_tao_elem);
  _tao_any <<= &_tao_objptr;
}

/// Non-copying insertion.
void
operator<<= (
    ::CORBA::Any &_tao_any,
    PapyrusrtDDS::RawBufferTypeSupport_ptr *_tao_elem)
{
  TAO::Any_Impl_T<PapyrusrtDDS::RawBufferTypeSupport>::insert (
      _tao_any,
      PapyrusrtDDS::RawBufferTypeSupport::_tao_any_destructor,
      PapyrusrtDDS::_tc_RawBufferTypeSupport,
      *_tao_elem);
}

::CORBA::Boolean
operator>>= (
    const ::CORBA::Any &_tao_any,
    PapyrusrtDDS::RawBufferTypeSupport_ptr &_tao_elem)
{
  return
    TAO::Any_Impl_T<PapyrusrtDDS::RawBufferTypeSupport>::extract (
        _tao_any,
        PapyrusrtDDS::RawBufferTypeSupport::_tao_any_destructor,
        PapyrusrtDDS::_tc_RawBufferTypeSupport,
        _tao_elem);
}

TAO_END_VERSIONED_NAMESPACE_DECL



#endif

// TAO_IDL - Generated from
// be/be_visitor_interface/any_op_cs.cpp:41
TAO_BEGIN_VERSIONED_NAMESPACE_DECL



namespace TAO
{
  template<>
  ::CORBA::Boolean
  Any_Impl_T<PapyrusrtDDS::RawBufferDataWriter>::to_object (
      ::CORBA::Object_ptr &_tao_elem) const
  {
    _tao_elem = ::CORBA::Object::_duplicate (this->value_);
    return true;
  }
}

namespace TAO
{
  template<>
  ::CORBA::Boolean
  Any_Impl_T<PapyrusrtDDS::RawBufferDataWriter>::marshal_value (TAO_OutputCDR &)
  {
    return false;
  }

  template<>
  ::CORBA::Boolean
  Any_Impl_T<PapyrusrtDDS::RawBufferDataWriter>::demarshal_value (TAO_InputCDR &)
  {
    return false;
  }
}
TAO_END_VERSIONED_NAMESPACE_DECL



#if defined (ACE_ANY_OPS_USE_NAMESPACE)

namespace PapyrusrtDDS
{
  

  /// Copying insertion.
  void
  operator<<= (
      ::CORBA::Any &_tao_any,
      RawBufferDataWriter_ptr _tao_elem)
  {
    RawBufferDataWriter_ptr _tao_objptr =
      RawBufferDataWriter::_duplicate (_tao_elem);
    _tao_any <<= &_tao_objptr;
  }

  /// Non-copying insertion.
  void
  operator<<= (
      ::CORBA::Any &_tao_any,
      RawBufferDataWriter_ptr *_tao_elem)
  {
    TAO::Any_Impl_T<RawBufferDataWriter>::insert (
        _tao_any,
        RawBufferDataWriter::_tao_any_destructor,
        _tc_RawBufferDataWriter,
        *_tao_elem);
  }

  ::CORBA::Boolean
  operator>>= (
      const ::CORBA::Any &_tao_any,
      RawBufferDataWriter_ptr &_tao_elem)
  {
    return
      TAO::Any_Impl_T<RawBufferDataWriter>::extract (
          _tao_any,
          RawBufferDataWriter::_tao_any_destructor,
          _tc_RawBufferDataWriter,
          _tao_elem);
  }
}

#else

TAO_BEGIN_VERSIONED_NAMESPACE_DECL



/// Copying insertion.
void
operator<<= (
    ::CORBA::Any &_tao_any,
    PapyrusrtDDS::RawBufferDataWriter_ptr _tao_elem)
{
  PapyrusrtDDS::RawBufferDataWriter_ptr _tao_objptr =
    PapyrusrtDDS::RawBufferDataWriter::_duplicate (_tao_elem);
  _tao_any <<= &_tao_objptr;
}

/// Non-copying insertion.
void
operator<<= (
    ::CORBA::Any &_tao_any,
    PapyrusrtDDS::RawBufferDataWriter_ptr *_tao_elem)
{
  TAO::Any_Impl_T<PapyrusrtDDS::RawBufferDataWriter>::insert (
      _tao_any,
      PapyrusrtDDS::RawBufferDataWriter::_tao_any_destructor,
      PapyrusrtDDS::_tc_RawBufferDataWriter,
      *_tao_elem);
}

::CORBA::Boolean
operator>>= (
    const ::CORBA::Any &_tao_any,
    PapyrusrtDDS::RawBufferDataWriter_ptr &_tao_elem)
{
  return
    TAO::Any_Impl_T<PapyrusrtDDS::RawBufferDataWriter>::extract (
        _tao_any,
        PapyrusrtDDS::RawBufferDataWriter::_tao_any_destructor,
        PapyrusrtDDS::_tc_RawBufferDataWriter,
        _tao_elem);
}

TAO_END_VERSIONED_NAMESPACE_DECL



#endif

// TAO_IDL - Generated from
// be/be_visitor_interface/any_op_cs.cpp:41
TAO_BEGIN_VERSIONED_NAMESPACE_DECL



namespace TAO
{
  template<>
  ::CORBA::Boolean
  Any_Impl_T<PapyrusrtDDS::RawBufferDataReader>::to_object (
      ::CORBA::Object_ptr &_tao_elem) const
  {
    _tao_elem = ::CORBA::Object::_duplicate (this->value_);
    return true;
  }
}

namespace TAO
{
  template<>
  ::CORBA::Boolean
  Any_Impl_T<PapyrusrtDDS::RawBufferDataReader>::marshal_value (TAO_OutputCDR &)
  {
    return false;
  }

  template<>
  ::CORBA::Boolean
  Any_Impl_T<PapyrusrtDDS::RawBufferDataReader>::demarshal_value (TAO_InputCDR &)
  {
    return false;
  }
}
TAO_END_VERSIONED_NAMESPACE_DECL



#if defined (ACE_ANY_OPS_USE_NAMESPACE)

namespace PapyrusrtDDS
{
  

  /// Copying insertion.
  void
  operator<<= (
      ::CORBA::Any &_tao_any,
      RawBufferDataReader_ptr _tao_elem)
  {
    RawBufferDataReader_ptr _tao_objptr =
      RawBufferDataReader::_duplicate (_tao_elem);
    _tao_any <<= &_tao_objptr;
  }

  /// Non-copying insertion.
  void
  operator<<= (
      ::CORBA::Any &_tao_any,
      RawBufferDataReader_ptr *_tao_elem)
  {
    TAO::Any_Impl_T<RawBufferDataReader>::insert (
        _tao_any,
        RawBufferDataReader::_tao_any_destructor,
        _tc_RawBufferDataReader,
        *_tao_elem);
  }

  ::CORBA::Boolean
  operator>>= (
      const ::CORBA::Any &_tao_any,
      RawBufferDataReader_ptr &_tao_elem)
  {
    return
      TAO::Any_Impl_T<RawBufferDataReader>::extract (
          _tao_any,
          RawBufferDataReader::_tao_any_destructor,
          _tc_RawBufferDataReader,
          _tao_elem);
  }
}

#else

TAO_BEGIN_VERSIONED_NAMESPACE_DECL



/// Copying insertion.
void
operator<<= (
    ::CORBA::Any &_tao_any,
    PapyrusrtDDS::RawBufferDataReader_ptr _tao_elem)
{
  PapyrusrtDDS::RawBufferDataReader_ptr _tao_objptr =
    PapyrusrtDDS::RawBufferDataReader::_duplicate (_tao_elem);
  _tao_any <<= &_tao_objptr;
}

/// Non-copying insertion.
void
operator<<= (
    ::CORBA::Any &_tao_any,
    PapyrusrtDDS::RawBufferDataReader_ptr *_tao_elem)
{
  TAO::Any_Impl_T<PapyrusrtDDS::RawBufferDataReader>::insert (
      _tao_any,
      PapyrusrtDDS::RawBufferDataReader::_tao_any_destructor,
      PapyrusrtDDS::_tc_RawBufferDataReader,
      *_tao_elem);
}

::CORBA::Boolean
operator>>= (
    const ::CORBA::Any &_tao_any,
    PapyrusrtDDS::RawBufferDataReader_ptr &_tao_elem)
{
  return
    TAO::Any_Impl_T<PapyrusrtDDS::RawBufferDataReader>::extract (
        _tao_any,
        PapyrusrtDDS::RawBufferDataReader::_tao_any_destructor,
        PapyrusrtDDS::_tc_RawBufferDataReader,
        _tao_elem);
}

TAO_END_VERSIONED_NAMESPACE_DECL



#endif

