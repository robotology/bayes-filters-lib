# 📜 BayesFilters changelog

## Version 0.7.101
###### `CMake`
 - Devel branch will now have +100 on the patch number to differentiate from master branch.
 - Fourth number of the project version has been removed to be compliant with SemVer system.


## Version 0.7.1.0
##### `Bugfix`
 - Fix WhiteNoiseAcceleration implementation.


## Version 0.7.0.0
##### `Filtering classes`
 - Remove PFVisualCorrection::getVisualObservationModel() and PFVisualCorrection::setVisualObservationModel().

##### `Bugfix`
 - Fix PF prediction and correction move constructor (minor fix).

##### `CMake`
  - Threads is now a dependency of the exported library. This will solve a CMake error during configuration in CMake projects linking BayesFilters.
  - BayesFilters is now fully relocatable.


## Version 0.6.2.0
### Updates
##### `Filtering classes`
 - Add ResamplingWithPrior class.
 - Add HistoryBuffer and EstimatesExtraction classes.

##### `Filtering Features`
- Add 'all' option to skip() method of particle filters. This option is particularly useful to reset the internal status of the filters.

##### `Dependencies`
 - Removed Eigen::NullaryExpr() calls. Now the library can be compiled with any Eigen 3.3 version.

##### `Bugfix`
 - Windows is now fully supported and tested.

##### `Test`
  - Add Appveyor and Travis yml files. Tests still need to be improved.

##### `CMake`
 - Update CMake modules.


## Version 0.6.1.0
### Updates
##### `CMake`
 - Update installation helper files, which are updated from YCM commit f162fcb.


## Version 0.6.0.1
### Bugfixes
 - `FilteringAlgorithm::filteringRecursion`: fix the invocation position of initialization.

## Version 0.6.0.0
 - This is the first public release of the BayesFilters library.
