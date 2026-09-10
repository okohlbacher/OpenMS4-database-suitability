# OpenMSDatabaseSuitability experimental

Independent `DatabaseSuitability` package consuming the installed exact Core/CLI revisions in `dependencies.lock.json`. The executable name and options are unchanged. Private implementation: `DBSuitability`; its focused class test moves with it. Reusable format readers/writers and scientific primitives remain Core. See `migration.json` for original source hashes/paths and `License.txt` for attribution terms.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=/path/to/sdk -DCMAKE_INSTALL_PREFIX=/path/to/sdk
cmake --build build --parallel
ctest --test-dir build --output-on-failure
cmake --install build
```

The installed manifest under `share/openms4/tools` registers this product independently. Use `OPENMS_TOOL_PREFIX_PATH` for tool discovery. A combined install prefix supplies relative runtime library paths; separate fixed prefixes require `CMAKE_INSTALL_RPATH`. Native dependencies are not downloaded. Published builds use `OPENMS4_REQUIRE_CLEAN_SOURCE=ON`. Original numerical fixtures remain in the installed TestData suite. External search engines/services retain their existing configuration requirements.
