#ifndef LLARP_ENCCRYPTED_HPP
#define LLARP_ENCCRYPTED_HPP

#include <link_layer.hpp>
#include <util/aligned.hpp>
#include <util/bencode.h>
#include <util/buffer.h>
#include <util/mem.hpp>

#include <vector>
#include <stdexcept>

namespace llarp
{
  /// encrypted buffer base type
  template < size_t bufsz = MAX_LINK_MSG_SIZE >
  struct Encrypted
  {
    Encrypted(Encrypted&& other)
    {
      _sz  = std::move(other._sz);
      _buf = std::move(other._buf);
      UpdateBuffer();
    }

    Encrypted(const Encrypted& other) : Encrypted(other.data(), other.size())
    {
      UpdateBuffer();
    }

    Encrypted()
    {
      Clear();
    }

    void
    Clear()
    {
      _sz = 0;
      UpdateBuffer();
    }

    Encrypted(const byte_t* buf, size_t sz)
    {
      if(sz <= bufsz)
      {
        _sz = sz;
        if(buf)
          memcpy(_buf.data(), buf, sz);
        else
          _buf.Zero();
      }
      else
        _sz = 0;
      UpdateBuffer();
    }

    Encrypted(size_t sz) : Encrypted(nullptr, sz)
    {
    }

    ~Encrypted()
    {
    }

    bool
    BEncode(llarp_buffer_t* buf) const
    {
      return bencode_write_bytestring(buf, data(), _sz);
    }

    bool
    operator==(const Encrypted& other) const
    {
      return _sz == other._sz && memcmp(data(), other.data(), _sz) == 0;
    }

    bool
    operator!=(const Encrypted& other) const
    {
      return !(*this == other);
    }

    Encrypted&
    operator=(const Encrypted& other)
    {
      return (*this) = other.Buffer();
    }

    Encrypted&
    operator=(const llarp_buffer_t& buf)
    {
      if(buf.sz <= _buf.size())
      {
        _sz = buf.sz;
        memcpy(_buf.data(), buf.base, _sz);
      }
      UpdateBuffer();
      return *this;
    }

    void
    Fill(byte_t fill)
    {
      size_t idx = 0;
      while(idx < _sz)
        _buf[idx++] = fill;
    }

    void
    Randomize()
    {
      if(_sz)
        randombytes(_buf.data(), _sz);
    }

    bool
    BDecode(llarp_buffer_t* buf)
    {
      llarp_buffer_t strbuf;
      if(!bencode_read_string(buf, &strbuf))
        return false;
      if(strbuf.sz > sizeof(_buf))
        return false;
      _sz = strbuf.sz;
      if(_sz)
        memcpy(_buf.data(), strbuf.base, _sz);
      UpdateBuffer();
      return true;
    }

    llarp_buffer_t*
    Buffer()
    {
      return &m_Buffer;
    }

    llarp_buffer_t
    Buffer() const
    {
      return m_Buffer;
    }

    size_t
    size()
    {
      return _sz;
    }

    size_t
    size() const
    {
      return _sz;
    }

    byte_t*
    data()
    {
      return _buf.data();
    }

    const byte_t*
    data() const
    {
      return _buf.data();
    }

   protected:
    void
    UpdateBuffer()
    {
      m_Buffer.base = _buf.data();
      m_Buffer.cur  = _buf.data();
      m_Buffer.sz   = _sz;
    }
    AlignedBuffer< bufsz > _buf;
    size_t _sz;
    llarp_buffer_t m_Buffer;
  };  // namespace llarp
}  // namespace llarp

#endif
