cask "openms4-database-suitability" do
  arch arm: "arm64", intel: "x64"

  version "1.0.0-ci.3,79021a07180d"
  sha256 arm:   "7c00b7bf81bcf706fd6f6efe9396d6bd462179933bcd70e7117503278e75dc59",
         intel: "a6c9a5fb4b026bfe1748940774b6e02dc820b5df5cdfe34bfbf6980ce8e12ea0"

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
    next if core == "ac41cc177023e24a8fbc711a6ce9010187c54c44"

    raise Cask::CaskError, "openms4-database-suitability #{version.csv.first} was built against openms4-core ac41cc177023, " \
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
