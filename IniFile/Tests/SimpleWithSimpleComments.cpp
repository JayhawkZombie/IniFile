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

#include <IniFile/catch.hpp>

#include <IniFile/IniFile.h>

/*
  SimpleWithSimpleComments.ini

  ini files with comments only on their own lines and not between any two entries in a section
  (ie all section's have contiguous entries with no comments between them)
*/

TEST_CASE("A simple ini file with simple comments and multiple sections SimpleWithSimpleComments.ini can be parsed", "[file][parse][simple][sections][comments]")
{
  IniFile Ini("Tests/SimpleWithSimpleComments.ini");

  auto Sec1_Opt = Ini.TryGetSection("Section1");
  auto Sec2_Opt = Ini.TryGetSection("Section2");

  SECTION("All sections are recognized")
  {
    REQUIRE(Sec1_Opt.has_value());
    REQUIRE(Sec2_Opt.has_value());
  }

  SECTION("Section1's entries are parsed into the section \"Section1\"")
  {
    IniSection &Sec1 = Sec1_Opt.value().get();
    REQUIRE(Sec1.GetNumEntries() == 2);

    auto Key1_Opt = Sec1.TryGetEntry("Key1");
    auto Key2_Opt = Sec1.TryGetEntry("key2");

    REQUIRE(Key1_Opt.has_value());
    REQUIRE(Key2_Opt.has_value());

    auto &Entry1 = Key1_Opt.value().get();
    auto &Entry2 = Key2_Opt.value().get();

    REQUIRE(Entry1.GetKey() == "Key1");
    REQUIRE(Entry1.GetValueCount() == 1);
    REQUIRE(Entry1.TryGetValue().value_or("bad") == "whatever");

    REQUIRE(Entry2.GetKey() == "key2");
    REQUIRE(Entry2.GetValueCount() == 1);
    REQUIRE(Entry2.TryGetValue().value_or("bad") == R"("something")");
  }

  SECTION("Section2's entries are parsed into the section \"Section2\"")
  {
    IniSection &Sec2 = Sec2_Opt.value().get();
    REQUIRE(Sec2.GetNumEntries() == 1);

    auto Entry_Opt = Sec2.TryGetEntry("SomeKey");

    REQUIRE(Entry_Opt.has_value());

    auto &Entry = Entry_Opt.value().get();
    
    REQUIRE(Entry.GetKey() == "SomeKey");
    REQUIRE(Entry.GetValueCount() == 1);
    REQUIRE(Entry.TryGetValue().value_or("bad") == R"((23904823,"asdals"))");
  }

}
