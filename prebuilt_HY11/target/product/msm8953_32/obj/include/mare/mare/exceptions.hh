// --~--~--~--~----~--~--~--~----~--~--~--~----~--~--~--~----~--~--~--~--
// Copyright 2013-2015 Qualcomm Technologies, Inc.
// All rights reserved.
// Confidential and Proprietary – Qualcomm Technologies, Inc.
// --~--~--~--~----~--~--~--~----~--~--~--~----~--~--~--~----~--~--~--~--
#pragma once
#include <exception>
#include <stdint.h>
#include <sstream>
#include <typeinfo>
#include <vector>

#include <mare/internal/compat/compilercompat.h>
#include <mare/internal/util/strprintf.hh>

namespace mare {

class mare_exception : public std::exception
{
public:

  virtual ~mare_exception() MARE_NOEXCEPT {}

  virtual const char* what() const MARE_NOEXCEPT = 0;
};

class error_exception : public mare_exception
{

public:
  error_exception(std::string msg, const char* filename, int lineno, const char* fname);

  ~error_exception() MARE_NOEXCEPT {};

  virtual const char* message() const MARE_NOEXCEPT { return _message.c_str(); }
  virtual const char* what() const MARE_NOEXCEPT { return _long_message.c_str(); }
  virtual const char* file() const MARE_NOEXCEPT { return _file.c_str(); }
  virtual int         line() const MARE_NOEXCEPT { return _line; }
  virtual const char* function() const MARE_NOEXCEPT { return _function.c_str(); }
  virtual const char* type() const MARE_NOEXCEPT { return _classname.c_str(); }

private:

  std::string _message;
  std::string _long_message;
  std::string _file;
  int _line;
  std::string _function;
  std::string _classname;
};

class api_exception : public error_exception
{
public:
  api_exception(std::string msg,
                const char* filename, int lineno, const char* funcname)
    : error_exception(msg, filename, lineno, funcname)
  { }
};

#define MARE_API_THROW(cond, format, ...) do {                         \
    if(!(cond)) { throw                                                 \
        ::mare::api_exception(::mare::internal::                        \
                              strprintf(format, ## __VA_ARGS__),        \
                              __FILE__, __LINE__, __FUNCTION__);        \
    } } while(false)

#define MARE_API_THROW_CUSTOM(cond, exception, format, ...) do {        \
    if(!(cond)) { throw                                                 \
        exception(::mare::internal::                                    \
                  strprintf(format, ## __VA_ARGS__));                   \
    } } while(false)

class tls_exception : public error_exception
{
public:
  tls_exception(std::string msg,
                const char* filename, int lineno, const char* funcname)
    : error_exception(msg, filename, lineno, funcname)
  {}
};

class abort_task_exception: public mare_exception
{
public:
  explicit abort_task_exception(std::string msg="aborted task")
    :_msg(msg){
  }

  virtual ~abort_task_exception() MARE_NOEXCEPT {}

  virtual const char* what() const MARE_NOEXCEPT {
    return _msg.c_str();
  }

private:
  std::string _msg;
};

class canceled_exception : public mare_exception
{
public:
  canceled_exception() {}
  virtual ~canceled_exception() MARE_NOEXCEPT {}
  MARE_DEFAULT_METHOD(canceled_exception(const canceled_exception&));
  MARE_DEFAULT_METHOD(canceled_exception(canceled_exception&& ));
  MARE_DEFAULT_METHOD(canceled_exception& operator=(const canceled_exception&) &);
  MARE_DEFAULT_METHOD(canceled_exception& operator=(canceled_exception&&) &);
  virtual const char* what() const MARE_NOEXCEPT {
    return "canceled exception";
  }
};

class aggregate_exception : public mare_exception
{
public:
  explicit aggregate_exception(std::vector<std::exception_ptr>* exceptions)
    : _exceptions(exceptions), _count(exceptions->size())  {}

  aggregate_exception(const aggregate_exception& other)
    : mare_exception(other),
      _exceptions(other._exceptions),
      _count(other._count) {}

  aggregate_exception(aggregate_exception&& other)
    : mare_exception(std::move(other)),
      _exceptions(other._exceptions),
      _count(other._count) {}

  aggregate_exception& operator=(const aggregate_exception& other) {
    mare_exception::operator=(other);
    _exceptions = other._exceptions;
    _count = other._count;
    return *this;
  }

  aggregate_exception& operator=(aggregate_exception&& other) {
    mare_exception::operator=(std::move(other));
    _exceptions = std::move(other._exceptions);
    _count = other._count;
    return *this;
  }

  virtual ~aggregate_exception() MARE_NOEXCEPT {}

  virtual const char* what() const MARE_NOEXCEPT {
    return "aggregate exception";
  }

  bool has_next() const {
    return _count > 0;
  }

  void next() {
    if (_count-- != 0) {
      std::rethrow_exception(_exceptions->at(_count));
    }
  }

private:
  std::vector<std::exception_ptr>* _exceptions;
  size_t _count;
};

class gpu_exception : public mare_exception
{
public:
  gpu_exception() {}
  virtual ~gpu_exception() MARE_NOEXCEPT {}
  MARE_DEFAULT_METHOD(gpu_exception(const gpu_exception&));
  MARE_DEFAULT_METHOD(gpu_exception(gpu_exception&& ));
  MARE_DEFAULT_METHOD(gpu_exception& operator=(const gpu_exception&) &);
  MARE_DEFAULT_METHOD(gpu_exception& operator=(gpu_exception&&) &);
  virtual const char* what() const MARE_NOEXCEPT {
    return "gpu exception";
  }
};

class hexagon_exception : public mare_exception
{
public:
  hexagon_exception() {}
  virtual ~hexagon_exception() MARE_NOEXCEPT {}
  MARE_DEFAULT_METHOD(hexagon_exception(const hexagon_exception&));
  MARE_DEFAULT_METHOD(hexagon_exception(hexagon_exception&& ));
  MARE_DEFAULT_METHOD(hexagon_exception& operator=(const hexagon_exception&) &);
  MARE_DEFAULT_METHOD(hexagon_exception& operator=(hexagon_exception&&) &);
  virtual const char* what() const MARE_NOEXCEPT {
    return "hexagon exception";
  }
};

};
