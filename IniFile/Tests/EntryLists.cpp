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
#include <IniFile/Tests/TestConfig.h>

TEST_CASE("An ini file with entries that append to lists in values for single keys EntryLists.ini can be parsed", "[file][parse][sections][lists]")
{

  IniFile Ini(TestFileDirectory + "EntryLists.ini");

  auto Sec1_Opt = Ini.TryGetSection("Section1");
  auto Sec2_Opt = Ini.TryGetSection("Section2");
  auto Sec3_Opt = Ini.TryGetSection("Section3");

  SECTION("All sections are recognized")
  {
    REQUIRE(Sec1_Opt.has_value());
    REQUIRE(Sec2_Opt.has_value());
    REQUIRE(Sec3_Opt.has_value());
  }

  SECTION("Section1's entries with single values and lists of values on contiguous entries are parsed")
  {
    IniSection &Sec1 = Sec1_Opt.value().get();
    REQUIRE(Sec1.GetNumEntries() == 2);

    auto NoList_Opt = Sec1.TryGetEntry("NoList");
    auto List_Opt = Sec1.TryGetEntry("List");

    REQUIRE(NoList_Opt.has_value());
    REQUIRE(List_Opt.has_value());

    auto &NoList_Entry = NoList_Opt.value().get();
    auto &List_Entry = List_Opt.value().get();

    REQUIRE(NoList_Entry.GetKey() == "NoList");
    REQUIRE(NoList_Entry.GetValueCount() == 1);
    REQUIRE(NoList_Entry.TryGetValue().value_or("bad") == R"("SingleValue")");

    REQUIRE(List_Entry.GetKey() == "List");
    REQUIRE(List_Entry.GetValueCount() == 3);

    auto &List_Entry_Values = List_Entry.GetValues();
    REQUIRE(List_Entry_Values.size() == 3);
    REQUIRE(List_Entry_Values[0] == "FirstVal");
    REQUIRE(List_Entry_Values[1] == R"("SecondVal")");
    REQUIRE(List_Entry_Values[2] == R"(("Third",Value))");
  }

  SECTION("Section2's entries with single values and no lists of values are parsed")
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
    REQUIRE(Key1_Entry.TryGetValue().value_or("bad") == "Something");

    REQUIRE(Key2_Entry.GetKey() == "Key2");
    REQUIRE(Key2_Entry.GetValueCount() == 1);
    REQUIRE(Key2_Entry.TryGetValue().value_or("bad") == R"("SomethingElse")");
  }

  SECTION("Section3's entries with single values and lists of values on non-contiguous entries are parsed")
  {
    IniSection &Sec3 = Sec3_Opt.value().get();
    REQUIRE(Sec3.GetNumEntries() == 2);

    auto List1_Opt = Sec3.TryGetEntry("List1");
    auto List2_Opt = Sec3.TryGetEntry("List2");

    REQUIRE(List1_Opt.has_value());
    REQUIRE(List2_Opt.has_value());

    auto &List1_Entry = List1_Opt.value().get();
    auto &List2_Entry = List2_Opt.value().get();

    REQUIRE(List1_Entry.GetKey() == "List1");
    REQUIRE(List1_Entry.GetValueCount() == 3);

    auto &List1_Entry_Values = List1_Entry.GetValues();
    REQUIRE(List1_Entry_Values.size() == 3);
    REQUIRE(List1_Entry_Values[0] == "Val1");
    REQUIRE(List1_Entry_Values[1] == R"("Val2")");
    REQUIRE(List1_Entry_Values[2] == R"("ExtraValue")");

    auto &List2_Entry_Values = List2_Entry.GetValues();
    REQUIRE(List2_Entry_Values.size() == 4);
    REQUIRE(List2_Entry_Values[0] == "List2Val1");
    REQUIRE(List2_Entry_Values[1] == R"("List2Val2")");
    REQUIRE(List2_Entry_Values[2] == R"(("List2","Val",2))");
    REQUIRE(List2_Entry_Values[3] == R"(("Another",Extra,"Value"))");
  }
}
