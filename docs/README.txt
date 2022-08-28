
Brief:     What does each folder of the project mean?
Author(s): ntvisigoth@gmail.com
Created:   02.09.2011
Modified:  16.03.2016



$(RootProjectsDir)\docs\ - The project documentation

$(RootProjectsDir)\libraries\ - Sources of Framework and research libraries

$(RootProjectsDir)\ReadPE\ - PE32/PE32+ files Dumper. Output format is JSOON

$(RootProjectsDir)\third_party\ - Source of third party libraries

$(RootProjectsDir)\test-data\test-samples\ - Test-vectors for application smoky-testing

$(RootProjectsDir)\test-data\expected-results\ - Expected test-reports

$(RootProjectsDir)\scripts\ - Infrastructure-scripts
 * UpdateVersion.py - Updates the Application version info
 * WhiteBox.cmd - It tests the project by 'White box' method. It uses unit-tests application
 * boost_compile.cmd - The Boost libraries building
 * makeTestPathsHpp.py - Autogeneration testPaths.hpp for unit-test
 * SmokyTest.py - Make smoky testing of the readpe tool
 * make_release.cmd - Creates readpe release archive with
 * make_test_report.py - Make test report for smoky testing
