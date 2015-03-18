//  Boost general library logging.hpp header file  ---------------------------/

//  (C) Copyright Jean-Daniel Michaud, Ilya Pokolev 2007. 
//  Permission to copy, use, modify, sell and distribute this software is 
//  granted provided this copyright notice appears in all copies. 
//  This software is provided "as is" without express or implied warranty, 
//  and with no claim as to its suitability for any purpose.

//  See http://www.boost.org/LICENSE_1_0.txt for licensing.
//  See http://code.google.com/p/loglite/ for library home page.

#ifndef BOOST_LOGGING_HPP
#define BOOST_LOGGING_HPP

#include <list>
#include <stack>
#include <string>
#include <locale>
#include <ostream>
#include <sstream>
#include <stdio.h>
#include <algorithm>
#include <exception>

#ifdef _MSC_VER
#pragma warning(push, 0)
#endif /*#ifdef _MSC_VER*/
#include <boost/ref.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#ifndef BOOST_CONFIG_HPP
#  include <boost/config.hpp>
#endif
#if defined(BOOST_HAS_THREADS)
#  include <boost/thread/once.hpp>
#  include <boost/thread/thread.hpp>
#  include <boost/thread/condition.hpp>
#endif // BOOST_HAS_THREADS
#include <boost/filesystem.hpp>
#include <boost/date_time/date_facet.hpp>
#include <boost/date_time/time_facet.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/local_time/local_time.hpp>

#ifdef _MSC_VER
#pragma warning(pop)
#endif /*#ifdef _MSC_VER*/

// Code copied from redhat website
#if __STDC_VERSION__ < 199901L
# if __GNUC__ >= 2
#  define __func__ __FUNCTION__
# else
#  define __func__ "<unknown>"
# endif
#endif
// !Code copied from redhat website

#if !defined(__PRETTY_FUNCTION__)
#define __PRETTY_FUNCTION__ __FUNCTION__
#endif

#ifndef BOOST_NO_CODE_GENERATION_FOR_LOG
#define BOOST_LOG_INIT( format )                                              \
{                                                                             \
  boost::logging::logger_p l = boost::logging::logger::get_instance();        \
  l->add_format(format);                                                      \
}

#define BOOST_LOG_ADD_OUTPUT_STREAM( sink )                                   \
{                                                                             \
  boost::logging::logger_p l = boost::logging::logger::get_instance();        \
  l->add_sink(sink);                                                          \
}

#define BOOST_LOG(mask, qualifier, _trace)                                    \
{                                                                             \
  boost::logging::logger_p l = boost::logging::logger::get_instance();        \
  if (l->get_global_log_mask() & mask)                                        \
  {                                                                           \
    std::stringstream string_stream;                                          \
    string_stream << _trace;                                                  \
    boost::logging::log_param_t log_param = { mask,                           \
                              &qualifier,                                     \
                              string_stream.str(),                            \
                              __FILE__, __LINE__,                             \
                              __func__, __PRETTY_FUNCTION__, NULL };          \
    l->trace(log_param);                                                      \
  }                                                                           \
}

#define BOOST_LOG_( mask, _trace )                                            \
  { BOOST_LOG(mask, boost::logging::log, _trace) }

#define BOOST_LOG_UNFORMATTED(mask, qualifier, _trace)                        \
{                                                                             \
  boost::logging::logger_p l = boost::logging::logger::get_instance();        \
  if (l->get_global_log_mask() & mask)                                        \
  {                                                                           \
    std::stringstream string_stream;                                          \
    string_stream << _trace;                                                  \
    boost::logging::log_param_t log_param = { mask,                           \
                              &qualifier,                                     \
                              string_stream.str(),                            \
                              __FILE__, __LINE__,                             \
                              __func__, __PRETTY_FUNCTION__, NULL } ;         \
    l->unformatted_trace(log_param);                                          \
  }                                                                           \
}

#define BOOST_LOG_SCOPE( mask, scope_object )                                 \
  boost::logging::scope_item scope_object                                     \
  (                                                                           \
    mask,                                                                     \
    &boost::logging::scope,                                                   \
    __FILE__, __LINE__,                                                       \
    __func__, __PRETTY_FUNCTION__                                             \
  );

