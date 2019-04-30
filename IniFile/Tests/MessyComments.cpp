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

TEST_CASE("An ini file with multiple sections with many messy comments can be parsed", "[file][parse][sections][comments][messy]")
{

  IniFile Ini("Tests/MessyComments.ini");

  auto &Sec1_Opt = Ini.TryGetSection("Section1");
  auto &Sec2_Opt = Ini.TryGetSection("Section2");
  auto &Sec3_Opt = Ini.TryGetSection("Section3");

  SECTION("All sections are recognized")
  {
    REQUIRE(Sec1_Opt.has_value());
    REQUIRE(Sec2_Opt.has_value());
    REQUIRE(Sec3_Opt.has_value());
  }

  SECTION("Section1's entries with and without comments at the end of the entry are parsed")
  {
    IniSection &Sec1 = Sec1_Opt.value().get();
    REQUIRE(Sec1.GetNumEntries() == 3);

    auto Key1_Opt = Sec1.TryGetEntry("Key1");
    auto Key2_Opt = Sec1.TryGetEntry("Key2");
    auto Key3_Opt = Sec1.TryGetEntry("Key3");

    REQUIRE(Key1_Opt.has_value());
    REQUIRE(Key2_Opt.has_value());
    REQUIRE(Key3_Opt.has_value());

    auto &Key1_Entry = Key1_Opt.value().get();
    auto &Key2_Entry = Key2_Opt.value().get();
    auto &Key3_Entry = Key3_Opt.value().get();

    REQUIRE(Key1_Entry.GetKey() == "Key1");
    REQUIRE(Key1_Entry.GetValueCount() == 1);
    REQUIRE(Key1_Entry.TryGetValue().value_or("bad") == "Val1");

    REQUIRE(Key2_Entry.GetKey() == "Key2");
    REQUIRE(Key2_Entry.GetValueCount() == 1);
    REQUIRE(Key2_Entry.TryGetValue().value_or("bad") == "Val2 ");

    REQUIRE(Key3_Entry.GetKey() == "Key3");
    REQUIRE(Key3_Entry.GetValueCount() == 1);
    REQUIRE(Key3_Entry.TryGetValue().value_or("bad") == R"("whatever" )");
  }

  SECTION("Section2's entries with and without comments are parsed")
  {
    IniSection &Sec2 = Sec2_Opt.value().get();
    REQUIRE(Sec2.GetNumEntries() == 2);

    auto Key1_Opt = Sec2.TryGetEntry("Key1");
    auto Key2_Opt = Sec2.TryGetEntry("Key2");

    REQUIRE(Key1_Opt.has_value());
    REQUIRE(Key2_Opt.has_value());

    auto &Key1_Entry = Key1_Opt.value().get();
    auto &Key2_Entry = Key2_Opt.value().get();

    REQUIRE(Key1_Entry.GetKey() == "Key1");
    REQUIRE(Key1_Entry.GetValueCount() == 1);
    REQUIRE(Key1_Entry.TryGetValue().value_or("bad") == R"("Value1")" );

    REQUIRE(Key2_Entry.GetKey() == "Key2");
    REQUIRE(Key2_Entry.GetValueCount() == 1);
    REQUIRE(Key2_Entry.TryGetValue().value_or("bad") == R"(("Value",2,WithComments) )");
  }

  SECTION("Section3's entry can be parsed")
  {
    IniSection &Sec3 = Sec3_Opt.value().get();
    REQUIRE(Sec3.GetNumEntries() == 1);

    auto Key_Opt = Sec3.TryGetEntry("Key1");
    REQUIRE(Key_Opt.has_value());

    auto &Key_Entry = Key_Opt.value().get();
    REQUIRE(Key_Entry.GetKey() == "Key1");
    REQUIRE(Key_Entry.GetValueCount() == 1);
    REQUIRE(Key_Entry.TryGetValue().value_or("bad") == "whatever");
  }

}
