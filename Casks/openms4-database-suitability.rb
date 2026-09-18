cask "openms4-database-suitability" do
  arch arm: "arm64", intel: "x64"

  version "1.0.0-ci.5,0cdbd1a0571b"
  sha256 arm:   "c680a0d01b690c303a9325a113c56ffd182f507c84e100df81de4f2395112343",
         intel: "753d43dda726d2deb12d1bdedb1b117e1ac9cd0c8b3c1fc51be54616d9923aef"

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
    next if core == "eb58e981d7e0864634b59230874a56a1512369f7"

    raise Cask::CaskError, "openms4-database-suitability #{version.csv.first} was built against openms4-core eb58e981d7e0, " \
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
