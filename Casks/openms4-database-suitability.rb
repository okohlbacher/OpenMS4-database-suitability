cask "openms4-database-suitability" do
  arch arm: "arm64", intel: "x64"

  version "1.0.0-ci.1,677e58160bff"
  sha256 arm:   "2832e54aa2df2cdca692d6156a920554b6bf91da575cb6109d120df1a832a38d",
         intel: "2f8113975d284a1185166b8707fa29c4b3b7b0355111ae45d961dd617c15df53"

  url "https://github.com/okohlbacher/OpenMS4-database-suitability/releases/download/" \
      "database-suitability-v#{version.csv.first}/OpenMS4-database-suitability-macos-#{arch}-Homebrew-#{version.csv.second}.tar.gz"
  name "OpenMS 4 database-suitability tools"
  desc "Command-line mass-spectrometry tools built against the OpenMS Core SDK"
  homepage "https://github.com/okohlbacher/OpenMS4-database-suitability"

  depends_on formula: "okohlbacher/openms4-core/openms4-core"
  depends_on macos: :sequoia

  payload = "OpenMS4-database-suitability-macos-#{arch}-Homebrew-#{version.csv.second}"
  binary "#{payload}/bin/DatabaseSuitability"

  postflight_steps do
    run "/usr/bin/xattr",
        args:           ["-dr", "com.apple.quarantine", "."],
        chdir:          ".",
        writable_paths: ["."]
  end
end