#else // !BOOST_NO_CODE_GENERATION_FOR_LOG
#define BOOST_LOG_INIT( format )
#define BOOST_LOG_ADD_OUTPUT_STREAM( sink )
#define BOOST_LOG(mask, qualifier, _trace)
#define BOOST_LOG_( mask, _trace )
#define BOOST_LOG_UNFORMATTED(mask, qualifier, _trace)
#endif // BOOST_NO_CODE_GENERATION_FOR_LOG

#define BOOST_LOG_MAX_LINE_STR_SIZE       20    // log(2^64)
#define BOOST_LOG_MASK_UP_LIMIT           65535 // 2^sizeof (mask_t) - 1
#define BOOST_LOG_MASK_LEVEL_1            1     // 0001
#define BOOST_LOG_MASK_LEVEL_2            3     // 0011
#define BOOST_LOG_MASK_LEVEL_3            7     // 0111
#define BOOST_LOG_MASK_LEVEL_4            15    // 1111
#define BOOST_LOG_LEVEL_1                 1
#define BOOST_LOG_LEVEL_2                 2
#define BOOST_LOG_LEVEL_3                 4
#define BOOST_LOG_LEVEL_4                 8

#define BOOST_LOG_L1( _trace )  BOOST_LOG_( BOOST_LOG_LEVEL_1, _trace )
#define BOOST_LOG_L2( _trace )  BOOST_LOG_( BOOST_LOG_LEVEL_2, _trace )
#define BOOST_LOG_L3( _trace )  BOOST_LOG_( BOOST_LOG_LEVEL_3, _trace )
#define BOOST_LOG_L4( _trace )  BOOST_LOG_( BOOST_LOG_LEVEL_4, _trace )

namespace boost {

  namespace logging {

//  Logging forward declarations ---------------------------------------------//
    class log_element;
    class mask_element;
    class qualifier;
    class trace_element;
    class format;
    class sink;
    class logger;
    class scope_item;
    
//  Logging typedefs declarations --------------------------------------------//
    typedef enum { SINK = 0, FORMAT }                   sink_format_assoc_e;
    typedef std::list<boost::shared_ptr<log_element> >  element_list_t;
    typedef std::list<boost::shared_ptr<std::ostream> > stream_list_t;
    typedef unsigned short                              mask_t;
    typedef std::vector<scope_item *>                   scope_stack_t;
    typedef struct
    {
      mask_t           m_mask;
      const qualifier *m_qualifier;
      std::string      m_trace;
      std::string      m_filename;
      unsigned int     m_line;
      std::string      m_func_name;
      std::string      m_func_sig;
      scope_stack_t    *m_scope;
    } log_param_t;
    typedef std::list<format>                           format_list_t;
    typedef tuple<sink, format>                         sink_format_assoc_t;
    typedef std::list<sink_format_assoc_t>            sink_format_assoc_list_t;
    typedef std::list<qualifier *>                      qualifier_list_t;
    typedef shared_ptr<logger>                          logger_p;

//  Used for shared_ptr() on statically allocated log_element ----------------//
    struct null_deleter
    { void operator()(void const *) const {} };

//  Qualifier class declaration ----------------------------------------------//
    class qualifier
    {
    public:
      qualifier() {}
      virtual ~qualifier() {}
      inline std::string to_string() const { return m_identifier; }
      virtual bool operator==(const qualifier &) { return false; }
    protected:
      std::string m_identifier;
    };

    class log_qualifier : public qualifier 
    {
    public:
      log_qualifier() { m_identifier = "log"; }
      bool operator==(const qualifier &q) 
      { return (dynamic_cast<const log_qualifier *>(&q) != NULL); }
    };

    class notice_qualifier : public qualifier
    {
    public:
      notice_qualifier() { m_identifier = "notice"; }
      bool operator==(const qualifier &q) 
      { return (dynamic_cast<const notice_qualifier *>(&q) != NULL); }
    };

    class scope_qualifier : public qualifier
    {
    public:
      scope_qualifier() { m_identifier = "scope"; }
      bool operator==(const qualifier &q) 
      { return (dynamic_cast<const scope_qualifier *>(&q) != NULL); }
    };

    class warning_qualifier : public qualifier
    {
    public:
      warning_qualifier() { m_identifier = "warning"; }
      bool operator==(const qualifier &q) 
      { return (dynamic_cast<const warning_qualifier *>(&q) != NULL); }
    };

