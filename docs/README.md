
# What does each folder of the project mean?

- `$(RootProjectsDir)/docs/` - The project documentation
- `$(RootProjectsDir)/src/` - Sources of `Framework`, `PE` libraries and `ReadPE` tool
- `$(RootProjectsDir)/third-party/` - Source of third party libraries.
- `$(RootProjectsDir)/test-data/test-samples/` - Test files for application smoke testing
- `$(RootProjectsDir)\test-data/expected-results/` - Expected test-reports
- `$(RootProjectsDir)/scripts/ - Infrastructure-scripts`

# Infrastructure scripts

- `UpdateVersion.py` - Updates the Application version info
- `WhiteBox.cmd` - Run unit-tests applications for `Framework` and `PE` libraries testing.
- `boost_compile.cmd` - The Boost libraries building
- `make_release.cmd` - Create readpe release archive with
