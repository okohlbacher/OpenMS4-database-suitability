// SPDX-License-Identifier: BSD-3-Clause
// Test executable: validate the real driver's files and emit a tiny valid result.
#include <OpenMS/CHEMISTRY/AASequence.h>
#include <OpenMS/CHEMISTRY/ProteaseDB.h>
#include <OpenMS/FORMAT/FASTAFile.h>
#include <OpenMS/FORMAT/FileHandler.h>
#include <OpenMS/FORMAT/ParamXMLFile.h>
#include <OpenMS/KERNEL/MSExperiment.h>
#include <OpenMS/METADATA/PeptideHit.h>
#include <OpenMS/METADATA/PeptideIdentification.h>
#include <OpenMS/METADATA/PeptideIdentificationList.h>
#include <OpenMS/METADATA/ProteinIdentification.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

int main(int argc, char** argv)
{
  try
  {
    if (argc != 3 || std::string(argv[1]) != "-ini")
    {
      throw std::runtime_error("expected exactly -ini <file>");
    }
    OpenMS::Param parameters;
    OpenMS::ParamXMLFile().load(argv[2], parameters);
    const std::string prefix = "DBSuitabilityFakeAdapter:1:";
    const std::string input = parameters.getValue(prefix + "in").toString();
    const std::string database = parameters.getValue(prefix + "database").toString();
    const std::string output = parameters.getValue(prefix + "out").toString();
    const auto directory = std::filesystem::current_path();
    const auto is_temporary = [&](const std::string& value, const std::string& filename)
    {
      const auto path = std::filesystem::u8path(value);
      return path.filename() == filename && std::filesystem::equivalent(path.parent_path(), directory);
    };
    if (!is_temporary(argv[2], "parameters.INI") || !is_temporary(input, "spectra.mzML") ||
        !is_temporary(database, "database.FASTA") || !is_temporary(output, "out.idXML"))
    {
      throw std::runtime_error("driver did not provide its temporary working-directory paths");
    }

    OpenMS::MSExperiment spectra;
    OpenMS::FileHandler().loadExperiment(input, spectra, {OpenMS::FileTypes::MZML});
    std::vector<OpenMS::FASTAFile::FASTAEntry> fasta;
    OpenMS::FASTAFile().load(database, fasta);
    if (spectra.size() != 1 || spectra[0].getMSLevel() != 2 || spectra[0].getRT() != 42.0 ||
        spectra[0].size() != 1 || spectra[0][0].getMZ() != 100.0 || spectra[0][0].getIntensity() != 50.0f ||
        fasta.size() != 2 || fasta[0].identifier != "P0" || fasta[0].sequence != "PEPTIDER" ||
        fasta[1].identifier != "DECOY_P0" || fasta[1].sequence != "REDITPEP")
    {
      throw std::runtime_error("driver's serialized spectra/FASTA changed");
    }
    std::ofstream receipt(std::filesystem::u8path(parameters.getValue(prefix + "receipt").toString()));
    receipt << "validated INI, mzML, FASTA and working directory\n";
    receipt.close();
    if (!receipt)
    {
      throw std::runtime_error("could not write adapter receipt");
    }
    if (parameters.getValue(prefix + "mode").toString() == "fail")
    {
      std::cout << "FAKE_ADAPTER_STDOUT\n";
      std::cerr << "FAKE_ADAPTER_FAILURE: intentional exit 7\n";
      return 7;
    }

    OpenMS::ProteinIdentification proteins;
    proteins.setIdentifier("fake-search");
    OpenMS::ProteinIdentification::SearchParameters search_parameters;
    search_parameters.digestion_enzyme = *OpenMS::ProteaseDB::getInstance()->getEnzyme("Trypsin");
    proteins.setSearchParameters(search_parameters);
    OpenMS::PeptideIdentificationList peptides;
    for (const auto& entry : fasta)
    {
      OpenMS::PeptideHit hit;
      hit.setSequence(OpenMS::AASequence::fromString(entry.sequence));
      hit.setCharge(2);
      hit.setScore(1.0);
      OpenMS::PeptideIdentification peptide;
      peptide.setIdentifier("fake-search");
      peptide.setScoreType("fake-score");
      peptide.setHigherScoreBetter(true);
      peptide.setHits({hit});
      peptides.push_back(peptide);
    }
    OpenMS::FileHandler().storeIdentifications(output, {proteins}, peptides, {OpenMS::FileTypes::IDXML});
    return 0;
  }
  catch (const std::exception& error)
  {
    std::cerr << "FAKE_ADAPTER_CONTRACT_ERROR: " << error.what() << '\n';
    return 3;
  }
}