    class error_qualifier : public qualifier
    {
    public:
      error_qualifier() { m_identifier = "error"; }
      bool operator==(const qualifier &q) 
      { return (dynamic_cast<const error_qualifier *>(&q) != NULL); }
    };

//  Element classes declaration  ---------------------------------------------//
    class log_element
    {
    public:
      virtual ~log_element() {}
      virtual std::string to_string() { assert(0); return ""; };

      virtual std::string visit(format &f, const log_param_t &log_param);
    };
    
    class mask_element : public log_element
    {
    public:
      std::string to_string(mask_t l) 
      { 
        return str(boost::format("%i") % l);
      };

      std::string visit(format &f, const log_param_t &log_param);
    };
    
    class filename_element : public log_element
    {
    public:
      typedef enum { full_path, filename_only } display_format_e;

      filename_element() : m_display_format(full_path) {}
      filename_element(display_format_e d) : m_display_format(d) {}

      std::string to_string(const std::string &f) 
      { 
        if (m_display_format == full_path)
          return f;
        
        boost::filesystem::path p(f);
        return p.string();
      }
      std::string visit(format &f, const log_param_t &log_param);

    private:
      display_format_e m_display_format;
    };
    
    class line_element : public log_element
    {
    public:
      std::string to_string(unsigned int l) 
      {
        return str(boost::format("%i") % l);
      }
      std::string visit(format &f, const log_param_t &log_param);
    };
    
    class date_element : public log_element
    {
    public:
      date_element()
      {
        m_output_facet = new boost::gregorian::date_facet();
        m_output_facet->set_iso_extended_format();
        m_ss.reset(new std::stringstream());
        m_ss->imbue(std::locale(std::locale::classic(), m_output_facet));
      }

      date_element(const std::string &format)
      {
        m_output_facet = new boost::gregorian::date_facet();
        m_output_facet->format(format.c_str());
        m_ss.reset(new std::stringstream());
        m_ss->imbue(std::locale(std::locale::classic(), m_output_facet));
      }

      std::string to_string()
      {
        boost::gregorian::date d(boost::gregorian::day_clock::local_day());
        m_ss->str("");
        (*m_ss) << d;
        return m_ss->str();
      }

    private:
      boost::shared_ptr<std::stringstream> m_ss;
      // no shared ptr. the stringstream take ownership of the facet
      // once imbue called.
      boost::gregorian::date_facet *m_output_facet;
    };
    
    class time_element : public log_element
    {
    public:
      time_element()
      {
        m_output_facet = new boost::posix_time::time_facet();
        m_output_facet->set_iso_extended_format();
        m_ss.reset(new std::stringstream());
        m_ss->imbue(std::locale(std::locale::classic(), m_output_facet));
      }

      time_element(const std::string &format)
      {
        m_output_facet = new boost::posix_time::time_facet();
        m_output_facet->format(format.c_str());
        m_ss.reset(new std::stringstream());
        m_ss->imbue(std::locale(std::locale::classic(), m_output_facet));
      }

      std::string to_string() 
      { 
        boost::posix_time::ptime 
          t(boost::posix_time::microsec_clock::local_time());
        m_ss->str("");
        (*m_ss) << t;
        return m_ss->str();
      };

    private:
      boost::shared_ptr<std::stringstream> m_ss;
      // no shared ptr. the stringstream take ownership of the facet
      // once imbue called.
      boost::posix_time::time_facet *m_output_facet;
    };
    
    class trace_element : public log_element
    {
    public:
      std::string to_string(const std::string& s) { return s; };

      std::string visit(format &f, const log_param_t &log_param);
    };

    class eol_element : public log_element
    {
    public:
      std::string to_string() { return "\n"; };
    };

    class literal_element : public log_element
    {
    public:
      explicit literal_element(std::string l) : m_literal(l) {}
      std::string to_string() { return m_literal; };
    private:
      std::string m_literal;
    };

    class qualifier_element : public log_element
    {
    public:
      qualifier_element(const qualifier &lq) 
      {
        m_qualifier_identifier = lq.to_string();
      }
      std::string to_string() { return m_qualifier_identifier; };
    private:
      std::string m_qualifier_identifier;
    };

    // end-of-trace qualifier
    // Used to delimite a formatted trace when transmitted through network
    // or shared_memory streams.
    class eot_element : public log_element
    {
    public:
      std::string to_string() { return "\f"; };
    };

    class function_name_element : public log_element  //function name qualifier
    {
    public:
      std::string to_string(const std::string &n) { return n; }
      std::string visit(format &f, const log_param_t &log_param);
    };

