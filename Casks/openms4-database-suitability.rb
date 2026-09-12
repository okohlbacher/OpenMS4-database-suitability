cask "openms4-database-suitability" do
  arch arm: "arm64", intel: "x64"

  version "1.0.0-ci.2,c46998ff1001"
  sha256 arm:   "4696b6b3652e2e6e5abc51fd33a4e0e8ec2d0b5f1880f147afb52204c0ec7085",
         intel: "806b108c6c0dc85184851c21d7f919092b56d50527e5cf2b812a405eb2a72b60"

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
