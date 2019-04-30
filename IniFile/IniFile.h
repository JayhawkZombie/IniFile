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

#pragma once

#include <string>
#include <deque>
#include <fstream>
#include <unordered_map>
#include <utility>
#include <tuple>
#include <iterator>
#include <type_traits>
#include <algorithm>
#include <optional>
#include <initializer_list>
#include <regex>

struct IniEntry
{
  IniEntry(const std::string &Key);
  explicit IniEntry(const std::string &Key, std::size_t NumVals, const std::string &FillValue);
  IniEntry(const std::string &Key, const std::string &Value);
  IniEntry(const std::string &Key, std::initializer_list<std::string> InitValues);

  template<class... Vals>
  IniEntry(const std::string &Key, const std::string &Val1, Vals... OtherVals);

  IniEntry(const IniEntry &Other);
  IniEntry(IniEntry &&Other);

  IniEntry& operator =(const IniEntry &Other);
  IniEntry& operator =(IniEntry &&Other);

  inline friend bool IniEntry::operator<(const IniEntry &LHS, const IniEntry &RHS)
  {
    return (LHS.m_Key < RHS.m_Key);
  }

  inline friend bool IniEntry::operator>(const IniEntry &LHS, const IniEntry &RHS)
  {
    return (LHS.m_Key > RHS.m_Key);
  }

  inline friend bool IniEntry::operator==(const IniEntry &LHS, const IniEntry &RHS)
  {
    return (LHS.m_Key == RHS.m_Key);
  }

  std::size_t GetValueCount() const;
  std::optional<std::string> TryGetValue() const;
  const std::deque<std::string>& GetValues() const;
  std::optional<std::string> operator[](std::size_t ValIndex) const;
  void AddValue(const std::string &Val);

  template<class ...Vals>
  void AddValue(const std::string &Val, Vals... vals);

private:

  void AddValueAtFront(const std::string &Val);

  template<class ...Vals>
  IniEntry::IniEntry(const std::string &Key, const std::string &Val1, Vals... OtherVals)
  {
    static_assert(std::conjunction<std::is_same<std::string, Vals>...>::value || std::conjunction<std::is_same<const char *, Vals>...>::value, "IniEntry values must of a string type");

    m_Key = Key;
    AddValueAtFront(Val1, std::forward<Vals>(OtherVals)...);
  }

  template<class ...Vals>
  void IniEntry::AddValue(const std::string &Val, Vals... vals)
  {
    m_ValueList.emplace_back(Val);
    AddValue(std::forward<Vals>(vals)...);
  }

  template<class ...Vals>
  void IniEntry::AddValueAtFront(const std::string &Val, Vals... vals)
  {
    m_ValueList.emplace_front(Val);
    AddValueAtFront(std::forward<Vals>(vals)...);
  }

  template<class ...Vals>
  void AddValueAtFront(const std::string &Val, Vals... vals);

  using value_const_iterator_t = typename std::deque<std::string>::const_iterator;
  using value_const_reverse_iterator_t = typename std::deque<std::string>::const_reverse_iterator;
  using value_iterator_t = typename std::deque<std::string>::iterator;
  using value_reverse_iterator_t = typename std::deque<std::string>::reverse_iterator;

public:

  value_const_iterator_t cbegin() const;
  value_const_iterator_t cend() const;
  value_iterator_t begin();
  value_iterator_t end();
  value_reverse_iterator_t rbegin();
  value_reverse_iterator_t rend();
  value_const_reverse_iterator_t begin() const;
  value_const_reverse_iterator_t end() const;
  std::tuple<bool, std::size_t> RemoveValue(std::size_t ValIndex);
  void Swap(IniEntry &Other);
  void Clear();
  const std::string& GetKey() const;

private:

  std::deque<std::string> m_ValueList;
  std::string m_Key{ "" };

};

class IniSection
{
public:

  using entry_iterator_t = typename std::unordered_map<std::string, IniEntry>::iterator;
  using entry_const_iterator_t = typename std::unordered_map<std::string, IniEntry>::const_iterator;

  entry_iterator_t begin()
  {
    return m_Entries.begin();
  }

  entry_iterator_t end()
  {
    return m_Entries.end();
  }

  entry_const_iterator_t cbegin() const
  {
    return m_Entries.cbegin();
  }

  entry_const_iterator_t cend() const
  {
    return m_Entries.cend();
  }

  IniSection(const IniSection &Other)
    : m_SectionName(Other.m_SectionName)
    , m_Entries(Other.m_Entries)
  { }

  IniSection(IniSection &&Other)
    : m_SectionName(std::move(Other.m_SectionName))
    , m_Entries(std::move(Other.m_Entries))
  { }