    // pretty function name qualifier
    class pretty_function_name_element : public log_element
    {
    public:
      std::string to_string(const std::string &n) { return n; }
      std::string visit(format &f, const log_param_t &log_param);
    };

    // scope qualifier
    // Dump a string containing the stack scope maintained by the
    // creation of scope_item object or the call to macro BOOST_LOG_SCOPE
    class scope_element : public log_element  
    {
    public:
      std::string to_string(scope_stack_t *s);
      std::string visit(format &f, const log_param_t &log_param);
    };

//  Format class declaration ------------------------------------------------//
    class format
    {
    public:
      format(log_element &e)
        : m_identifier("unnamed") 
      {
        boost::shared_ptr<boost::logging::log_element> p(&e, null_deleter());
        m_element_list.push_back(p);
      }

      format(log_element &e, const std::string &identifier) 
        : m_identifier(identifier) 
      {
        boost::shared_ptr<boost::logging::log_element> p(&e, null_deleter());
        m_element_list.push_back(p);
      }

      format(element_list_t e) 
        : m_element_list(e), m_identifier("unnamed") {}

      format(element_list_t e, const std::string &identifier) 
        : m_element_list(e), m_identifier(identifier) {}

      std::string produce_trace(const log_param_t &log_param)
      {
        element_list_t::iterator e_it = m_element_list.begin();
        std::stringstream str_stream;
        for (; e_it != m_element_list.end(); ++e_it)
        {
          str_stream << (*e_it)->visit(*this, log_param);
        }

        return str_stream.str();
      }

      // Visitors for the log elements
      std::string accept(log_element &e)
      {
        return e.to_string();
      }
      std::string accept(mask_element &e, mask_t l)
      {
        return e.to_string(l);
      }
      std::string accept(trace_element &e, const std::string& s)
      {
        return e.to_string(s);
      }
      std::string accept(filename_element &e, const std::string& s)
      {
        return e.to_string(s);
      }
      std::string accept(line_element &e, unsigned int l)
      {
        return e.to_string(l);
      }
      std::string accept(function_name_element &e, const std::string& s)
      {
        return e.to_string(s);
      }
      std::string accept(pretty_function_name_element &e, const std::string& s)
      {
        return e.to_string(s);
      }
      std::string accept(scope_element &e, scope_stack_t *s)
      {
        return e.to_string(s);
      }

    private:
      element_list_t    m_element_list;
      std::string       m_identifier;
    };

//  Sink class declaration --------------------------------------------------//
    class sink
    {
    public:
      sink(std::ostream *s, mask_t log_mask = 1)
      {
        if (s){
          if (*s == std::cout || *s == std::cerr || *s == std::clog){
            m_output_stream.reset(s, null_deleter());
	  } else {
            m_output_stream.reset(s);
	  }
	}
        set_log_mask(log_mask);
      }
      
      void set_log_mask(mask_t log_mask)
      { 
        //        m_log_mask = ((BOOST_LOG_MASK_UP_LIMIT < log_mask) 
        //          ? BOOST_LOG_MASK_UP_LIMIT : log_mask);
        m_log_mask = log_mask;
      }

      inline mask_t get_log_mask() const { return m_log_mask; }

      void consume_trace(format &f, const log_param_t &log_param)
      {
        /* make here check to avoid producing a useless trace */
        if (log_param.m_mask > m_log_mask)
          return ;

        qualifier_list_t::const_iterator it = m_qualifier_list.begin();
        bool qualifier_present = false;
        for ( ; !qualifier_present && it !=  m_qualifier_list.end(); ++it)
          qualifier_present = (**it == *log_param.m_qualifier);

        if (!qualifier_present)
          return ;

        *m_output_stream << f.produce_trace(log_param);
      }

      void attach_qualifier(qualifier &q)
      {
        m_qualifier_list.push_back(&q);
      }

    private:
      mask_t                   m_log_mask;
      shared_ptr<std::ostream> m_output_stream;
      qualifier_list_t         m_qualifier_list;      
    };
   
//  Element static instantiations --------------------------------------------//
    static mask_element      mask      = mask_element();
    static filename_element  filename  = filename_element();
    static line_element      line      = line_element();
    static date_element      date      = date_element();
    static time_element      time      = time_element();
    static trace_element     trace     = trace_element();
    static eol_element       eol       = eol_element();
    static eot_element       eot       = eot_element();
    static function_name_element 
                       function_name   = function_name_element();
    static pretty_function_name_element 
                  function_signature   = pretty_function_name_element();
    static scope_element   scope_stack = scope_element();

