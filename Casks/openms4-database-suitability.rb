cask "openms4-database-suitability" do
  arch arm: "arm64", intel: "x64"

  version "1.0.0-ci.4,17ca75dc5467"
  sha256 arm:   "8dad82752480dffd1aa71cfbfa6b8a8aff3cac5a975bd6a24ad00b9c0e58ddeb",
         intel: "e6811def041ebc06f0feae1d7a1f38dfa921e0c28c785a9bd3d006f258285993"

  url "https://github.com/okohlbacher/OpenMS4-database-suitability/releases/download/" \
      "database-suitability-v#{version.csv.first}/OpenMS4-database-suitability-macos-#{arch}-Homebrew-#{version.csv.second}.tar.gz"
  name "OpenMS 4 database-suitability tools"
  desc "Command-line mass-spectrometry tools built against the OpenMS Core SDK"
  homepage "https://github.com/okohlbacher/OpenMS4-database-suitability"

  depends_on formula: "okohlbacher/openms4-core/openms4-core"
  depends_on macos: :sequoia

  payload = "OpenMS4-database-suitability-macos-#{arch}-Homebrew-#{version.csv.second}"
  binary "#{payload}/bin/DatabaseSuitability"

  # libOpenMS has no versioned name, so a payload only runs with the Core it was built against.
  preflight do
    config = "#{HOMEBREW_PREFIX}/opt/openms4-core/lib/cmake/OpenMS/OpenMSConfig.cmake"
    core = File.exist?(config) ? File.read(config)[/set\(OpenMS_SOURCE_REVISION "([0-9a-f]{40})"\)/, 1] : nil
    next if core == "84847138c0de67149601aaa860af7ac8e2e64534"

    raise Cask::CaskError, "openms4-database-suitability #{version.csv.first} was built against openms4-core 84847138c0de, " \
                           "but the installed openms4-core is #{core&.slice(0, 12) || "unknown"}. " \
                           "Install the openms4-database-suitability release built for the installed Core."
  end

  postflight_steps do
    run "/usr/bin/xattr",
        args:           ["-dr", "com.apple.quarantine", "."],
        chdir:          ".",
        writable_paths: ["."]
  end
end
