// SPDX-License-Identifier: BSD-3-Clause
#include "DBSuitabilityTestAccess.h"
#include <OpenMS/CONCEPT/ClassTest.h>
#include <OpenMS/CONCEPT/Exception.h>
#include <OpenMS/CONCEPT/LogStream.h>
#include <OpenMS/KERNEL/MSExperiment.h>
#include <OpenMS/SYSTEM/ExternalProcess.h>
#include <OpenMS/SYSTEM/File.h>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace
{
  class ScopedPath
  {
  public:
    explicit ScopedPath(const std::string& directory)
      : previous_(std::getenv("PATH") == nullptr ? "" : std::getenv("PATH"))
    {
#ifdef _WIN32
      const std::string value = directory + ";" + DBSUITABILITY_CORE_RUNTIME_DIR + ";" + previous_;
      _putenv_s("PATH", value.c_str());
#else
      const std::string value = directory + ":" + previous_;
      setenv("PATH", value.c_str(), 1);
#endif
    }
    ~ScopedPath()
    {
#ifdef _WIN32
      _putenv_s("PATH", previous_.c_str());
#else
      setenv("PATH", previous_.c_str(), 1);
#endif
    }
  private:
    std::string previous_;
  };

  struct ErrorCapture
  {
    std::ostringstream stream;
    ErrorCapture() { OpenMS::getGlobalLogError().insert(stream); }
    ~ErrorCapture() { OpenMS::getGlobalLogError().remove(stream); }
  };

  std::string readReceipt(const std::string& path)
  {
    std::ifstream input(std::filesystem::u8path(path));
    return std::string(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
  }
}

START_TEST(DBSuitabilityProcess, "$Id$")

OpenMS::File::TempDir fixture;
const auto adapter = std::filesystem::u8path(DBSUITABILITY_FAKE_ADAPTER);
std::filesystem::copy_file(adapter, std::filesystem::u8path(fixture.getPath()) / adapter.filename());
ScopedPath path(fixture.getPath());

OpenMS::MSSpectrum spectrum;
spectrum.setMSLevel(2);
spectrum.setRT(42.0);
spectrum.push_back(OpenMS::Peak1D(100.0, 50.0));
OpenMS::MSExperiment spectra;
spectra.addSpectrum(spectrum);
const std::vector<OpenMS::FASTAFile::FASTAEntry> fasta = {
  {"P0", "target", "PEPTIDER"}, {"DECOY_P0", "decoy", "REDITPEP"}
};
const std::string prefix = "DBSuitabilityFakeAdapter:1:";
OpenMS::Param parameters;
parameters.setValue(prefix + "in", "must be replaced");
parameters.setValue(prefix + "database", "must be replaced");
parameters.setValue(prefix + "out", "must be replaced");
parameters.setValue(prefix + "receipt", fixture.getPath() + "receipt.txt");
parameters.setValue(prefix + "mode", "success");

START_SECTION((Adapter resolves through PATH, receives serialized inputs, and returned IDs are indexed))
{
  OpenMS::DBSuitability_friend driver;
  auto peptides = driver.runIdentificationSearch(spectra, fasta, "DBSuitabilityFakeAdapter", parameters);
  TEST_EQUAL(peptides.size(), 2)
  TEST_EQUAL(peptides.at(0).getHits().size(), 1)
  TEST_EQUAL(peptides.at(1).getHits().size(), 1)
  TEST_STRING_EQUAL(peptides.at(0).getHits().at(0).getMetaValue("target_decoy").toString(), "target")
  TEST_STRING_EQUAL(peptides.at(1).getHits().at(0).getMetaValue("target_decoy").toString(), "decoy")
  TEST_STRING_EQUAL(readReceipt(fixture.getPath() + "receipt.txt"), "validated INI, mzML, FASTA and working directory\n")
  const auto temporary = std::filesystem::u8path(parameters.getValue(prefix + "in").toString()).parent_path();
  TEST_EQUAL(std::filesystem::exists(temporary), false)
}
END_SECTION

START_SECTION((Adapter failure preserves its diagnostic and cleans temporary inputs))
{
  std::filesystem::remove(std::filesystem::u8path(fixture.getPath() + "receipt.txt"));
  parameters.setValue(prefix + "mode", "fail");
  OpenMS::DBSuitability_friend driver;
  ErrorCapture errors;
  bool rejected = false;
  try
  {
    driver.runIdentificationSearch(spectra, fasta, "DBSuitabilityFakeAdapter", parameters);
  }
  catch (const OpenMS::Exception::InternalToolError& error)
  {
    rejected = true;
    const auto state = static_cast<int>(OpenMS::ExternalProcess::RETURNSTATE::NONZERO_EXIT);
    TEST_EQUAL(std::string(error.what()).find("Return state was: " + std::to_string(state)) != std::string::npos, true)
  }
  TEST_EQUAL(rejected, true)
  TEST_EQUAL(errors.stream.str().find("FAKE_ADAPTER_STDOUT") != std::string::npos, true)
  TEST_EQUAL(errors.stream.str().find("FAKE_ADAPTER_FAILURE: intentional exit 7") != std::string::npos, true)
  TEST_STRING_EQUAL(readReceipt(fixture.getPath() + "receipt.txt"), "validated INI, mzML, FASTA and working directory\n")
  const auto temporary = std::filesystem::u8path(parameters.getValue(prefix + "in").toString()).parent_path();
  TEST_EQUAL(std::filesystem::exists(temporary), false)
}
END_SECTION

END_TEST