    static log_qualifier     log       = log_qualifier();
    static notice_qualifier  notice    = notice_qualifier();
    static scope_qualifier   scope     = scope_qualifier();
    static warning_qualifier warning   = warning_qualifier();
    static error_qualifier   error     = error_qualifier();

//  Logger class declaration  ------------------------------------------------//
    static logger_p g__logger;
    class logger
    {
    public: 
      logger() : m_global_log_mask(0) {}
        
      static void create_instance()
      {
        g__logger.reset(new logger());
      }

      static logger_p &get_instance()
      {
        static boost::once_flag   once = BOOST_ONCE_INIT;
        boost::call_once(logger::create_instance, once);
        return g__logger;
      }

      void add_format(format f)
      {
        m_format_list.push_back(f);
      }

      void add_sink(sink s)
      {
        if (m_format_list.begin() == m_format_list.end())
          throw "no format defined";

        // Updating global_mask used for full lazy evaluation
        m_global_log_mask |= s.get_log_mask();

        m_sink_format_assoc.push_back
          (
            sink_format_assoc_t(s, *m_format_list.begin())
          );
      }

      void add_sink(sink s, format f)
      {
        // Updating global_mask used for full lazy evaluation
        m_global_log_mask |= s.get_log_mask();

        m_sink_format_assoc.push_back(sink_format_assoc_t(s, f));
      }

      inline mask_t get_global_log_mask() 
      { return m_global_log_mask; }

      void trace(mask_t m, 
                 const qualifier &q, 
                 const std::string &s, 
                 const std::string &f = "<unknown>",
                 unsigned int l = 0)
      {
        log_param_t log_param = { m, &q, s, f, l, 
                                  "<unknown>", "<unknown>", NULL };
        trace(log_param);
      }

      void unformatted_trace(mask_t m, 
                             const qualifier &q, 
                             const std::string &s)
      {
        log_param_t log_param = { m, &q, s, 
                                  "<unknown>", 0, 
                                  "<unknown>", "<unknown>", NULL };
        unformatted_trace(log_param);
      }

      void trace(log_param_t &log_param)
      {
#if defined(BOOST_HAS_THREADS)
        boost::mutex::scoped_lock scoped_lock(m_mutex);
#endif // BOOST_HAS_THREADS

        log_param.m_scope = &m_scope_stack; // Attach the scope stack here
        sink_format_assoc_list_t::iterator 
          s_it = m_sink_format_assoc.begin();
        for (; s_it != m_sink_format_assoc.end(); ++s_it)
        {
          get<SINK>(*s_it).consume_trace(get<FORMAT>(*s_it), log_param);
        }
      }
      
      void unformatted_trace(log_param_t &log_param);

      void push_scope(scope_item *s) { m_scope_stack.push_back(s); }
      void pop_scope() { m_scope_stack.pop_back(); }

    private:
      format_list_t            m_format_list;
      sink_format_assoc_list_t m_sink_format_assoc;

      // The global log mask is the highest log mask on all the sink
      // added to the logger. If no sink as a log mask matching the global
      // mask for a trace, the trace does not need to be evaluated.
      mask_t                  m_global_log_mask;

      // The scope stack. Can be access using the scope_stack element
      scope_stack_t           m_scope_stack;

#if defined(BOOST_HAS_THREADS)
      boost::mutex             m_mutex;
#endif // BOOST_HAS_THREADS
    };  // logger

//  Scope Logger class declaration -------------------------------------------//
    class scope_item
    {
    public:
      scope_item(const std::string &scope_identifier,
                 mask_t mask = BOOST_LOG_LEVEL_1)
        : m_scope_identifier(scope_identifier),
          m_mask(mask)
      {
        log_param_t lp = { m_mask, &scope, 
                           "Entering " + m_scope_identifier,
                           "<unknown>", 0, 
                           "<unknown>", "<unknown>", NULL };
        m_log_param = lp;
        g__logger->trace(m_log_param);
        g__logger->push_scope(this);
      }
      
      scope_item(mask_t m,
                 qualifier *q,
                 const std::string &f,
                 unsigned int l,
                 const std::string &func_name,
                 const std::string &func_sig)
        : m_scope_identifier(func_name),
          m_mask(m)
      {
        log_param_t lp = { m, q, "Entering " + func_name,
                           f, l, func_name, func_sig, NULL };
        m_log_param = lp;
        g__logger->trace(m_log_param);
        g__logger->push_scope(this);
      }

