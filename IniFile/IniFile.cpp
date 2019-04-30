////////////////////////////////////////////////////////////
//
// MIT License
//
// Copyright(c) 2018 Kurt Slagle - kurt_slagle@yahoo.com
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// The origin of this software must not be misrepresented; you must not claim
// that you wrote the original software.If you use this software in a product,
// an acknowledgment of the software used is required.
//
////////////////////////////////////////////////////////////

#include "IniFile.h"

IniEntry::value_reverse_iterator_t IniEntry::rend()
{
  return m_ValueList.rend();
}

IniEntry& IniEntry::operator=(IniEntry &&Other)
{
  m_Key = std::move(Other.m_Key);
  m_ValueList = std::move(Other.m_ValueList);
  return *this;
}

IniEntry& IniEntry::operator=(const IniEntry &Other)
{
  m_Key = Other.m_Key;
  m_ValueList = Other.m_ValueList;
  return *this;
}

IniEntry::IniEntry(IniEntry &&Other) : m_Key(std::move(Other.m_Key))
, m_ValueList(std::move(Other.m_ValueList))
{

}

IniEntry::IniEntry(const IniEntry &Other) : m_Key(Other.m_Key)
, m_ValueList(Other.m_ValueList)
{

}

IniEntry::IniEntry(const std::string &Key, std::initializer_list<std::string> InitValues) : m_Key(Key)
, m_ValueList(InitValues)
{

}

IniEntry::IniEntry(const std::string &Key, const std::string &Value) : m_Key(Key)
, m_ValueList(1, Value)
{

}

IniEntry::IniEntry(const std::string &Key, std::size_t NumVals, const std::string &FillValue) : m_Key(Key)
, m_ValueList(NumVals, FillValue)
{

}

IniEntry::IniEntry(const std::string &Key) : m_Key(Key)
{

}

std::size_t IniEntry::GetValueCount() const
{
  return m_ValueList.size();
}

std::optional<std::string> IniEntry::TryGetValue() const
{
  if (m_ValueList.empty())
    return std::nullopt;

  return std::make_optional(m_ValueList.front());
}

const std::deque<std::string>& IniEntry::GetValues() const
{
  return m_ValueList;
}

std::optional<std::string> IniEntry::operator[](std::size_t ValIndex) const
{
  if (ValIndex >= m_ValueList.size())
    return std::nullopt;

  return std::make_optional(m_ValueList.at(ValIndex));
}

void IniEntry::AddValue(const std::string &Val)
{
  m_ValueList.emplace_back(Val);
}

void IniEntry::AddValueAtFront(const std::string &Val)
{
  m_ValueList.emplace_front(Val);
}

IniEntry::value_const_iterator_t IniEntry::cbegin() const
{
  return m_ValueList.cbegin();
}

IniEntry::value_const_iterator_t IniEntry::cend() const
{
  return m_ValueList.cend();
}

IniEntry::value_const_reverse_iterator_t IniEntry::begin() const
{
  return m_ValueList.crbegin();
}

IniEntry::value_iterator_t IniEntry::begin()
{
  return m_ValueList.begin();
}

IniEntry::value_const_reverse_iterator_t IniEntry::end() const
{
  return m_ValueList.crend();
}

IniEntry::value_iterator_t IniEntry::end()
{
  return m_ValueList.end();
}

IniEntry::value_reverse_iterator_t IniEntry::rbegin()
{
  return m_ValueList.rbegin();
}

std::tuple<bool, std::size_t> IniEntry::RemoveValue(std::size_t ValIndex)
{
  if (ValIndex >= m_ValueList.size() || m_ValueList.empty())
    return std::make_tuple(false, m_ValueList.size());

  m_ValueList.erase(m_ValueList.cbegin() + ValIndex);
  return std::make_tuple(true, m_ValueList.size());
}

void IniEntry::Swap(IniEntry &Other)
{
  m_Key.swap(Other.m_Key);
  m_ValueList.swap(Other.m_ValueList);
}

void IniEntry::Clear()
{
  m_ValueList.clear();
}

const std::string& IniEntry::GetKey() const
{
  return m_Key;
}

const std::regex IniFile::SectionHeaderRegex = std::regex(R"(\[(.+)\])");
const std::regex IniFile::EntryRegex = std::regex(R"(\+?(\w+)=([^#;\n\r]+)*)");
