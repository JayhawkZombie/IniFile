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

TEST_CASE("A simlpe .ini file SimpleMultipleSections.ini with multiple sections can be parsed", "[file][parse][simple][sections]")
{
  IniFile Ini(TestFileDirectory + "SimpleMultipleSections.ini");

  auto Sec1_Opt = Ini.TryGetSection("Section1");
  auto Sec2_Opt = Ini.TryGetSection("Section2");
  auto Sec3_Opt = Ini.TryGetSection("Section3");

  SECTION("All sections are recognized")
  {
    REQUIRE(Sec1_Opt.has_value());
    REQUIRE(Sec2_Opt.has_value());
    REQUIRE(Sec3_Opt.has_value());
  }

  SECTION("Section1's entries are parsed into the section \"Section1\"")
  {
    IniSection &Sec1 = Sec1_Opt.value().get();

    REQUIRE(Sec1.GetNumEntries() == 3);

    auto Key1Opt = Sec1.TryGetEntry("Key1");
    auto Key2Opt = Sec1.TryGetEntry("Key2");
    auto Key3Opt = Sec1.TryGetEntry("Key3");

    REQUIRE(Key1Opt.has_value());
    REQUIRE(Key2Opt.has_value());
    REQUIRE(Key3Opt.has_value());

    auto &Entry_Key1 = Key1Opt.value().get();
    auto &Entry_Key2 = Key2Opt.value().get();
    auto &Entry_Key3 = Key3Opt.value().get();

    REQUIRE(Entry_Key1.GetKey() == "Key1");
    REQUIRE(Entry_Key1.GetValueCount() == 1);
    REQUIRE(Entry_Key1.TryGetValue().value_or("bad") == R"("someval")");

    REQUIRE(Entry_Key2.GetKey() == "Key2");
    REQUIRE(Entry_Key2.GetValueCount() == 1);
    REQUIRE(Entry_Key2.TryGetValue().value_or("bad") == R"((1324,234,"fjs"))");

    REQUIRE(Entry_Key3.GetKey() == "Key3");
    REQUIRE(Entry_Key3.GetValueCount() == 1);
    REQUIRE(Entry_Key3.TryGetValue().value_or("bad") == R"(kdsjfh sk)");
  }

  SECTION("Section2's entries are parsed into the section \"Section2\"")
  {
    IniSection &Sec2 = Sec2_Opt.value().get();

    REQUIRE(Sec2.GetNumEntries() == 2);

    auto Key1Opt = Sec2.TryGetEntry("S2Key1");
    auto Key2Opt = Sec2.TryGetEntry("Key2");

    REQUIRE(Key1Opt.has_value());
    REQUIRE(Key2Opt.has_value());

    auto &Entry_Key1 = Key1Opt.value().get();
    auto &Entry_Key2 = Key2Opt.value().get();

    REQUIRE(Entry_Key1.GetKey() == "S2Key1");
    REQUIRE(Entry_Key1.GetValueCount() == 1);
    REQUIRE(Entry_Key1.TryGetValue().value_or("bad") == "something");

    REQUIRE(Entry_Key2.GetKey() == "Key2");
    REQUIRE(Entry_Key2.GetValueCount() == 1);
    REQUIRE(Entry_Key2.TryGetValue().value_or("bad") == "23948.23432");
  }

  SECTION("Section3's entries are parsed into the section \"Section3\"")
  {
    IniSection &Sec3 = Sec3_Opt.value().get();

    REQUIRE(Sec3.GetNumEntries() == 1);

    auto EntryOpt = Sec3.TryGetEntry("Key");
    REQUIRE(EntryOpt.has_value());

    auto &Entry = EntryOpt.value().get();
    REQUIRE(Entry.GetKey() == "Key");
    REQUIRE(Entry.GetValueCount() == 1);
    REQUIRE(Entry.TryGetValue().value_or("bad") == "LastEntry");
  }
}
