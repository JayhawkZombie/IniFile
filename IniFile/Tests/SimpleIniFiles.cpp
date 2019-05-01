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

SCENARIO("Simple .ini files can be parsed", "[file][parse][simple]")
{
  std::string IniFileName;

  GIVEN("A basic .ini file Simple.ini")
  {
    IniFileName = TestFileDirectory + "Simple.ini";

    WHEN("An IniFile parses Simple.ini")
    {
      IniFile SimpleIniFile(IniFileName);
      auto    SimpleSection = SimpleIniFile.TryGetSection("Simple");

      THEN("The section header \"[Simple]\" is parsed")
      {
        REQUIRE(SimpleIniFile.HasSection("Simple"));
        REQUIRE(SimpleSection.has_value());
      }

      AND_THEN("The key \"SimpleKey\" is parsed")
      {
        IniSection &Section = SimpleSection.value().get();
        REQUIRE(Section.HasEntry("SimpleKey"));

        auto SimpleKeyEntryOpt = Section.TryGetEntry("SimpleKey");
        REQUIRE(SimpleKeyEntryOpt.has_value());

        auto SimpleKeyEntry = SimpleKeyEntryOpt.value().get();
        REQUIRE(SimpleKeyEntry.GetKey() == "SimpleKey");
        REQUIRE(SimpleKeyEntry.GetValueCount() == 1);

        auto SimpleKeyValueOpt = SimpleKeyEntry.TryGetValue();
        REQUIRE(SimpleKeyValueOpt.has_value());
        REQUIRE(SimpleKeyValueOpt.value() == "SomeValue");
      }

      AND_THEN("The key \"OtherKey\" is parsed")
      {
        IniSection &Section = SimpleSection.value().get();
        REQUIRE(Section.HasEntry("OtherKey"));

        auto OtherKeyEntryOpt = Section.TryGetEntry("OtherKey");
        REQUIRE(OtherKeyEntryOpt.has_value());

        auto OtherKeyEntry = OtherKeyEntryOpt.value().get();
        REQUIRE(OtherKeyEntry.GetKey() == "OtherKey");
        REQUIRE(OtherKeyEntry.GetValueCount() == 1);

        auto OtherKeyValueOpt = OtherKeyEntry.TryGetValue();
        REQUIRE(OtherKeyValueOpt.has_value());
        REQUIRE(OtherKeyValueOpt.value() == "(1,222,\"string\")");
      }
    }
  }
}
