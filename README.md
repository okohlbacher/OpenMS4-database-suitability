# OpenMSDatabaseSuitability experimental

Independent `DatabaseSuitability` package consuming the installed exact Core/CLI revisions in `dependencies.lock.json`. The executable name and options are unchanged. Private implementation: `DBSuitability`; its focused class test moves with it. Reusable format readers/writers and scientific primitives remain Core. See `migration.json` for original source hashes/paths and `License.txt` for attribution terms.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=/path/to/sdk -DCMAKE_INSTALL_PREFIX=/path/to/sdk
cmake --build build --parallel
ctest --test-dir build --output-on-failure
cmake --install build
```

The installed manifest under `share/openms4/tools` registers this product independently. Use `OPENMS_TOOL_PREFIX_PATH` for tool discovery. A combined install prefix supplies relative runtime library paths; separate fixed prefixes require `CMAKE_INSTALL_RPATH`. Native dependencies are not downloaded. Published builds use `OPENMS4_REQUIRE_CLEAN_SOURCE=ON`. Original numerical fixtures remain in the installed TestData suite. External search engines/services retain their existing configuration requirements.

<!-- package-graph:begin -->
## Where this package sits

![OpenMS 4 package architecture](docs/package-architecture.svg)

`database-suitability` builds against the installed **core**, **cli**, **test-data** packages at the revisions recorded in [`dependencies.lock.json`](dependencies.lock.json). No other package builds against it.

| Repository | Relation | Contents |
| --- | --- | --- |
| [OpenMS4-core](https://github.com/okohlbacher/OpenMS4-core) | dependency | scientific library, OpenSwathAlgo, readers and writers, runtime data, optional TestSupport |
| [OpenMS4-cli](https://github.com/okohlbacher/OpenMS4-cli) | dependency | TOPPBase, tool registration and discovery |
| [OpenMS4-test-data](https://github.com/okohlbacher/OpenMS4-test-data) | dependency | versioned fixtures and the installed numerical suite |

The eighteen repositories are assembled by the parent repository
[OpenMS4-tests](https://github.com/okohlbacher/OpenMS4-tests), which holds the submodule pins (`packages.lock.json`), the
dependency-order build runner and the contract tests that keep the graph consistent.
[`docs/project-state.md`](https://github.com/okohlbacher/OpenMS4-tests/blob/codex/package-split/docs/project-state.md) is the current state
of the whole project; [`docs/build-split-packages.md`](https://github.com/okohlbacher/OpenMS4-tests/blob/codex/package-split/docs/build-split-packages.md)
reproduces the installed-SDK build.
<!-- package-graph:end -->