  IniSection& operator =(const IniSection &Other)
  {
    m_SectionName = Other.m_SectionName;
    m_Entries = Other.m_Entries;
    return *this;
  }

  IniSection& operator =(IniSection &&Other)
  {
    m_SectionName = std::move(Other.m_SectionName);
    m_Entries = std::move(Other.m_Entries);
    return *this;
  }

  IniSection(const std::string &SectionName)
    : m_SectionName(SectionName)
  { }

  bool HasEntry(const std::string &Key)
  {
    return (m_Entries.find(Key) != m_Entries.cend());
  }

  std::optional<std::reference_wrapper<IniEntry>> TryGetEntry(const std::string &Key)
  {
    entry_iterator_t it = m_Entries.find(Key);

    if (it == m_Entries.end())
      return std::nullopt;

    return std::make_optional(std::reference_wrapper<IniEntry>((*it).second));
  }

  IniEntry& CreateEntry(const std::string &Key)
  {
    auto res = m_Entries.emplace(Key, IniEntry(Key));
    return m_Entries.at(Key);
  }

  void AddEntry(const IniEntry &Entry)
  {
    m_Entries.emplace(Entry.GetKey(), Entry);
  }

  std::size_t GetNumEntries() const
  {
    return m_Entries.size();
  }

  const std::string& GetName() const
  {
    return m_SectionName;
  }

  void RemoveEntry(const std::string &Key)
  {
    auto it = m_Entries.find(Key);
    if (it != m_Entries.end())
    {
      m_Entries.erase(it);
    }
  }

  IniEntry& operator[](const std::string &Key)
  {
    auto it = m_Entries.find(Key);
    if (it == m_Entries.end())
      m_Entries.emplace(Key, Key);

    return m_Entries.at(Key);
  }

  void Clear()
  {
    m_Entries.clear();
  }

private:

  std::string m_SectionName;
  std::unordered_map<std::string, IniEntry> m_Entries;

};

class IniFile
{
public:

  IniFile(const std::string &FileName)
    : m_FileName(FileName)
  {
    if (!FileName.empty() && !ReadFile(FileName))
      throw std::runtime_error("Failed to read ini file");
  }

  bool ReadFile(const std::string &FileName)
  {
    std::ifstream in(FileName, std::ios_base::in);
    return ParseFile(in);
  }

  bool HasSection(const std::string &SectionName) const
  {
    return (m_Sections.find(SectionName) != m_Sections.end());
  }

  std::optional<std::reference_wrapper<IniSection>> TryGetSection(const std::string &SectionName)
  {
    auto it = m_Sections.find(SectionName);
    if (it == m_Sections.end())
      return std::nullopt;

    return std::make_optional(std::reference_wrapper<IniSection>(m_Sections.at(SectionName)));
  }

private:

  using section_iterator_t = typename std::unordered_map<std::string, IniSection>::iterator;
  using section_const_iterator_t = typename std::unordered_map<std::string, IniSection>::const_iterator;

public:

  section_iterator_t begin()
  {
    return m_Sections.begin();
  }

  section_iterator_t end()
  {
    return m_Sections.end();
  }

  section_const_iterator_t cbegin() const
  {
    return m_Sections.cbegin();
  }

  section_const_iterator_t cend() const
  {
    return m_Sections.cend();
  }

  const std::string& GetFileName() const
  {
    return m_FileName;
  }

private:

  bool ParseFile(std::ifstream &InFile)
  {
    if (!InFile)
      return false;

    std::string CurrentLine;

    while (InFile && std::getline(InFile, CurrentLine))
    {
      if (CurrentLine.empty() || CurrentLine.front() == '\n' || CurrentLine.front() == '#' || CurrentLine.front() == ';')
        continue;

      std::smatch Match;
      if (std::regex_search(CurrentLine, Match, SectionHeaderRegex))
      {
        IniSection Section(Match[1].str());
        ParseSection(Section, InFile);
        m_Sections.emplace(Section.GetName(), Section);
      }
    }

    return true;
  }

  void ParseSection(IniSection &Section, std::ifstream &InFile)
  {
    std::string CurrentLine;
    std::smatch Match;

    while (InFile && std::getline(InFile, CurrentLine) && !CurrentLine.empty() && (CurrentLine.front() != '\n'))
    {
      if (std::regex_search(CurrentLine, Match, EntryRegex))
      {
        if (CurrentLine.front() == '+')
        {
          Section[Match[1]].AddValue(Match[2]);
        }
        else
        {
          Section.CreateEntry(Match[1]).AddValue(Match[2]);
        }
      }
    }
  }

  std::unordered_map<std::string, IniSection> m_Sections;
  std::string m_FileName;

  static const std::regex SectionHeaderRegex;
  static const std::regex EntryRegex;

};