      ~scope_item()
      {
        m_log_param.m_trace = "Exiting " + m_scope_identifier;
        g__logger->trace(m_log_param);
        g__logger->pop_scope();
      }

      std::string get_id() { return m_scope_identifier; }

    private:
      std::string m_scope_identifier;
      mask_t      m_mask;

      log_param_t m_log_param;
    };

//  Element functions definition ---------------------------------------------//
    inline std::string log_element::visit(format &f, 
                                          const log_param_t &)
    {
      return f.accept(*this);
    }

    inline std::string mask_element::visit(format &f, 
                                           const log_param_t &log_param)
    {
      return f.accept(*this, log_param.m_mask);
    }

    inline std::string trace_element::visit(format &f, 
                                            const log_param_t &log_param)
    {
      return f.accept(*this, log_param.m_trace);
    }

    inline std::string filename_element::visit(format &f, 
                                               const log_param_t &log_param)
    {
      return f.accept(*this, log_param.m_filename);
    }

    inline std::string line_element::visit(format &f, 
                                           const log_param_t &log_param)
    {
      return f.accept(*this, log_param.m_line);
    }

    inline std::string function_name_element::visit
    (
     format &f, 
     const log_param_t &log_param
    )
    {
      return f.accept(*this, log_param.m_func_name);
    }

    inline std::string pretty_function_name_element::visit
    (
     format &f, 
     const log_param_t &log_param
    )
    {
      return f.accept(*this, log_param.m_func_sig);
    }

    inline std::string scope_element::visit(format &f, 
                                            const log_param_t &log_param)
    {
      return f.accept(*this, log_param.m_scope);
    }

  } // !namespace logging

} // !namespace boost

//  Element global operators -------------------------------------------------//
inline boost::logging::element_list_t operator>>(
  boost::logging::log_element &lhs, 
  boost::logging::log_element &rhs)
{ 
  boost::logging::element_list_t l;
  l.push_back(boost::shared_ptr<boost::logging::log_element> 
                (&lhs, boost::logging::null_deleter()));
  l.push_back(boost::shared_ptr<boost::logging::log_element> 
                (&rhs, boost::logging::null_deleter())); 
  return l;
}

inline boost::logging::element_list_t operator>>(
  boost::logging::element_list_t lhs, 
  boost::logging::log_element &rhs)
{ 
  lhs.push_back(boost::shared_ptr<boost::logging::log_element> 
                (&rhs, boost::logging::null_deleter())); 
  return lhs; 
}

inline boost::logging::element_list_t operator>>(
  const std::string &s, 
  boost::logging::log_element &rhs)
{
  boost::logging::element_list_t l;
  boost::shared_ptr<boost::logging::literal_element> 
    p(new boost::logging::literal_element(s));
  l.push_back(p);
  l.push_back(boost::shared_ptr<boost::logging::log_element> 
                (&rhs, boost::logging::null_deleter())); 
  return l;
}

inline boost::logging::element_list_t operator>>(
  boost::logging::element_list_t lhs, 
  const std::string &s)
{ 
  boost::shared_ptr<boost::logging::literal_element> 
    p(new boost::logging::literal_element(s));
  lhs.push_back(p);
  return lhs;
}

inline
void boost::logging::logger::unformatted_trace(log_param_t &log_param)
{
#if defined(BOOST_HAS_THREADS)
  boost::mutex::scoped_lock scoped_lock(m_mutex);
#endif // BOOST_HAS_THREADS

  log_param.m_scope = &m_scope_stack; // Attach the scope stack here
  sink_format_assoc_list_t::iterator 
    s_it = m_sink_format_assoc.begin();
  for (; s_it != m_sink_format_assoc.end(); ++s_it)
    {
      boost::logging::format f(boost::logging::trace);
      get<SINK>(*s_it).consume_trace(f, log_param);
    }
}

inline
std::string boost::logging::scope_element::to_string(scope_stack_t *s)
{ 
  scope_stack_t::iterator it = s->begin();
  std::string scope_str;
  for (; it != s->end(); ++it)
  {
    if (it == s->begin())
      scope_str += (*it)->get_id();
    else
      scope_str += " -> " + (*it)->get_id();
  }
  
  return scope_str;
};


#endif  // !BOOST_LOGGING_HPP
