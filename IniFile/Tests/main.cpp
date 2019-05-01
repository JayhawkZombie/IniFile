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

#define CATCH_CONFIG_RUNNER
#include <IniFile/catch.hpp>

#include "TestConfig.h"

std::string TestFileDirectory = "TestFiles/";

int main(int argc, char **argv)
{
  Catch::Session Session;
  Session.configData().reporterName = "console";

  using namespace Catch::clara;

  auto CommandLine = Session.cli()
    | Opt(TestFileDirectory, "test file directory")
      ["--testdir"]
      ("path to directory containing ini files to use for testing");

  Session.cli(CommandLine);

  int Ret = Session.applyCommandLine(argc, argv);
  if (TestFileDirectory.empty())
  {
    std::cerr << "Path to directory containing test files cannot be empty" << std::endl;
    return -1;
  }

  // Make sure it has a '/' at the end so we don't get the folder name mashed into the file names
  if (TestFileDirectory.back() != '/')
  {
    TestFileDirectory += '/';
  }

  if (Ret != 0)
  {
    std::cerr << "Error applying command line arguments: error code \"" << Ret << "\"" << std::endl;
    return Ret;
  }

  int NumFailed = Session.run();

  return NumFailed;
}
