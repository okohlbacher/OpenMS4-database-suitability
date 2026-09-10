// Copyright (c) 2002-present, OpenMS Inc. -- EKU Tuebingen, ETH Zurich, and FU Berlin
// SPDX-License-Identifier: BSD-3-Clause
//
// --------------------------------------------------------------------------
// $Maintainer: Tom Waschischeck $
// $Authors: Tom Waschischeck $
// --------------------------------------------------------------------------


#pragma once
#include <OpenMS/QC/DBSuitability.h>

namespace OpenMS
{
  // friend class to test private member functions
  class DBSuitability_friend
  {
  public:
    DBSuitability_friend() = default;

    ~DBSuitability_friend() = default;

    std::vector<FASTAFile::FASTAEntry> getSubsampledFasta(const std::vector<FASTAFile::FASTAEntry>& fasta_data, double subsampling_rate)
    {
      return suit_.getSubsampledFasta_(fasta_data, subsampling_rate);
    }

    void appendDecoys(std::vector<FASTAFile::FASTAEntry>& fasta)
    {
      suit_.appendDecoys_(fasta);
    }

    double calculateCorrectionFactor(const DBSuitability::SuitabilityData& data, const DBSuitability::SuitabilityData& data_sampled, double sampling_rate)
    {
      return suit_.calculateCorrectionFactor_(data, data_sampled, sampling_rate);
    }

    UInt numberOfUniqueProteins(const PeptideIdentificationList& peps, UInt number_of_hits = 1)
    {
      return suit_.numberOfUniqueProteins_(peps, number_of_hits);
    }

    Size getIndexWithMedianNovoHits(const std::vector<DBSuitability::SuitabilityData>& data)
    {
      return suit_.getIndexWithMedianNovoHits_(data);
    }

    double getScoreMatchingFDR(const PeptideIdentificationList& pep_ids, double FDR, std::string score_name, bool higher_score_better)
    {
      return suit_.getScoreMatchingFDR_(pep_ids, FDR, score_name, higher_score_better);
    }

    PeptideIdentificationList runIdentificationSearch(const MSExperiment& spectra,
        const std::vector<FASTAFile::FASTAEntry>& fasta, const std::string& adapter,
        Param& parameters)
    {
      return suit_.runIdentificationSearch_(spectra, fasta, adapter, parameters);
    }

    /* Not tested:
      getDecoyDiff_, getDecoyCutOff_, isNovoHit_, checkScoreBetterThanThreshold_
      Reason: These functions are essential to the normal suitability calculation and if something would not work, the test for 'compute' would fail.

      extractSearchAdapterInfoFromMetaValues_, extractScore_
      Reason: These functions are very straightforeward.
    */

  private:
    DBSuitability suit_;
  };
}
